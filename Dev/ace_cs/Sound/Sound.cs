using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 音を管理するクラス
	/// </summary>
	public class Sound
	{
		internal swig.Sound SwigObject = null;

		internal Sound(swig.Sound sound)
		{
			SwigObject = sound;
		}

		/// <summary>
		/// 音を読み込む。
		/// </summary>
		/// <param name="path">パス</param>
		/// <param name="isDecompressed">解凍するか?</param>
		/// <returns>音源</returns>
		public SoundSource CreateSoundSource(string path, bool isDecompressed)
		{
			return GC.GenerateSoundSource(swig.Accessor.Sound_CreateSoundSource(SwigObject, path, isDecompressed), GC.GenerationType.Create);
		}

		/// <summary>
		/// 音を再生する。
		/// </summary>
		/// <param name="soundSource">音源</param>
		/// <returns>ID</returns>
		public int Play(SoundSource soundSource)
		{
			int ret = SwigObject.Play(soundSource.SwigObject);
			return ret;
		}

		/// <summary>
		/// 音が再生中か、取得する。
		/// </summary>
		/// <param name="id">ID</param>
		/// <returns>再生中か?</returns>
		public bool IsPlaying(int id)
		{
			bool ret = SwigObject.GetIsPlaying(id);
			return ret;
		}

		/// <summary>
		/// 全ての再生中の音を停止する。
		/// </summary>
		public void StopAll()
		{
			SwigObject.StopAll();
		}

		/// <summary>
		/// 指定した音を停止する。
		/// </summary>
		/// <param name="id"ID></param>
		public void Stop(int id)
		{
			SwigObject.Stop(id);
		}

		/// <summary>
		/// 指定した音を一時停止する。
		/// </summary>
		/// <param name="id">ID</param>
		public void Pause(int id)
		{
			SwigObject.Pause(id);
		}

		/// <summary>
		/// 指定した一時停止中の音の一時停止を解除する。
		/// </summary>
		/// <param name="id">ID</param>
		public void Resume(int id)
		{
			SwigObject.Resume(id);
		}

		/// <summary>
		/// 指定した音の音量を設定する。
		/// </summary>
		/// <param name="id">ID</param>
		/// <param name="volume">音量(0.0～1.0)</param>
		public void SetVolume(int id, float volume)
		{
			SwigObject.SetVolume(id, volume);
		}

		/// <summary>
		/// 指定した音をフェードインさせる。
		/// </summary>
		/// <param name="id">ID</param>
		/// <param name="second">フェードインに使用する時間(秒)</param>
		public void FadeIn(int id, float second)
		{
			SwigObject.FadeIn(id, second);
		}

		/// <summary>
		/// 指定した音をフェードアウトさせる。
		/// </summary>
		/// <param name="id">ID</param>
		/// <param name="second">フェードアウトに使用する時間(秒)</param>
		public void FadeOut(int id, float second)
		{
			SwigObject.FadeOut(id, second);
		}
	}
}
