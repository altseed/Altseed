using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal interface IBeingAbleToDisposeNative
	{
		void Dispose(bool disposeNative);
		void DisposeImmediately(bool disposeNative);
	}
}
