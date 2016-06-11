using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal interface IImmediateRemovingComponentmanager
	{
		void ImmediatelyRemoveComponent(string key);
	}

	internal interface IImmediateComponentmanager<TComponent> : IImmediateRemovingComponentmanager
		where TComponent : Component
	{
		void ImmediatelyAddComponent(TComponent component, string key);
	}
}
