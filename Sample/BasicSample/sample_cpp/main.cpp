#include "SampleGuide.h"
#include "Browser\SampleBrowser.h"
#define INFO(func, title, description) { func, asd::ToAString(#func), asd::ToAString(title), asd::ToAString(description), true }

#if _WIN32
#ifdef _DEBUG
#pragma comment(lib,"Debug/Altseed.lib")
#else
#pragma comment(lib,"Release/Altseed.lib")
#endif
#endif

#include <string>

#if _WIN32
#include <Windows.h>
std::wstring ToWide(const char* pText);
void GetDirectoryName(char* dst, char* src);
#endif

extern void Basic_Empty();
extern void Basic_FullScreen();

extern void PostEffect_GrayScale();
extern void PostEffect_Sepia();
extern void PostEffect_GaussianBlur();
extern void PostEffect_LightBloom();

extern void Transition_Fade();

extern void Keyboard_Basic();
extern void Keyboard_SingularKey();
extern void Keyboard_PuralKeys();

extern void Mouse_Position();
extern void Mouse_Click();
extern void Mouse_State();
extern void Mouse_Wheel();

extern void Joystick_Basic();
extern void Joystick_Axis();
extern void Joystick_GamePad();

extern void Sound_SE();
extern void Sound_BGM();
extern void Sound_Loop();

extern void EffectObject2D_Basic();

extern void TextObject2D_Basic();
extern void TextObject2D_DynamicFont();
extern void TextObject2D_Rotate();
extern void TextObject2D_Filtered();
extern void TextObject2D_Centering();

extern void TextureObject2D_Basic();
extern void TextureObject2D_Transform();
extern void TextureObject2D_Turn();
extern void TextureObject2D_Src();

extern void CameraObject2D_Basic();
extern void CameraObject2D_Magnify();
extern void CameraObject2D_Group();
extern void CameraObject2D_Texture();

extern void GeometryObject2D_CircleShape();
extern void GeometryObject2D_ArcShape();
extern void GeometryObject2D_RectangleShape();
extern void GeometryObject2D_PolygonShape();
extern void GeometryObject2D_PolygonShape_Textured();

extern void Collision2D_Basic();
extern void Collision2D_Geometries();

extern void Object2DComponent_Custom();

extern void SceneAndLayer_Basic();
extern void SceneAndLayer_CustomLayer();
extern void SceneAndLayer_CustomScene();

extern void Dispose_Object2D();
extern void Dispose_Layer2D();

extern void File_PackFile();
extern void File_RootDirectory();
extern void File_StaticFile();
extern void File_StreamFile();

extern void EngineMisc_ShowFps();

extern void Texture_Basic();
extern void Texture_Edit();

extern void Log_Basic();
extern void Log_OutputLevel();
extern void Log_Table();

