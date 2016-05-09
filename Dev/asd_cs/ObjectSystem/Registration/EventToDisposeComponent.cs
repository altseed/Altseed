using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	class EventToDisposeComponent : ICommitable
	{
		public EventToDisposeComponent(asd.Component component)
		{
			Component = component;
		}

		private asd.Component Component { get; set; }

		public void Commit()
		{
			Component.ImmediatelyDispose();
		}
	}
}
