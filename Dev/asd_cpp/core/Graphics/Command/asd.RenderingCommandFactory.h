
#pragma once

#include <asd.common.Base.h>
#include <thread>
#include <mutex>

#include "../../asd.Core.Base.h"
#include "../../asd.Core.Base_Imp.h"

namespace asd
{
	class RenderingCommandFactory
	{
	private:
		static const int32_t	MainBufferSize = 1024 * 1024;
		static const int32_t	SubBufferSize = 1024 * 1024;

		std::vector<uint8_t>	buffer;
		std::list<uint8_t*>		subBuffers;

		int32_t					offset = 0;
		int32_t					subBufferOffset = 0;

		std::recursive_mutex		mtx_;
	public:

		RenderingCommandFactory();
		virtual ~RenderingCommandFactory();

		/**
			@brief	プレイスメントニューにより高速にバッファを確保する。
		*/
		template<typename T, class... Args>
		T* CreateCommand(Args... args)
		{
			std::lock_guard<std::recursive_mutex> lock(mtx_);
			auto size = sizeof(T);
			uint8_t* b = nullptr;

			if (buffer.size() > offset + size)
			{
				// 通常モード
				b = &(buffer[offset]);
				offset += size;
			}
			else
			{
				// サブバッファーモード
				if (subBuffers.size() == 0 || subBufferOffset + size > SubBufferSize)
				{
					subBuffers.push_back(new uint8_t[SubBufferSize]);
					subBufferOffset = 0;
				}
				uint8_t* subBuf = subBuffers.back();

				b = &(subBuf[subBufferOffset]);
				subBufferOffset += size;
			}
			return new(b) T(args...);
		}

		/**
		@brief	プレイスメントニューにより高速にバッファを確保する。
		*/
		template<typename T>
		T* CreateCommands(int32_t count)
		{
			std::lock_guard<std::recursive_mutex> lock(mtx_);
			auto size = sizeof(T) * count;
			uint8_t* b = nullptr;

			if (buffer.size() > offset + size)
			{
				// 通常モード
				b = &(buffer[offset]);
				offset += size;
			}
			else
			{
				// サブバッファーモード
				if (subBuffers.size() == 0 || subBufferOffset + size > SubBufferSize)
				{
					subBuffers.push_back(new uint8_t[SubBufferSize]);
					subBufferOffset = 0;
				}
				
				uint8_t* subBuf = subBuffers.back();

				b = &(subBuf[subBufferOffset]);
				subBufferOffset += size;
			}
			return new(b) T[count];
		}

		/**
			@brief	バッファを全て回収しリセットする。
		*/
		void Reset();

		void Clear(std::vector<RenderingCommand*>& commands, bool isColorTarget, bool isDepthTarget, const Color& color);
		void SetRenderTarget(std::vector<RenderingCommand*>& commands, RenderTexture2D* renderTexture0, RenderTexture2D* renderTexture1, RenderTexture2D* renderTexture2, RenderTexture2D* renderTexture3, DepthBuffer_Imp* depth);
		void SetRenderTarget(std::vector<RenderingCommand*>& commands, RenderTexture2D* renderTexture0, DepthBuffer_Imp* depth);
		void Draw(std::vector<RenderingCommand*>& commands, int32_t polyCount, VertexBuffer_Imp* vb, IndexBuffer_Imp* ib, NativeShader_Imp* shader, RenderState rs, std::vector<ShaderConstantValue>& constantValues);
		void DrawEffect(std::vector<RenderingCommand*>& commands, Matrix44 projMat, Matrix44 cameraMat);
		void DrawSprite(std::vector<RenderingCommand*>& commands, Matrix44 projMat, Matrix44 cameraMat);
	};
}