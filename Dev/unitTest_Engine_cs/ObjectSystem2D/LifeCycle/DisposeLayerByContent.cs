using System;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D.LifeCycle
{
	/// <summary>
	/// レイヤーが、持っているオブジェクトのUpdate中に破棄された場合のテスト。
	/// </summary>
	class DisposeLayerByContent : EngineTest
	{
		class DisposingObject : TextureObject2D
		{
			private int time;

			public DisposingObject()
			{
				Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath);
			}

			protected override void OnUpdate()
			{
				++time;
				if (time == 10)
				{
					Layer.Dispose();
				}
			}
		}

		private Scene scene_;

		public DisposeLayerByContent()
			: base(300)
		{
		}

		protected override void OnStart()
		{
			scene_ = new Scene();
			var layer = new Layer2D();
			layer.AddObject(new DisposingObject());
			scene_.AddLayer(layer);
			Engine.ChangeScene(scene_);
		}

		protected override void OnUpdating()
		{
			if (Time == 120)
			{
				GC.Collect();
			}
		}
	}
}
