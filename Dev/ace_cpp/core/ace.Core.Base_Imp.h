#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Core.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "PCH/ace.Core.PCH.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Core_Imp;
	
	class Window_Imp;
	class Mouse_Imp;
	class Keyboard_Imp;
	class Log_Imp;
	class Profiler_Imp;
	class ProfilerViewer_Imp;

	class Joystick_Imp;
	class JoystickContainer_Imp;

	class File_Imp;
	class Path_Imp;
	class StaticFile_Imp;
	class StreamFile_Imp;

	class Sound_Imp;
	class SoundSource_Imp;

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

	class ShaderCache;

	class MaterialCommand;
	class Material2DCommand;
	class Material2D_Imp;

	class PostEffectRenderer;
	class Renderer2D_Imp;
	class LayerRenderer;

	class GraphicsResourceContainer;

	class ObjectSystemFactory_Imp;

	class Renderer3D;
	class RenderedObject3D;
	class RenderedEffectObject3D;
	class RenderedModelObject3D;
	class RenderedDirectionalLightObject3D;
	class RenderedCameraObject3D;

	class Model_Imp;

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
		AlphaBlend							AlphaBlendState;
		eCullingType						CullingType;
		
		void Reset()
		{
			DepthTest = false;
			DepthWrite = false;
			AlphaBlendState = AlphaBlend::Blend;
			CullingType = CULLING_DOUBLE;
		}

		RenderState()
		{
			Reset();
		}

		void CopyTo(RenderState& state)
		{
			state.DepthTest = DepthTest;
			state.DepthWrite = DepthWrite;
			state.AlphaBlendState = AlphaBlendState;
			state.CullingType = CullingType;
		}
	};

	enum eVertexLayoutFormat
	{
		LAYOUT_FORMAT_R32G32B32_FLOAT,
		LAYOUT_FORMAT_R8G8B8A8_UNORM,
		LAYOUT_FORMAT_R8G8B8A8_UINT,
		LAYOUT_FORMAT_R32G32_FLOAT,
	};

	enum eConstantBufferFormat
	{
		CONSTANT_BUFFER_FORMAT_FLOAT1,
		CONSTANT_BUFFER_FORMAT_FLOAT2,
		CONSTANT_BUFFER_FORMAT_FLOAT3,
		CONSTANT_BUFFER_FORMAT_FLOAT4,
		CONSTANT_BUFFER_FORMAT_MATRIX44,
		CONSTANT_BUFFER_FORMAT_MATRIX44_ARRAY,
		CONSTANT_BUFFER_FORMAT_UNKNOWN,
	};

	struct VertexLayout
	{
		std::string			Name;
		eVertexLayoutFormat LayoutFormat;

		VertexLayout()
		{
			Name = "";
			LayoutFormat = LAYOUT_FORMAT_R32G32B32_FLOAT;
		}

		VertexLayout(const char* name, eVertexLayoutFormat format)
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
	@brief	シェーダーの定数バッファ向け情報
	*/
	struct ConstantBufferInformation
	{
		std::string		Name;
		int32_t			Offset;
		eConstantBufferFormat	Format;
		
		/**
			@brief	定数バッファが配列だった際の配列の個数
		*/
		int32_t			Count;

		ConstantBufferInformation()
			: Offset(0)
			, Format(CONSTANT_BUFFER_FORMAT_FLOAT4)
			, Count(0)
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

		} Data;

		ShaderConstantValue();
		ShaderConstantValue(const ShaderConstantValue& value);
		ShaderConstantValue(const float& value);
		ShaderConstantValue(const Vector2DF& value);
		ShaderConstantValue(const Vector3DF& value);
		ShaderConstantValue(const Vector4DF& value);
		ShaderConstantValue(const Matrix44& value);
		ShaderConstantValue(Texture2D* value, TextureFilterType filterType, TextureWrapType wrapType);
		virtual ~ShaderConstantValue();
		ShaderConstantValue& operator=(const ShaderConstantValue& value);
	};

	/**
	@brief	リロード情報
	*/
	struct Texture2DReloadInformation
	{
		time_t ModifiedTime;
		astring	Path;
	};

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
