#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Core;
	class Graphics;
	class Window;
	class Keyboard;
	class Mouse;
	class Log;
	class Profiler;
	class ProfilerViewer;
	class Renderer2D;

	class Joystick;
	class JoystickContainer;
	
	class File;
	class Path;
	class StaticFile;
	class StreamFile;

	class Sound;
	class SoundSource;

	class Texture;
	class Texture2D;
	class RenderTexture2D;
	class CubemapTexture;

	class Shader2D;
	class Material2D;
	class Shader3D;
	class Material3D;
	class MaterialPropertyBlock;

	class Effect;

	class MassModel;
	class Model;
	class Terrain3D;

	class Font;

	class Chip2D;

	class Mesh;
	class Deformer;

	class AnimationSystem;
	class AnimationClip;
	class AnimationSource;
	class KeyframeAnimation;


	class ObjectSystemFactory;
	class CoreScene;
	class CoreLayer;

	class CoreLayer2D;
	class CoreObject2D;
	class CoreTextureObject2D;
	class CoreCameraObject2D;
	class CoreTextObject2D;
	class CoreEffectObject2D;
	class CoreMapObject2D;

	class CoreLayer3D;
	class CoreObject3D;
	class CoreModelObject3D;
	class CoreCameraObject3D;
	class CoreEffectObject3D;
	class CoreDirectionalLightObject3D;
	class CoreMassModelObject3D;
	class CoreTerrainObject3D;

	class CorePostEffect;
	class CoreTransition;

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	enum class WritingDirection :int32_t
	{
		Vertical,
		Horizontal,
	};

	/**
		@brief	フレームレートの制御方法
	*/
	enum class FramerateMode : int32_t
	{
		/**
			@brief	固定フレームレート
		*/
		Constant,

		/**
			@brief	可変フレームレート
		*/
		Variable,
	};

	enum class TextureFormat : int32_t
	{
		R8G8B8A8_UNORM = 0,
		R32G32B32A32_FLOAT = 1,
		R8G8B8A8_UNORM_SRGB = 2,
		R16G16_FLOAT = 3,
		R8_UNORM = 4,
	};

	enum eTextureClassType
	{
		TEXTURE_CLASS_TEXTURE2D = 0,
		TEXTURE_CLASS_RENDERTEXTURE2D = 1,
		TEXTURE_CLASS_CUBEMAPTEXTURE = 2,
		TEXTURE_CLASS_DEPTHBUFFER = 3,

	};

	/**
		@brief	描画時のブレンドモードを表す列挙体
	*/
	enum class AlphaBlend : int32_t
	{
		/// <summary>
		/// 不透明
		/// </summary>
		Opacity = 0,
		/// <summary>
		/// 透明
		/// </summary>
		Blend = 1,
		/// <summary>
		/// 加算
		/// </summary>
		Add = 2,
		/// <summary>
		/// 減算
		/// </summary>
		Sub = 3,
		/// <summary>
		/// 乗算
		/// </summary>
		Mul = 4,
	};

	enum class TextureFilterType : int32_t
	{
		Nearest = 0,
		Linear = 1,
	};

	enum class TextureWrapType : int32_t
	{
		Repeat = 0,
		Clamp = 1,
	};

	enum class CullingType : int32_t
	{
		Front = 0,
		Back = 1,
		Double = 2,
	};

	/**
		@brief	描画方法
	*/
	enum class GraphicsDeviceType : int32_t
	{
		Default = 0,		///< 実行環境で最も安定している描画方法(初期化時に使用)
		DirectX11 = 1,		///< DirectX11
		OpenGL = 2,			///< OpenGL
	};

	enum class Object2DType : int32_t
	{
		Unknown,
		Texture,
		Camera,
		Effect,
		Text,
		Map,
	};

	enum eRenderedObject3DType
	{
		RENDERED_OBJECT3D_TYPE_UNKNOWN,
		RENDERED_OBJECT3D_TYPE_MESH,
		RENDERED_OBJECT3D_TYPE_CAMERA,
		RENDERED_OBJECT3D_TYPE_EFFECT,
		RENDERED_OBJECT3D_TYPE_DIRECTIONALLIGHT,
		RENDERED_OBJECT3D_TYPE_MASSOBJECT,
		RENDERED_OBJECT3D_TYPE_TERRAIN,
	};

	/**
		@brief	シェーダーに対して外部から設定可能なプロパティの変数の種類
	*/
	enum ShaderVariableType
	{
		SHADER_VARIABLE_TYPE_UNKNOWN,
		SHADER_VARIABLE_TYPE_FLOAT,
		SHADER_VARIABLE_TYPE_VECTOR2DF,
		SHADER_VARIABLE_TYPE_VECTOR3DF,
		SHADER_VARIABLE_TYPE_VECTOR4DF,
		SHADER_VARIABLE_TYPE_VECTOR4DF_ARRAY,
		SHADER_VARIABLE_TYPE_MATRIX44,
		SHADER_VARIABLE_TYPE_MATRIX44_ARRAY,
		SHADER_VARIABLE_TYPE_TEXTURE2D,
		SHADER_VARIABLE_TYPE_CUBEMAPTEXTURE,
	};

	/**
	@brief	3D描画時に表示されるバッファ
	*/
	enum class VisualizedBufferType : int32_t
	{
		FinalImage,
		DiffuseColor,
		SpecularColor,
		Normal,
		Smoothness,
		Environment,
		Occlusion,
	};

	/**
		@brief	描画設定のクラス
	*/
	class RenderSettings
	{
	public:
		/**
			@brief	遅延レンダリングから軽量レンダリングに変更し高速に描画するか?
		*/
		bool IsLightweightMode;
		
		/**
			@brief	画面に表示されるバッファ
		*/
		VisualizedBufferType VisualizedBuffer;

		RenderSettings()
		{
			IsLightweightMode = false;
			VisualizedBuffer = VisualizedBufferType::FinalImage;
		}
	};

	/**
	@brief	参照カウンタのインターフェース
	*/
	class IReference
	{
	public:
		/**
			@brief	参照カウンタを加算する。
			@return	加算後の参照カウンタ
		*/
		virtual int AddRef() = 0;

		/**
		@brief	参照カウンタを取得する。
		@return	参照カウンタ
		*/
		virtual int GetRef() = 0;

		/**
		@brief	参照カウンタを減算する。0になった時、インスタンスを削除する。
		@return	減算後の参照カウンタ
		*/
		virtual int Release() = 0;
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};