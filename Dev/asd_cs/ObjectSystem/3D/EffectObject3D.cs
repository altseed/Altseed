using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// エフェクトの機能を提供するクラス
	/// </summary>
	public class EffectObject3D : Object3D
	{
		swig.CoreEffectObject3D coreObject = null;

		public EffectObject3D()
		{
			coreObject = Engine.ObjectSystemFactory.CreateEffectObject3D();
			GC.Object3Ds.AddObject(coreObject.GetPtr(), this);
			commonObject = coreObject;
		}

		public override void Destroy()
		{
			coreObject = null;
			base.Destroy();
		}

		protected override void OnStart() { }
		protected override void OnUpdate() { }

		protected override void OnUpdateInternal()
		{
		}

		/// <summary>
		/// 描画に使用するエフェクトを設定する。
		/// </summary>
		/// <param name="effect">エフェクト</param>
		public void SetEffect(Effect effect)
		{
			coreObject.SetEffect(IG.GetEffect(effect));
		}

		/// <summary>
		/// 設定されている全てのエフェクトを再生する。
		/// </summary>
		public void Play()
		{
			coreObject.Play();
		}

		/// <summary>
		/// このオブジェクトから再生されたエフェクトを全て停止する。
		/// </summary>
		public void Stop()
		{
			coreObject.Stop();
		}

		/// <summary>
		/// このオブジェクトから再生されたエフェクトのルートを全て停止する。
		/// </summary>
		public void StopRoot()
		{
			coreObject.StopRoot();
		}

		/// <summary>
		/// このオブジェクトから再生されたエフェクトが再生中か取得する。
		/// </summary>
		/// <returns>再生中か?</returns>
		public bool IsPlaying
		{
			get
			{
				return coreObject.GetIsPlaying();
			}
		}

		/// <summary>
		/// このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか取得、または設定する。
		/// </summary>
		public bool SyncEffects
		{
			get
			{
				return coreObject.GetSyncEffects();
			}
			set
			{
				coreObject.SetSyncEffects(value);
			}
		}
	}
	
}
