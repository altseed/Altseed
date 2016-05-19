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
			if(GC.Object2Ds.GetObject(p) != null)
			{
				Particular.Helper.ThrowException("");
			}

			GC.Object2Ds.AddObject(p, this);
		}

		/// <summary>
		/// 描画に使用するエフェクトを取得、または設定する。
		/// </summary>
		public Effect Effect
		{
			get
			{
				ThrowIfReleased();
				return GC.GenerateEffect(swig.Accessor.CoreEffectObject2D_GetEffect(coreEffectObject), GenerationType.Get);
			}
			set
			{
				ThrowIfReleased();
				coreEffectObject.SetEffect(IG.GetEffect(value));
			}

		}

		/// <summary>
		/// 設定されている全てのエフェクトを再生する。
		/// </summary>
		/// <returns>再生されたエフェクトのID</returns>
		public int Play()
		{
			ThrowIfReleased();
			if(Layer == null)
			{
				Engine.Logger.WriteLine("エフェクトをレイヤーに追加する前に再生しました。正しく表示されない場合があります。", LogLevel.Warning);
			}

			return coreEffectObject.Play();
		}

		/// <summary>
		/// このオブジェクトから再生されたエフェクトを全て停止する。
		/// </summary>
		public void Stop()
		{
			ThrowIfReleased();
			coreEffectObject.Stop();
		}

		/// <summary>
		/// このオブジェクトから再生されたエフェクトのルートを全て停止する。
		/// </summary>
		public void StopRoot()
		{
			ThrowIfReleased();
			coreEffectObject.StopRoot();
		}

		/// <summary>
		/// このオブジェクトから再生されたエフェクトを表示状態にする。
		/// </summary>
		public void Show()
		{
			ThrowIfReleased();
			coreEffectObject.Show();
		}

		/// <summary>
		/// このオブジェクトから再生されたエフェクトを非表示状態にする。
		/// </summary>
		public void Hide()
		{
			ThrowIfReleased();
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
				ThrowIfReleased();
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
				ThrowIfReleased();
				return coreEffectObject.GetSyncEffects();
			}
			set
			{
				ThrowIfReleased();
				coreEffectObject.SetSyncEffects(value);
			}
		}


		/// <summary>
		/// このオブジェクトから再生されるエフェクトの角度(X軸)を指定する。
		/// </summary>
		/// <remarks>
		/// エフェクトの形状が画面に垂直な方向に長い場合、2D表示では予期せぬ見た目で表示されることがある。
		/// この時、X軸を中心にエフェクトを回転することで、違和感のない表示をできるようにする。
		/// </remarks>
		public float EffectRotationX
		{
			get
			{
				ThrowIfReleased();
				return coreEffectObject.GetEffectRotationX();
			}
			set
			{
				ThrowIfReleased();
				coreEffectObject.SetEffectRotationX(value);
			}
		}


		/// <summary>
		/// このオブジェクトから再生されるエフェクトの角度(Y軸)を指定する。
		/// </summary>
		/// <remarks>
		/// エフェクトの形状が画面に垂直な方向に長い場合、2D表示では予期せぬ見た目で表示されることがある。
		/// この時、Y軸を中心にエフェクトを回転することで、違和感のない表示をできるようにする。
		/// </remarks>
		public float EffectRotationY
		{
			get
			{
				ThrowIfReleased();
				return coreEffectObject.GetEffectRotationY();
			}
			set
			{
				ThrowIfReleased();
				coreEffectObject.SetEffectRotationY(value);
			}
		}

		/// <summary>
		/// このオブジェクトから再生されるエフェクトの向き(度)を指定する。
		/// </summary>
		/// <remarks>
		/// 2D表示だとエフェクトが画面に対して前後に表示されるように作成されていた場合、予期せぬ見た目で表示されてしまうことがある。
		/// そのため、Y軸を中心にエフェクトを回転させ2Dとして違和感のない表示ができるようにする。
		/// 互換性のために残されている。
		/// </remarks>
		public float EffectRotation
		{
			get
			{
				ThrowIfReleased();
				return coreEffectObject.GetEffectRotation();
			}
			set
			{
				ThrowIfReleased();
				coreEffectObject.SetEffectRotation(value);
			}
		}

		/// <summary>
		/// この2Dオブジェクトを描画する際の描画優先度を取得または設定する。描画優先度が高いほど手前に描画される。
		/// </summary>
		public int DrawingPriority
		{
			get
			{
				ThrowIfReleased();
				return coreEffectObject.GetDrawingPriority();
			}
			set
			{
				ThrowIfReleased();
				coreEffectObject.SetDrawingPriority(value);
			}
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
			lock (this)
			{
				if(coreEffectObject == null) return;
				GC.Collector.AddObject(coreEffectObject);
				coreEffectObject = null;
			}
			Particular.GC.SuppressFinalize(this);
		}
		#endregion
	}
}
