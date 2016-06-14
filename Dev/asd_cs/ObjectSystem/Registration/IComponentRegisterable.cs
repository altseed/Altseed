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
		void __Register(TComponent component);
		void __Unregister(TComponent component);
	}
}
