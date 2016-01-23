//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Font_Imp.h"
#include "../asd.Graphics_Imp.h"

#ifdef WIN32
#ifdef min
#undef min
#endif 
#ifdef max
#undef max
#endif
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {

	Font_Imp* Font_Imp::Create(Graphics* graphics, const achar* font, int32_t fontSize, Color color, int32_t outlineSize, Color outlineColor)
	{
		auto g = (Graphics_Imp*) graphics;
		auto file = g->GetFile();
	
		std::shared_ptr<StaticFile> staticFile = nullptr;
		
		// 規定のフォント
		if (font == nullptr || astring(font) == astring())
		{
#ifdef _WIN32
			staticFile = file->CreateStaticFile(ToAString("C:\\Windows\\Fonts\\meiryo.ttc").c_str());
#elif defined __APPLE__
			staticFile = file->CreateStaticFile(ToAString("/Library/Fonts/ヒラギノ丸ゴ Pro W4.otf").c_str());
#endif
		}

		// ファイルパスとして検索
		if (font != nullptr && staticFile == nullptr)
		{
			staticFile = file->CreateStaticFile(font);
		}

		// フォント名から検索
		if (font != nullptr && staticFile == nullptr)
		{
			InstalledFontList::Load();

			auto f = astring(font);

			for (auto i = 0; i < InstalledFontList::Fonts.size(); i++)
			{
				if (InstalledFontList::Fonts[i].Name == f)
				{
					staticFile = file->CreateStaticFile(InstalledFontList::Fonts[i].Path.c_str());
					break;
				}
			}
		}

		// フォントが見つからなかった
		if (staticFile == nullptr) return nullptr;

		// ライタライザを生成する
		auto rasterizer = std::make_shared<asd::FontRasterizer>();
		if (!rasterizer->Initialize(staticFile->GetData(), staticFile->GetSize(), fontSize, outlineSize, color, outlineColor, 1024))
		{
			return nullptr;
		}

		return new Font_Imp(graphics, rasterizer);
	}

	Font_Imp::Font_Imp(Graphics* graphics, std::shared_ptr<FontRasterizer> rasterizer)
		: DeviceObject(graphics)
		, m_graphics(graphics)
		, isDynamic(true)
		, rasterizer(rasterizer)
	{

	}

	Font_Imp::Font_Imp(Graphics* graphics, const achar* affFilePathChar, std::vector<uint8_t> data)
		: DeviceObject(graphics)
		, m_graphics(graphics)
		, isDynamic(false)
	{
		asd::astring affFilePathStr = asd::astring(affFilePathChar);
		AffLoader affLoader = AffLoader(data);
		m_glyphs = affLoader.GetGlyphs();

		//AFFファイルの拡張子以前のパスを取得。
		asd::astring rawFilePath = affFilePathStr.substr(0, affFilePathStr.length() - 4);

		int pictureNumber = 0;

		asd::astring pngExtension = ToAString(".png");

		while (true)
		{
			//連番を文字列化。
			asd::astring strNumber = asd::ToAString(std::to_string(pictureNumber).c_str());

			//PNGファイルへのパス文字列を構成。
			asd::astring pngFilePath = (rawFilePath + ToAString("_") + strNumber + pngExtension);

			//この連番のファイルが存在するか否か調べて、存在しなかったらループを抜ける。

			auto texture = m_graphics->CreateTexture2D(pngFilePath.c_str());

			if (texture == nullptr)
			{
				break;
			}

			m_textures.push_back(texture);


			++pictureNumber;
		}
	}

	Font_Imp::Font_Imp(Graphics* graphics, void* data, int32_t size, std::vector<std::shared_ptr<Texture2D>> textures)
		: DeviceObject(graphics)
		, m_graphics(graphics)
		, isDynamic(false)
	{
		std::vector<uint8_t> data_;
		data_.resize(size);
		memcpy(data_.data(), data, size);

		AffLoader affLoader = AffLoader(data_);
		m_glyphs = affLoader.GetGlyphs();

		m_textures = textures;
	}

	Font_Imp::~Font_Imp()
	{
		m_textures.clear();
		m_glyphs.clear();
		auto g = (Graphics_Imp*)GetGraphics();
		g->FontContainer->Unregister(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Vector2DI Font_Imp::CalcTextureSize(const achar* text, WritingDirection writingDirection)
	{
		if (text == nullptr) return Vector2DI(0, 0);

		if (isDynamic)
		{
			AddCharactorsDynamically(text);
		}

		if (m_glyphs.empty() && m_textures.empty())
		{
			return Vector2DI(0, 0);
		}

		Vector2DI sumSize = Vector2DI(0, 0);
		Vector2DI size = Vector2DI(0, 0);
		astring t(text);
		t.append(ToAString("\n"));
		for (auto c = t.begin(); c != t.end(); ++c)
		{
			if (*c != '\n'&&!HasGlyphData(*c))
			{
				continue;
			}
			else if (*c == '\n')
			{
				if (writingDirection == WritingDirection::Horizontal)
				{
					sumSize.X = std::max(size.X, sumSize.X);
					sumSize.Y += size.Y;
				}
				else
				{
					sumSize.X += size.X;
					sumSize.Y = std::max(size.Y, sumSize.Y);
				}
				size = Vector2DI(0, 0);
				continue;
			}

			auto src = m_glyphs.at(*c).GetSrc();
			if (writingDirection == WritingDirection::Horizontal)
			{
				size.X += src.Width;
				size.Y = std::max(src.Height, size.Y);
			}
			else
			{
				size.X = std::max(src.Width, size.X);
				size.Y += src.Height;
			}
		}
		return sumSize;

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	std::shared_ptr<Texture2D> Font_Imp::GetTexture(int index) const
	{
		if (index < 0 || index >= m_textures.size())
		{
			return nullptr;
		}
		return m_textures[index];
	}

	void Font_Imp::AddCharactorDynamically(achar c)
	{
		if (!isDynamic) return;
		if (m_glyphs.find(c) != m_glyphs.end()) return;
		
		auto ig = rasterizer->AddGlyph(c);
	
		GlyphData gd = GlyphData(c, ig.Index, ig.Src);
		m_glyphs[c] = gd;

		updatingTexture.insert(ig.Index);
	}

	void Font_Imp::AddCharactorsDynamically(const achar* text)
	{
		if (!isDynamic) return;
		if (text == nullptr) return;
		
		for (int32_t i = 0; text[i] != 0; i++)
		{
			AddCharactorDynamically(text[i]);
		}
	}

	void Font_Imp::UpdateTextureDynamically()
	{
		auto g = (Graphics_Imp*) GetGraphics();

		for (auto index : updatingTexture)
		{
			while (index >= m_textures.size())
			{
				m_textures.push_back(
					g->CreateEmptyTexture2D(rasterizer->GetImageSize(), rasterizer->GetImageSize(), asd::TextureFormat::R8G8B8A8_UNORM_SRGB));
			}

			auto& buf = rasterizer->GetImages()[index];
			auto& tex = m_textures[index];

			TextureLockInfomation info;
			if (tex->Lock(&info))
			{
				auto p = info.GetPixels();
				memcpy(p, buf->Buffer.data(), rasterizer->GetImageSize() * rasterizer->GetImageSize() * 4);
				tex->Unlock();
			}
		}

		updatingTexture.clear();
	}

	GlyphData Font_Imp::GetGlyphData(achar c)
	{
		return m_glyphs[c];
	}

	bool Font_Imp::HasGlyphData(achar c) const
	{
		return m_glyphs.find(c) != m_glyphs.end();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Font_Imp::Reload(const achar* affFilePathChar, std::vector<uint8_t> data)
	{
		if (isDynamic) return;

		m_glyphs.clear();
		m_textures.clear();

		asd::astring affFilePathStr = asd::astring(affFilePathChar);
		AffLoader affLoader = AffLoader(data);
		m_glyphs = affLoader.GetGlyphs();

		//AFFファイルの拡張子以前のパスを取得。
		asd::astring rawFilePath = affFilePathStr.substr(0, affFilePathStr.length() - 4);

		int pictureNumber = 0;

		asd::astring pngExtension = ToAString(".png");

		while (true)
		{
			//連番を文字列化。
			asd::astring strNumber = asd::astring(asd::ToAString(std::to_string(pictureNumber).c_str()));

			//PNGファイルへのパス文字列を構成。
			asd::astring pngFilePath = (rawFilePath + ToAString("_") + strNumber + pngExtension);

			auto texture = m_graphics->CreateTexture2D(pngFilePath.c_str());

			if (texture == nullptr)
			{
				break;
			}

			m_textures.push_back(texture);


			++pictureNumber;
		}
	}
}