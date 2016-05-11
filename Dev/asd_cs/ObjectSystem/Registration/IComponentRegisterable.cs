using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal interface IComponentRegisterable<TComponent>
		where TComponent : Component
	{
		void Register(TComponent component);
		void Unregister(TComponent component);
	}
}
