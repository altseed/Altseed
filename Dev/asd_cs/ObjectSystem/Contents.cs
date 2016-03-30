using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	public abstract class Content
	{
		private int updatePriority_;

		/// <summary>
		/// このインスタンスの更新の優先順位を取得または設定する。
		/// </summary>
		public int UpdatePriority
		{
			get { return updatePriority_; }
			set
			{
				updatePriority_ = value;
				if(OnUpdatePriorityChanged != null)
				{
					OnUpdatePriorityChanged(this);
				}
			}
		}

		internal event Action<Content> OnUpdatePriorityChanged;

		internal abstract bool GetIsAlive();
		internal abstract void Update();
		public abstract void Dispose(bool disposeNative);
	}
}
