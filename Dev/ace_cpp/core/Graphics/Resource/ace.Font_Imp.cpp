//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Font_Imp.h"
#include "../ace.Graphics_Imp.h"
#include "../ace.GraphicsResourceContainer.h"


#define Z_SOLO
#include <png.h>
#include <pngstruct.h>
#include <pnginfo.h>

#if _WIN32
#if _DEBUG
#pragma comment(lib,"libpng16.Debug.lib")
#pragma comment(lib,"zlib.Debug.lib")
#else
#pragma comment(lib,"libpng16.Release.lib")
#pragma comment(lib,"zlib.Release.lib")
#endif
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Font_Imp::Font_Imp(Graphics* graphics, const achar* affFilePath)
		: DeviceObject(graphics)
		, m_graphics(graphics)
	{
		const ace::astring affFilePathStr = ace::astring(affFilePath);
		AffLoader affLoader = AffLoader(affFilePathStr);
		m_glyphs = affLoader.GetGlyphs();

		//AFFファイルの拡張子以前のパスを取得。
		const ace::astring rawFilePath = affFilePathStr.substr(0, affFilePathStr.length() - 4);

		int pictureNumber = 1;

		const ace::astring pngExtension = ace::astring(ToAString(".png"));

		while (true)
		{
			//連番を文字列化。
			const ace::astring strNumber = ace::ToAString(std::to_string(pictureNumber).c_str());

			//PNGファイルへのパス文字列を構成。
			const ace::astring pngFilePath = (rawFilePath + ace::astring(ToAString("_")) + strNumber + pngExtension);

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
		g->GetResourceContainer()->Fonts.Unregist(this);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Vector2DI Font_Imp::CalcTextureSize(achar* text, WritingDirection writingDirection)
	{
		if (m_glyphs.empty() && m_textures.empty())
		{
			return Vector2DI(0, 0);
		}
		else
		{
			Vector2DI size = Vector2DI(0, 0);
			astring t = ToAString(text);
			for (auto c = t.begin(); c != t.end(); ++c)
			{
				assert(HasGlyphData(*c));

				auto src = m_glyphs[*c].GetSrc();
				if (writingDirection == WritingDirection::Horizontal)
				{
					size.X += src.Width;
					size.Y = max(src.Height, size.Y);
				}
				else
				{
					size.X = max(src.Width, size.X);
					size.Y += src.Height;
				}
			}

			return size;
		}
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	const std::shared_ptr<Texture2D> Font_Imp::GetTexture(int index) const
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
	const GlyphData Font_Imp::GetGlyphData(achar c)
	{
		return m_glyphs[c];
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	const bool Font_Imp::HasGlyphData(achar c)
	{
		return m_glyphs.count(c) == 1;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Font_Imp::Reload(const achar* affFilePath)
	{
		m_glyphs.clear();
		m_textures.clear();

		const ace::astring affFilePathStr = ace::astring(affFilePath);
		AffLoader affLoader = AffLoader(affFilePathStr);
		m_glyphs = affLoader.GetGlyphs();

		//AFFファイルの拡張子以前のパスを取得。
		const ace::astring rawFilePath = affFilePathStr.substr(0, affFilePathStr.length() - 4);

		int pictureNumber = 0;

		const ace::astring pngExtension = ace::astring(ToAString(".png"));

		while (true)
		{
			//連番を文字列化。
			const ace::astring strNumber = ace::astring(ace::ToAString(std::to_string(pictureNumber).c_str()));

			//PNGファイルへのパス文字列を構成。
			const ace::astring pngFilePath = (rawFilePath + ace::astring(ToAString("_")) + strNumber + pngExtension);

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