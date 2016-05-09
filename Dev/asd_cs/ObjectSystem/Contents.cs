using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal interface IActionWithObject
	{
		void Invoke(AltseedObject obj);
	}

	public abstract class AltseedObject
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
					OnUpdatePriorityChanged.Invoke(this);
				}
			}
		}

		internal abstract bool IsRegisteredToLayer { get; }

		internal IActionWithObject OnUpdatePriorityChanged;

		internal abstract bool GetIsAlive();
		internal abstract void Update();
		internal abstract void RaiseOnAdded();
		internal abstract void RaiseOnRemoved();
		public abstract void Dispose(bool disposeNative);
	}
}
