
#include "FBX2MDL.Base.h"

namespace FBX2MDL
{

	class FBXExporter
	{
	private:
		std::shared_ptr<ace::BinaryWriter> m_writer;

		void WriteHeader();
		void WriteAnimationSource(AnimationSource& source);
		void WriteAnimationSources(std::vector<AnimationSource>& animationSources);

	public:
		FBXExporter();
		virtual ~FBXExporter();

		/**
			@brief	シーンをバイナリとして出力する・
			@param	scene	シーン
			@return	バイナリ
			@note
			この関数はsceneには破壊的変更を加える。
		*/
		std::shared_ptr<ace::BinaryWriter> Export(std::shared_ptr<Scene> scene);
	};
}