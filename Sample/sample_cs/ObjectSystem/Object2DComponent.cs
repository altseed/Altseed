using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.ObjectSystem
{
	/// <summary>
	/// コンポーネントを用いてオブジェクトを変化させるサンプル。
	/// </summary>
	class Object2DComponent : ISample
	{
		class RotateComponent : asd.Object2DComponent
		{
			protected override void OnUpdate()
			{
				// 毎フレーム、オブジェクトの角度を回転させる
				Owner.Angle += 2;
			}
		}

		public void Run()
		{
			// aceの初期化
			asd.Engine.Initialize("Object2DComponent", 640, 480, new asd.EngineOption());

			// 使用するシーン、レイヤー、オブジェクト、コンポーネントを生成
			var scene = new asd.Scene();
			var layer = new asd.Layer2D();
			var obj = new asd.TextureObject2D();
			var component = new RotateComponent();


			// シーンを変更
			asd.Engine.ChangeScene(scene);

			// シーンにレイヤーを追加
			scene.AddLayer(layer);

			// レイヤーにオブジェクトを追加
			layer.AddObject(obj);

			// オブジェクトに回転コンポーネントをコンポーネント名"Rotate"で追加
			obj.AddComponent(component, "Rotate");


			// テクスチャをロード
			var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Block1.png");

			// オブジェクトに画像をセット
			obj.Texture = texture;

			// オブジェクトの位置を指定
			obj.Position = new asd.Vector2DF(320, 240);

			while(asd.Engine.DoEvents())
			{
				asd.Engine.Update();
			}

			asd.Engine.Terminate();
		}
	}
}
