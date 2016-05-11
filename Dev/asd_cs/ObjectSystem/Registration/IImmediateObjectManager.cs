using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal interface IImmediateObjectManager<TObject>
		where TObject : AltseedObject
	{
		void ImmediatelyAddObject(TObject obj, bool raiseEvent);
		void ImmediatelyRemoveObject(TObject obj, bool raiseEvent);
	}
}
