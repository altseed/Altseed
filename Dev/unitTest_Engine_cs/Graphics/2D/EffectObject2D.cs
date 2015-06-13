using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics._2D
{
	class EffectObject2D : EngineTest
	{
		asd.EffectObject2D m_effectObj;

		public EffectObject2D()
			: base(20)
		{
		}

		protected override void OnStart()
		{
			var scene = new asd.Scene();
			var layer = new asd.Layer2D();
			var effect = asd.Engine.Graphics.CreateEffect("Data/Effect/Laser01.efk");
			m_effectObj = new asd.EffectObject2D();

			layer.AddObject(m_effectObj);
			scene.AddLayer(layer);
			asd.Engine.ChangeScene(scene);

			m_effectObj.Position = new asd.Vector2DF(320, 240);
			m_effectObj.Scale = new asd.Vector2DF(20, 20);
			m_effectObj.EffectRotation = 90;

			m_effectObj.Effect = effect;
			m_effectObj.Play();
		}

		protected override void OnUpdating()
		{
			base.OnUpdating();
		}
	}
}
