using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ace
{
	public abstract class Object2DComponent
	{
		public Object2D Owner { get; internal set; }

		protected abstract void OnUpdate();

		internal void Update()
		{
			OnUpdate();
		}
	}
}