int main(int argc, char** argv)
{
#if _WIN32
	char current_path[MAX_PATH + 1];
	GetDirectoryName(current_path, argv[0]);
	SetCurrentDirectoryA(current_path);
#endif

	std::vector<SampleInfo> samples =
	{
		INFO(Basic_Empty, "Altseedの基本", "空のウィンドウを表示するサンプル。"),
		//INFO(Basic_FullScreen, "フルスクリーンモード", "アプリケーションをフルスクリーンで起動するサンプル。"),

		INFO(TextureObject2D_Basic, "画像の表示", "画像を表示する基本的なサンプルです。"),
		INFO(TextureObject2D_Turn, "画像の上下反転", "画像の上下を反転して描画するサンプルです。"),
		INFO(TextureObject2D_Src, "画像の切り抜き", "画像の一部を切り出して描画するサンプルです。"),
		INFO(TextureObject2D_Transform, "画像の変形", "画像を回転させたり、拡大縮小して描画するサンプルです。"),

		INFO(Mouse_Position, "マウス座標の取得", "マウスの座標を取得し、その位置を表示するサンプルです。"),
		INFO(Mouse_Click, "マウスのクリック検知", "マウスの左クリックを検知するサンプルです。"),
		INFO(Mouse_State, "マウスボタンの状態", "マウスの中ボタンの状態を調べ、状態を表示するサンプルです。"),
		INFO(Mouse_Wheel, "マウスホイール", "マウスホイールの回転の度合いを調べ、表示するサンプルです。"),

		INFO(Keyboard_Basic, "キーボード入力の基本", "Zキーを押しているかどうかを調べ、結果を表示するサンプルです。"),
		INFO(Keyboard_SingularKey, "キーボードの入力状態", "キーボードのキー入力状態を詳しく調べ、状態を表示するサンプルです。"),
		INFO(Keyboard_PuralKeys, "複数のキーの入力状態", "キーボードの複数のキーの入力状態を調べ、表示するサンプルです。"),

		INFO(Joystick_Basic, "ジョイスティックのボタン入力", "ジョイスティックのボタンの入力状態を調べ、表示するサンプルです。"),
		INFO(Joystick_Axis, "アナログスティック入力", "ジョイスティックのアナログスティックの入力状態を調べ、表示するサンプルです。"),
		INFO(Joystick_GamePad, "ゲームパッド入力", "一部のゲームパッドの入力状態を調べ、表示するサンプルです。"),

		INFO(Sound_SE, "効果音の再生", "効果音(Sound Effect)を再生するサンプルです。"),
		INFO(Sound_BGM, "BGMの再生", "BGMを再生するサンプルです。"),
		INFO(Sound_Loop, "BGMのループ再生", "BGMをループ再生するサンプルです。"),

		INFO(TextObject2D_Basic, "文字列の描画", "文字列を描画するサンプルです。"),
		INFO(TextObject2D_DynamicFont, "フォントの動的生成", "フォントをプログラム実行中にに準備して、そのフォントで文字列を描画するサンプルです。"),
		INFO(TextObject2D_Rotate, "文字列の回転", "文字列を回転させて描画するサンプルです。"),
		INFO(TextObject2D_Filtered, "回転した文字列の滑らかな描画", "文字列の回転をしたときにギザギザが発生するのを防ぐ機能を使ったサンプルです。"),
		INFO(TextObject2D_Centering, "文字列の中央揃え", "文字列を中央揃えで描画するサンプルです。"),

		INFO(CameraObject2D_Basic, "カメラの基本", "カメラを用いて描画空間の一部を切り取って描画するサンプルです。"),
		INFO(CameraObject2D_Magnify, "カメラによる拡大描画", "カメラを用いて描画空間の一部を虫眼鏡のように拡大して描画するサンプルです。"),
		INFO(CameraObject2D_Group, "", ""),
		INFO(CameraObject2D_Texture, "", ""),

		INFO(EffectObject2D_Basic, "", ""),

		INFO(GeometryObject2D_CircleShape, "", ""),
		INFO(GeometryObject2D_ArcShape, "", ""),
		INFO(GeometryObject2D_RectangleShape, "", ""),
		INFO(GeometryObject2D_PolygonShape, "", ""),
		INFO(GeometryObject2D_PolygonShape_Textured, "", ""),

		INFO(Object2DComponent_Custom, "", ""),

		INFO(Collision2D_Basic, "", ""),
		INFO(Collision2D_Geometries, "", ""),

		INFO(File_PackFile, "", ""),
		INFO(File_RootDirectory, "", ""),
		INFO(File_StaticFile, "", ""),
		INFO(File_StreamFile, "", ""),

		INFO(SceneAndLayer_Basic, "", ""),
		INFO(SceneAndLayer_CustomLayer, "", ""),
		INFO(SceneAndLayer_CustomScene, "", ""),

		INFO(Transition_Fade, "フェードイン・フェードアウト", "画像を回転させたり、拡大縮小して描画するサンプルです。"),

		INFO(Dispose_Object2D, "", ""),
		INFO(Dispose_Layer2D, "", ""),

		INFO(PostEffect_GrayScale, "", ""),
		INFO(PostEffect_Sepia, "", ""),
		INFO(PostEffect_GaussianBlur, "", ""),
		INFO(PostEffect_LightBloom, "", ""),

		INFO(EngineMisc_ShowFps, "", ""),

		INFO(Texture_Basic, "", ""),
		INFO(Texture_Edit, "", ""),

		INFO(Log_Basic, "ログ出力", "ログファイルにログを出力するサンプルです。"),
		INFO(Log_OutputLevel, "ログの出力レベルの指定", "ログファイルにログを出力するサンプルです。\n出力レベルを指定し、低レベルのログを無視します。"),
		INFO(Log_Table, "表組みされたログ出力", "ログファイルに表組みで表現されたログを出力するサンプルです。"),
	};

	auto browser = SampleBrowser(samples);
	browser.Run();

	return 0;
}

#if _WIN32
static std::wstring ToWide(const char* pText)
{
	int Len = ::MultiByteToWideChar(CP_ACP, 0, pText, -1, NULL, 0);

	wchar_t* pOut = new wchar_t[Len + 1];
	::MultiByteToWideChar(CP_ACP, 0, pText, -1, pOut, Len);
	std::wstring Out(pOut);
	delete [] pOut;

	return Out;
}

void GetDirectoryName(char* dst, char* src)
{
	auto Src = std::string(src);
	int pos = 0;
	int last = 0;
	while (Src.c_str()[pos] != 0)
	{
		dst[pos] = Src.c_str()[pos];

		if (Src.c_str()[pos] == L'\\' || Src.c_str()[pos] == L'/')
		{
			last = pos;
		}

		pos++;
	}

	dst[pos] = 0;
	dst[last] = 0;
}
#endif