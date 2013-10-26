using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ace
{
	public abstract class Component
	{
		protected abstract void OnUpdate();

		internal void Update()
		{
			OnUpdate();
		}
	}
}
