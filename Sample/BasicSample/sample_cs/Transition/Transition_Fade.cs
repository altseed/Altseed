using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs
{
	class Transition_Fade : ISample
	{
		// フェードアウト・フェードインでシーン遷移をするサンプルです。
		public void Run()
		{
			asd.Engine.Initialize("Transition_Fade", 640, 480, new asd.EngineOption());

			// シーン(1)、レイヤー、オブジェクトを生成する。
			var scene1 = new asd.Scene();
			var layer1 = new asd.Layer2D();
			var object1 = new asd.TextureObject2D();

			// テクスチャを生成し、オブジェクトに設定する。
			var texture1 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Scene1.png");
			object1.Texture = texture1;

			// シーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
			scene1.AddLayer(layer1);
			layer1.AddObject(object1);


			// 上と同じものをもう１セット作る。
			var scene2 = new asd.Scene();
			var layer2 = new asd.Layer2D();
			var object2 = new asd.TextureObject2D();

			var texture2 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Scene2.png");
			object2.Texture = texture2;

			scene2.AddLayer(layer2);
			layer2.AddObject(object2);


			// シーンをシーン1に設定する。
			asd.Engine.ChangeScene(scene1);

			while(asd.Engine.DoEvents())
			{
				asd.Engine.Update();

				// マウスの左ボタンが押されるのを待つ。
				if(asd.Engine.Mouse.LeftButton.ButtonState == asd.MouseButtonState.Push)
				{
					// フェードアウト・インによるシーン遷移を開始する。
					// 1秒かけてフェードアウトし、1.5秒かけてフェードイン。
					asd.Engine.ChangeSceneWithTransition(scene2, new asd.TransitionFade(1.0f, 1.5f));
				}
			}
		}
	}
}
