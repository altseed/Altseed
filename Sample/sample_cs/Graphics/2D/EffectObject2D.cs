using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.Graphics._2D
{
	/// <summary>
	/// エフェクトを表示する。
	/// </summary>
	class EffectObject2D : ISample
	{
		public void Run()
		{
			// aceを初期化する
			asd.Engine.Initialize("EffectObject2D", 640, 480, new asd.EngineOption());

			// シーンを生成する
			var scene = new asd.Scene();

			// レイヤーを生成する
			var layer = new asd.Layer2D();

			// シーンにレイヤーを追加する
			scene.AddLayer(layer);

			// シーンを切り替える
			asd.Engine.ChangeScene(scene);

			// エフェクトを読み込む
			var effect = asd.Engine.Graphics.CreateEffect("Data/Effect/magic.efk");

			// エフェクトオブジェクトを生成する
			var effectObj = new asd.EffectObject2D();

			// エフェクトをレイヤーに追加する
			layer.AddObject(effectObj);

			// エフェクトの位置、大きさを指定する
			effectObj.Position = new asd.Vector2DF(320, 240);
			effectObj.Scale = new asd.Vector2DF(50, 50);

			// エフェクトを設定する
			effectObj.Effect = effect;

			// エフェクトを再生する
			effectObj.Play();

			// aceが進行可能かチェックする。
			while (asd.Engine.DoEvents())
			{
				// aceを更新する。
				asd.Engine.Update();
			}

			// aceを終了する。
			asd.Engine.Terminate();
		}
	}
}
