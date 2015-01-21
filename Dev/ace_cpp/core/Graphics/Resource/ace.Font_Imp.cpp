//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Font_Imp.h"
#include "../ace.Graphics_Imp.h"

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
namespace ace {

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Font_Imp::Font_Imp(Graphics* graphics, const achar* affFilePathChar, std::vector<uint8_t> data)
		: DeviceObject(graphics)
		, m_graphics(graphics)
	{
		ace::astring affFilePathStr = ace::astring(affFilePathChar);
		AffLoader affLoader = AffLoader(data);
		m_glyphs = affLoader.GetGlyphs();

		//AFFファイルの拡張子以前のパスを取得。
		ace::astring rawFilePath = affFilePathStr.substr(0, affFilePathStr.length() - 4);

		int pictureNumber = 0;

		ace::astring pngExtension = ToAString(".png");

		while (true)
		{
			//連番を文字列化。
			ace::astring strNumber = ace::ToAString(std::to_string(pictureNumber).c_str());

			//PNGファイルへのパス文字列を構成。
			ace::astring pngFilePath = (rawFilePath + ToAString("_") + strNumber + pngExtension);

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

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
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
	Vector2DI Font_Imp::CalcTextureSize(const achar* text, WritingDirection writingDirection) const
	{
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

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	GlyphData Font_Imp::GetGlyphData(achar c)
	{
		return m_glyphs[c];
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Font_Imp::HasGlyphData(achar c) const
	{
		return m_glyphs.find(c) != m_glyphs.end();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Font_Imp::Reload(const achar* affFilePathChar, std::vector<uint8_t> data)
	{
		m_glyphs.clear();
		m_textures.clear();

		ace::astring affFilePathStr = ace::astring(affFilePathChar);
		AffLoader affLoader = AffLoader(data);
		m_glyphs = affLoader.GetGlyphs();

		//AFFファイルの拡張子以前のパスを取得。
		ace::astring rawFilePath = affFilePathStr.substr(0, affFilePathStr.length() - 4);

		int pictureNumber = 0;

		ace::astring pngExtension = ToAString(".png");

		while (true)
		{
			//連番を文字列化。
			ace::astring strNumber = ace::astring(ace::ToAString(std::to_string(pictureNumber).c_str()));

			//PNGファイルへのパス文字列を構成。
			ace::astring pngFilePath = (rawFilePath + ToAString("_") + strNumber + pngExtension);

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