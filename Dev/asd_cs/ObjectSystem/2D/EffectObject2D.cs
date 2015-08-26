using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	public class EffectObject2D : Object2D, IReleasable
	{
		
		internal override swig.CoreObject2D CoreObject
		{
			get { return coreEffectObject; }
		}
		private swig.CoreEffectObject2D coreEffectObject { get; set; }

		public EffectObject2D()
		{
			coreEffectObject = Engine.ObjectSystemFactory.CreateEffectObject2D();

			var p = coreEffectObject.GetPtr();
			if( GC.Object2Ds.GetObject( p ) != null )
			{
				Particular.Helper.ThrowException("");
			}

			GC.Object2Ds.AddObject( p, this );
		}

		/// <summary>
		/// 描画に使用するエフェクトを取得、または設定する。
		/// </summary>
		public Effect Effect
		{
			get
			{
				return GC.GenerateEffect(swig.Accessor.CoreEffectObject2D_GetEffect(coreEffectObject), GC.GenerationType.Get);
			}
			set
			{
				coreEffectObject.SetEffect(IG.GetEffect(value));
			}
			
		}

		/// <summary>
		/// 設定されている全てのエフェクトを再生する。
		/// </summary>
		/// <returns>再生されたエフェクトのID</returns>
		public int Play()
		{
			return coreEffectObject.Play();
		}

		/// <summary>
		/// このオブジェクトから再生されたエフェクトを全て停止する。
		/// </summary>
		public void Stop()
		{
			coreEffectObject.Stop();
		}

		/// <summary>
		/// このオブジェクトから再生されたエフェクトのルートを全て停止する。
		/// </summary>
		public void StopRoot()
		{
			coreEffectObject.StopRoot();
		}

		/// <summary>
		/// このオブジェクトから再生されたエフェクトを表示状態にする。
		/// </summary>
		public void Show()
		{
			coreEffectObject.Show();
		}

		/// <summary>
		/// このオブジェクトから再生されたエフェクトを非表示状態にする。
		/// </summary>
		public void Hide()
		{
			coreEffectObject.Hide();
		}

		/// <summary>
		/// このオブジェクトから再生されたエフェクトが再生中か取得する。
		/// </summary>
		/// <returns>再生中か?</returns>
		public bool IsPlaying
		{
			get
			{
				return coreEffectObject.GetIsPlaying();
			}
		}

		/// <summary>
		/// このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか取得、または設定する。
		/// </summary>
		public bool SyncEffects
		{
			get
			{
				return coreEffectObject.GetSyncEffects();
			}
			set
			{
				coreEffectObject.SetSyncEffects(value);
			}
		}

		/// <summary>
		/// このオブジェクトから再生されるエフェクトの向き(度)を指定する。
		/// </summary>
		/// <remarks>
		/// 2D表示だとエフェクトが画面に対して前後に表示されるように作成されていた場合、予期せぬ見た目で表示されてしまうことがある。
		/// そのため、Y軸を中心にエフェクトを回転させ2Dとして違和感のない表示ができるようにする。
		/// </remarks>
		public float EffectRotation
		{
			get
			{
				return coreEffectObject.GetEffectRotation();
			}
			set
			{
				coreEffectObject.SetEffectRotation(value);
			}
		}

		/// <summary>
		/// この2Dオブジェクトを描画する際の描画優先度を取得または設定する。描画優先度が高いほど手前に描画される。
		/// </summary>
		public int DrawingPriority
		{
			get { return coreEffectObject.GetDrawingPriority(); }
			set { coreEffectObject.SetDrawingPriority(value); }
		}

#region GC対策
		~EffectObject2D()
		{
			ForceToRelease();
		}

		public override bool IsReleased
		{
			get { return coreEffectObject == null; }
		}

		/// <summary>
		/// 強制的に使用しているメモリを開放する。
		/// </summary>
		/// <remarks>
		/// 何らかの理由でメモリが不足した場合に実行する。
		/// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
		/// </remarks>
		public override void ForceToRelease()
		{
			lock( this )
			{
				if( coreEffectObject == null ) return;
				GC.Collector.AddObject( coreEffectObject );
				coreEffectObject = null;
			}
			Particular.GC.SuppressFinalize(this);
		}
#endregion
	}
}
