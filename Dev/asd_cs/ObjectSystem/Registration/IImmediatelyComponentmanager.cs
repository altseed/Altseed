using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd.ObjectSystem.Registration
{
	internal interface IImmediatelyComponentmanager<TComponent>
		where TComponent : Component
	{
		void ImmediatelyAddComponent(TComponent component, string key);
		void ImmediatelyRemoveComponent(string key);
	}
}
