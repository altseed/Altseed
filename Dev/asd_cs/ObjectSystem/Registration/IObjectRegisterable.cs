using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal interface IObjectRegisterable<TObject>
	{
		bool IsAlive { get; }
		void __Register(TObject obj);
		void __Unregister(TObject obj);
		void __AddToCore(TObject obj);
		void __RemoveFromCore(TObject obj);
	}
}
