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
		class RotateComponent : ace.Object2DComponent
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
			ace.Engine.Initialize("Object2DComponent", 640, 480, new ace.EngineOption());

			// 使用するシーン、レイヤー、オブジェクト、コンポーネントを生成
			var scene = new ace.Scene();
			var layer = new ace.Layer2D();
			var obj = new ace.TextureObject2D();
			var component = new RotateComponent();


			// シーンを変更
			ace.Engine.ChangeScene(scene);

			// シーンにレイヤーを追加
			scene.AddLayer(layer);

			// レイヤーにオブジェクトを追加
			layer.AddObject(obj);

			// オブジェクトに回転コンポーネントをコンポーネント名"Rotate"で追加
			obj.AddComponent(component, "Rotate");


			// テクスチャをロード
			var texture = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Block1.png");

			// オブジェクトに画像をセット
			obj.Texture = texture;

			// オブジェクトの位置を指定
			obj.Position = new ace.Vector2DF(320, 240);

			while(ace.Engine.DoEvents())
			{
				ace.Engine.Update();
			}

			ace.Engine.Terminate();
		}
	}
}
