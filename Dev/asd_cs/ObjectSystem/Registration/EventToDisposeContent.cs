using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	class EventToDisposeContent : ICommitable
	{
		public EventToDisposeContent(IBeingAbleToDisposeNative disposable, bool disposeNative)
		{
			Disposable = disposable;
			DisposeNative = disposeNative;
		}

		private IBeingAbleToDisposeNative Disposable { get; set; }
		private bool DisposeNative { get; set; }

		public void Commit()
		{
			Disposable.DisposeImmediately(DisposeNative);
		}
	}
}
