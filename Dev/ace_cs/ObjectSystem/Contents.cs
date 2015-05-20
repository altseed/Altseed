using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public abstract class Content
	{
		internal abstract bool GetIsAlive();
		internal abstract void Update();
	}
}
