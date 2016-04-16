using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// アニメーションに関するインスタンスを生成するためのクラス
	/// </summary>
	public class AnimationSystem
	{
		swig.AnimationSystem animationSyatem = null;

		internal AnimationSystem(swig.AnimationSystem animationSyatem)
		{
			this.animationSyatem = animationSyatem;
		}

		/// <summary>
		/// アニメーションクリップのインスタンスを生成する。
		/// </summary>
		/// <returns>インスタンス</returns>
		public AnimationClip CreateAnimationClip()
		{
			return GC.GenerateAnimationClip(swig.Accessor.AnimationSyatem_CreateAnimationClip(animationSyatem), GenerationType.Create);
		}

		/// <summary>
		/// アニメーションソースのインスタンスを生成する。
		/// </summary>
		/// <returns>インスタンス</returns>
		public AnimationSource CreateAnimationSource()
		{
			return GC.GenerateAnimationSource(swig.Accessor.AnimationSyatem_CreateAnimationSource(animationSyatem), GenerationType.Create);
		}

		/// <summary>
		/// キーフレームアニメーションのインスタンスを生成する。
		/// </summary>
		/// <returns>インスタンス</returns>
		public KeyframeAnimation CreateKeyframeAnimation()
		{
			return GC.GenerateKeyframeAnimation(swig.Accessor.AnimationSyatem_CreateKeyframeAnimation(animationSyatem), GenerationType.Create);
		}
	}
}
