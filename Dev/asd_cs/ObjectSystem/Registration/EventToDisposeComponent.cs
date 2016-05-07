using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd.ObjectSystem.Registration
{
	class EventToDisposeComponent<TOwner> : ICommitable
		where TOwner : class
	{
		public EventToDisposeComponent(Component<TOwner> component)
		{
			Component = component;
		}

		private Component<TOwner> Component { get; set; }

		public void Commit()
		{
			Component.ImmediatelyDispose();
		}
	}
}
