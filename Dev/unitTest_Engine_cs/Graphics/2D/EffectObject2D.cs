using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics._2D
{
	class EffectObject2D : EngineTest
	{
		ace.EffectObject2D m_effectObj;

		public EffectObject2D()
			: base(20)
		{
		}

		protected override void OnStart()
		{
			var scene = new ace.Scene();
			var layer = new ace.Layer2D();
			var effect = ace.Engine.Graphics.CreateEffect("Data/Effect/Laser01.efk");
			m_effectObj = new ace.EffectObject2D();

			layer.AddObject(m_effectObj);
			scene.AddLayer(layer);
			ace.Engine.ChangeScene(scene);

			m_effectObj.Position = new ace.Vector2DF(320, 240);
			m_effectObj.Scale = new ace.Vector2DF(20, 20);
			m_effectObj.EffectRotation = 90;

			m_effectObj.SetEffect(effect);
			m_effectObj.Play();
		}

		protected override void OnUpdating()
		{
			base.OnUpdating();
		}
	}
}
