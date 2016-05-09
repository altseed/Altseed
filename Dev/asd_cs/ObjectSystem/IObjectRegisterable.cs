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
		void Register(TObject obj);
		void Unregister(TObject obj);
	}
}
