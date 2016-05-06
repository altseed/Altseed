using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal interface IActionWithObject<TLayer>
		where TLayer : Layer
	{
		void Invoke(AltseedObject<TLayer> obj);
	}

	public abstract class AltseedObject<TLayer>
		where TLayer : Layer
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

		public TLayer Layer { get; set; }

		internal IActionWithObject<TLayer> OnUpdatePriorityChanged;

		internal abstract bool GetIsAlive();
		internal abstract void Update();
		internal abstract void RaiseOnAdded();
		internal abstract void RaiseOnRemoved();
		public abstract void Dispose(bool disposeNative);
	}
}
