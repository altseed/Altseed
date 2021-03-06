﻿#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "asd.Core.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "PCH/asd.Core.PCH.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Core_Imp;
	
	class Window_Imp;
	class Mouse_Imp;
	class Keyboard_Imp;
	class Log_Imp;
	class Profiler_Imp;
	class LayerProfiler_Imp;
	class ProfilerViewer_Imp;

	class Joystick_Imp;
	class JoystickContainer_Imp;

	class File_Imp;
	class Path_Imp;
	class StaticFile_Imp;
	class StreamFile_Imp;

	class RenderingThreadEvent;
	class RenderingThread;

	class Graphics_Imp;
	class Texture2D_Imp;
	class VertexBuffer_Imp;
	class IndexBuffer_Imp;
	class NativeShader_Imp;
	class RenderState_Imp;
	class RenderTexture2D_Imp;
	class CubemapTexture_Imp;
	class DepthBuffer_Imp;
	class Shader2D_Imp;
	class Shader3D_Imp;
	class Effect_Imp;
	class ShaderCache;

	class ImagePackage_Imp;

	class MaterialCommand;
	class Material2DCommand;
	class Material3DCommand;

	class Material2D_Imp;
	class Material3D_Imp;
	class MaterialPropertyBlock_Imp;

	class PostEffectRenderer;
	class Renderer2D_Imp;
	class LayerRenderer;

	class GraphicsResourceContainer;

	class ObjectSystemFactory_Imp;

	class Renderer3D;
	class Renderer3DProxy;
	class SpriteRenderer3D;

	class RenderedObject3D;
	class RenderedEffectObject3D;
	class RenderedModelObject3D;
	class RenderedDirectionalLightObject3D;
	class RenderedCameraObject3D;
	class RenderedMassModelObject3D;
	class RenderedTerrainObject3D;

	class RenderedObject3DProxy;
	class RenderedCameraObject3DProxy;
	class RenderedDirectionalLightObject3DProxy;
	class RenderedMassModelObject3DProxy;
	class RenderedTerrainObject3DProxy;

	class Model_Imp;
	class MassModel_Imp;

	class Mesh_Imp;
	class Deformer_Imp;

	class Font_Imp;

	class AnimationSystem_Imp;

	class RenderingCommand;
	class RenderingCommandExecutor;
	class RenderingCommandFactory;
	class RenderingCommandHelper;

	struct RenderState
	{
		bool								DepthTest;
		bool								DepthWrite;
		AlphaBlendMode						AlphaBlendState;
		CullingType							Culling;
		
		void Reset()
		{
			DepthTest = false;
			DepthWrite = false;
			AlphaBlendState = AlphaBlendMode::Blend;
			Culling = CullingType::Double;
		}

		RenderState()
		{
			Reset();
		}
	};

	enum class VertexLayoutFormat : int32_t
	{
		R32G32B32_FLOAT,
		R8G8B8A8_UNORM,
		R8G8B8A8_UINT,
		R32G32_FLOAT,
	};

	enum class ConstantBufferFormat : int32_t
	{
		Float1,
		Float2,
		Float3,
		Float4,
		Float4_ARRAY,
		Matrix44,
		Matrix44_ARRAY,
		Unknown,
	};

	struct VertexLayout
	{
		std::string			Name;
		VertexLayoutFormat LayoutFormat;

		VertexLayout()
		{
			Name = "";
			LayoutFormat = VertexLayoutFormat::R32G32B32_FLOAT;
		}

		VertexLayout(const char* name, VertexLayoutFormat format)
		{
			Name = name;
			LayoutFormat = format;
		}
	};

	struct Macro
	{
		const char* Name;
		const char* Definition;

		Macro()
			: Name(nullptr)
			, Definition(nullptr)
		{
		}

		Macro(const char* name, const char* definition)
			: Name(name)
			, Definition(definition)
		{
		}
	};

	/**
		@brief	シェーダー内の定数1つを保存する構造体
	*/
	struct ShaderConstantValue
	{
		ShaderVariableType	ValueType;
		int32_t				ID;
		union
		{
			float		Float4[4];
			float		Mat44[16];

			struct
			{
				Texture2D*			Ptr;
				TextureFilterType	FilterType;
				TextureWrapType		WrapType;
			} Texture2DPtr;

			struct
			{
				CubemapTexture*		Ptr;
				TextureFilterType	FilterType;
				TextureWrapType		WrapType;
			} CubemapTexturePtr;

			struct
			{
				Vector4DF*			Ptr;
				int32_t				Count;
			} Vector4DFArray;

			/**
				@brief	行列の配列
				@note
				配列の内容自体は保存しないので別領域に確保する必要がある。
				*/
			struct
			{
				Matrix44*			Ptr;
				int32_t				Count;
			} Mat44Array;

		} Data;

		ShaderConstantValue();
		ShaderConstantValue(const ShaderConstantValue& value);
		ShaderConstantValue(const float& value);
		ShaderConstantValue(const Vector2DF& value);
		ShaderConstantValue(const Vector3DF& value);
		ShaderConstantValue(const Vector4DF& value);
		ShaderConstantValue(Vector4DF* value, int32_t count);
		ShaderConstantValue(const Matrix44& value);
		ShaderConstantValue(Matrix44* value, int32_t count);
		ShaderConstantValue(Texture2D* value, TextureFilterType filterType, TextureWrapType wrapType);
		ShaderConstantValue(CubemapTexture* value, TextureFilterType filterType, TextureWrapType wrapType);

		virtual ~ShaderConstantValue();
		ShaderConstantValue& operator=(const ShaderConstantValue& value);
	};

	/**
	@brief	リロード情報
	*/
	struct ModelReloadInformation
	{
		time_t ModifiedTime;
		astring	Path;
	};

	struct FontReloadInformation
	{
		time_t ModifiedTime;
		astring Path;
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	extern "C"
	{
		struct Vector2DF_R
		{
			float	X;
			float	Y;
		};

		struct Vector2DI_R
		{
			int32_t	X;
			int32_t	Y;
		};

		struct Vector3DF_R
		{
			float	X;
			float	Y;
			float	Z;
		};

		struct Vector4DF_R
		{
			float	X;
			float	Y;
			float	Z;
			float	W;
		};
		struct Color_R
		{
			uint8_t	R;
			uint8_t	G;
			uint8_t	B;
			uint8_t	A;
		};

		struct Matrix33_R
		{
			float Values[3][3];
		};

		struct Matrix44_R
		{
			float	Values[4 * 4];
		};

		struct FCurveKeyframe_R
		{
			Vector2DF_R	KeyValue;
			Vector2DF_R	LeftHandle;
			Vector2DF_R	RightHandle;
		};

		struct RectI_R
		{
			int X;
			int Y;
			int Width;
			int Height;
		};

		struct RectF_R
		{
			float X;
			float Y;
			float Width;
			float Height;
		};

		struct TextureLockInfomation_R
		{
			void* Pixels;
			int Pitch;
			Vector2DI_R Size;
		};

	}
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};
