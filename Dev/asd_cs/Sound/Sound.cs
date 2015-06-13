using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// 音を管理するクラス
	/// </summary>
	public partial class Sound
	{
		internal Sound(swig.Sound sound)
		{
			CoreInstance = sound;
		}

		/// <summary>
		/// 音を読み込む。
		/// </summary>
		/// <param name="path">パス</param>
		/// <param name="isDecompressed">解凍するか?</param>
		/// <returns>音源</returns>
		public SoundSource CreateSoundSource(string path, bool isDecompressed)
		{
			return GC.GenerateSoundSource(swig.Accessor.Sound_CreateSoundSource(CoreInstance, path, isDecompressed), GC.GenerationType.Create);
		}

		/// <summary>
		/// 音が再生中か、取得する。
		/// </summary>
		/// <param name="id">ID</param>
		/// <returns>再生中か?</returns>
		public bool IsPlaying(int id)
		{
			bool ret = CoreInstance.GetIsPlaying(id);
			return ret;
		}
	}
}
