using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public abstract class Layer2DComponent
	{
		public Layer2D Owner { get; internal set; }

		protected abstract void OnUpdated();

		internal void Update()
		{
			OnUpdated();
		}
	}
}
