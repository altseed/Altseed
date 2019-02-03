#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../asd.Core.Base_Imp.h"

#include "asd.Graphics.h"

#include "Resource/asd.Texture2D_Imp.h"
#include "Resource/asd.Font_Imp.h"
#include "Resource/asd.RenderTexture2D_Imp.h"

#include "../IO/asd.File_Imp.h"

#include <Graphics/asd.Color.h>
#include <Math/asd.Vector2DI.h>

#include "../Utils/asd.ResourceContainer.h"

#if _WIN32

#ifdef _WIN64

#if _DEBUG
#pragma comment(lib,"x64/Debug/libgd_static.lib")
#else
#pragma comment(lib,"x64/Release/libgd_static.lib")
#endif

#else

#if _DEBUG
#pragma comment(lib,"x86/Debug/libgd_static.lib")
#else
#pragma comment(lib,"x86/Release/libgd_static.lib")
#endif

#endif

#endif

#if _WIN32
#elif __APPLE__
#else
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

#if !SWIG
	class ImageHelper
	{
	public:

		/**
		@brief	画像を保存する。
		@param	filepath	保存先
		@param	width	横幅
		@param	height	縦幅
		@param	data	隙間なく敷き詰められた画素データ(1画素4byte固定)
		@param	rev		上下反転で保存する。
		@note
		拡張子で画像の形式を判断する。
		*/
		static void SaveImage(const achar* filepath, int32_t width, int32_t height, void* data, bool rev);

		/**
		@brief	JPGファイルを保存する。
		@param	filepath	保存先
		@param	width	横幅
		@param	height	縦幅
		@param	data	隙間なく敷き詰められた画素データ(1画素4byte固定)
		@param	rev		上下反転で保存する。
		@note
		現在、Windows専用である。
		*/
		static void SaveJPGImage(const achar* filepath, int32_t width, int32_t height, void* data, bool rev);

		/**
		@brief	PNGファイルを保存する。
		@param	filepath	保存先
		@param	width	横幅
		@param	height	縦幅
		@param	data	隙間なく敷き詰められた画素データ(1画素4byte固定)
		@param	rev		上下反転で保存する。
		*/
		static void SavePNGImage(const achar* filepath, int32_t width, int32_t height, void* data, bool rev);

		/**
			@brief	PNGファイルを読み込む。
			@param	data	データ
			@param	size	データサイズ
			@param	imagewidth	画像横幅
			@param	imageheight	画像縦幅
			@param	imagedst		出力結果(newで確保される)
			@return	成否
		*/
		static bool LoadPNGImage(void* data, int32_t size, bool rev, int32_t& imagewidth, int32_t& imageheight, std::vector<uint8_t>& imagedst, Log* log);

		/**
			@brief	1ピクセルのサイズを取得する。
			@param	format	フォーマット
			@return	サイズ
		*/
		static int32_t GetPitch(TextureFormat format);

		/**
			@brief	画像が消費する推定VRAMサイズを取得する。
		*/
		static int32_t GetVRAMSize(TextureFormat format, int32_t width, int32_t height);

		static int32_t GetMipmapCount(int32_t width, int32_t height);
		static void GetMipmapSize(int mipmap, int32_t& width, int32_t& height);

		static bool IsPNG(const void* data, int32_t size);
		static bool IsDDS(const void* data, int32_t size);
	};

	class GraphicsHelper
	{
	public:
		static std::string GetFormatName(Graphics_Imp* graphics, TextureFormat format);
	};

	struct GraphicsOption
	{
		bool			IsReloadingEnabled;
		bool			IsFullScreen;
		ColorSpaceType	ColorSpace;
		GraphicsDeviceType	GraphicsDevice;
	};
#endif

#if !SWIG
	class WindowOpenGLX11
	{
#if _WIN32
#elif __APPLE__
#else
		GLXContext glx;
		Display* x11Display;
		::Window x11Window;
#endif
	public:
		WindowOpenGLX11();

		virtual ~WindowOpenGLX11();

		bool Initialize(void* display, void* window);

		void MakeContextCurrent();

		void SwapBuffers();
	};
#endif

/**
	@brief	描画を管理するクラス
	@note	現在、Graphicsクラスのインスタンスが複数存在した時の挙動は保証していない。
*/
	class Graphics_Imp
		: public Graphics
	{
		friend class DeviceObject;

	public:
		static const int32_t		MaxRenderTarget = 4;
		static const int32_t		MaxTextureCount = 16;

	protected:
		Window* m_window;
		WindowOpenGLX11* windowHelper = nullptr;
		ar::Context* rhiContext = nullptr;
		ar::DrawParameter drawParam;

		bool isSceneRunning = false;
		bool isRenderTargetDirty = false;

		std::array<RenderTexture2D_Imp*, 4> renderTargets;
		DepthBuffer_Imp* depthTarget = nullptr;

		std::array<RenderTexture2D_Imp*, 4> currentRenderTargets;
		DepthBuffer_Imp* currentDepthTarget = nullptr;

		bool isInitializedAsDX9 = false;

	private:
		ar::Manager*			rhi = nullptr;

		std::set<DeviceObject*>	m_deviceObjects;

		VertexBuffer_Imp*	m_vertexBufferPtr;
		IndexBuffer_Imp*	m_indexBufferPtr;
		NativeShader_Imp*	m_shaderPtr;

		Effekseer::FileInterface*	m_effectFileInterface = nullptr;
		Effekseer::Setting*	m_effectSetting = nullptr;

		ShaderCache*		m_shaderCache = nullptr;

		int32_t				drawCallCount = 0;
		int32_t				drawCallCountCurrent = 0;

		int32_t				vramCount = 0;

		GraphicsOption		option;

		void AddDeviceObject(DeviceObject* o);
		void RemoveDeviceObject(DeviceObject* o);

	protected:
		void ResetDrawState();

		Vector2DI					m_size;
		//GraphicsResourceContainer*	m_resourceContainer;

		Log*						m_log;
		File*						m_file;

		struct
		{
			RenderState					renderState;
			TextureFilterType			textureFilterTypes[MaxTextureCount];
			TextureWrapType				textureWrapTypes[MaxTextureCount];

			TextureFilterType			textureFilterTypes_vs[MaxTextureCount];
			TextureWrapType				textureWrapTypes_vs[MaxTextureCount];

		} currentState, nextState;

	protected:
		std::shared_ptr<RenderingThread>	m_renderingThread;

		static void StartRenderingThreadFunc(void* self);
		virtual void StartRenderingThread();

		static void EndRenderingThreadFunc(void* self);
		virtual void EndRenderingThread();

	protected:
		Texture2D* CreateTexture2D_(const achar* path) { return CreateTexture2D_Imp(path); }
		Texture2D* CreateTexture2DAsRawData_(const achar* path) { return CreateTexture2DAsRawData_Imp(path); }
		Texture2D* CreateEmptyTexture2D_(int32_t width, int32_t height, TextureFormat format) { return CreateEmptyTexture2D_Imp(width, height, format); }
		Texture2D* CreateEditableTexture2D_(const achar* path) { return CreateEditableTexture2D_Imp(path); }

		RenderTexture2D* CreateRenderTexture2D_(int32_t width, int32_t height, TextureFormat format) override;
	
	protected:
		void UpdateDrawStates(VertexBuffer_Imp* vertexBuffer,
							  IndexBuffer_Imp* indexBuffer,
							  NativeShader_Imp* shaderPtr,
							  int32_t& vertexBufferOffset);
		virtual void DrawPolygonInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr);
		virtual void DrawPolygonInternal(int32_t offset, int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr);

		virtual void DrawPolygonInstancedInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr, int32_t instanceCount);
		virtual void BeginInternal();
		virtual void EndInternal();

	private:

	public:

#if !SWIG
		virtual Texture2D_Imp* CreateTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size);
#endif

	private:
		virtual Texture2D_Imp* CreateTexture2DAsRawData_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size);

		virtual Texture2D_Imp* CreateEmptyTexture2D_Imp_Internal(Graphics* graphics, int32_t width, int32_t height, TextureFormat format, void* data);

		virtual Texture2D_Imp* CreateEditableTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size);

	public:
#if !SWIG
		std::shared_ptr<ResourceContainer<Texture2D_Imp>> Texture2DContainer;
		std::shared_ptr<ResourceContainer<Effect_Imp>> EffectContainer;
		std::shared_ptr<ResourceContainer<Font_Imp>> FontContainer;
		std::shared_ptr<ResourceContainer<Model_Imp>> ModelContainer;
		std::shared_ptr<ResourceContainer<ImagePackage_Imp>> ImagePackageContainer;

		ar::Manager* GetRHI() const { return rhi; }
		File* GetFile() { return m_file; }
		Log* GetLog() { return m_log; }
		GraphicsOption GetOption() { return option; }

		void IncVRAM(int32_t size) { vramCount += size; }
		void DecVRAM(int32_t size) { vramCount -= size; }

#endif

#if !SWIG
		Graphics_Imp(ar::Manager* manager, Vector2DI size, Log* log, File* file, GraphicsOption option);

		Graphics_Imp(ar::Manager* manager,
				  Window* window,
				  WindowOpenGLX11* windowHelper,
				  Vector2DI size,
				  Log* log,
				  File* file,
				  GraphicsOption option);

		virtual ~Graphics_Imp();

		void ApplyRenderTargets();

		static Graphics_Imp*
		Create(Window* window, void* handle1, void* handle2, int32_t width, int32_t height, Log* log, File* file, GraphicsOption option);

		static Graphics_Imp* Create(Window* window, Log* log, File* file, GraphicsOption option);

		static Graphics_Imp*
		Create(void* handle1, void* handle2, int32_t width, int32_t height, Log* log, File* file, GraphicsOption option);
		
		static Graphics_Imp* Create(Window* window, GraphicsDeviceType graphicsDevice, Log* log, File* file, GraphicsOption option);

		static Graphics_Imp* Create(void* handle1, void* handle2, int32_t width, int32_t height, GraphicsDeviceType graphicsDevice, Log* log, File *file, GraphicsOption option);

		/**
		@brief	画面をクリアする。
		@param	isColorTarget	色をクリアするか
		@param	isDepthTarget	深度をクリアするか
		@param	color			クリアに使用する色
		*/
		virtual void Clear(bool isColorTarget, bool isDepthTarget, const Color& color);

		/**
		@brief	内部のバッファを画面に表示する。
		*/
		virtual void Present();

		/**
		@brief	スクリーンショットを保存する。
		@param	path	保存先
		@note
		OpenGLの現状の実装だと、RenderTarget等が指定されている状態では使えないため、
		使用できるのはDoEventsかPresent近辺のみである。
		*/
		virtual void SaveScreenshot(const achar* path);

		/**
		@brief	スクリーンショットを保存する。
		@param	bufs	保存先
		@note
		OpenGLの現状の実装だと、RenderTarget等が指定されている状態では使えないため、
		使用できるのはDoEventsかPresent近辺のみである。
		*/
		virtual void SaveScreenshot(std::vector<Color>& bufs, Vector2DI& size);
#endif

		int32_t GetDrawCallCount() const override { return drawCallCount; };

		int32_t GetUsedVRAMSize() const override { return vramCount; }

		/**
		@brief	テクスチャを生成する。
		@param	path	パス
		@return	テクスチャ
		*/
		Texture2D_Imp* CreateTexture2D_Imp(const achar* path);

		/**
		@brief	テクスチャを生成する。
		@param	path	パス
		@return	テクスチャ
		*/
		Texture2D_Imp* CreateTexture2DAsRawData_Imp(const achar* path);

		/**
		@brief	空のテクスチャを生成する。
		@param	width	横幅
		@param	height	縦幅
		@param	format	フォーマット
		@return	テクスチャ
		*/
		Texture2D_Imp* CreateEmptyTexture2D_Imp(int32_t width, int32_t height, TextureFormat format);

		/**
		@brief	編集可能なテクスチャを生成する。
		@param	path	パス
		@return	テクスチャ
		*/
		Texture2D_Imp* CreateEditableTexture2D_Imp(const achar* path);

		/**
		@brief	生データからテクスチャを生成する。
		@param	width	横幅
		@param	height	縦幅
		@param	format	フォーマット
		@param	data	データ
		@return	テクスチャ
		*/
		Texture2D_Imp* CreateTexture2DWithRawData(int32_t width, int32_t height, TextureFormat format, void* data);

		/**
		@brief	描画先として指定可能なテクスチャを生成する。
		@param	width	横幅
		@param	height	縦幅
		@param	format	フォーマット
		*/
		virtual RenderTexture2D_Imp* CreateRenderTexture2D_Imp(int32_t width, int32_t height, TextureFormat format);

		CubemapTexture* CreateCubemapTexture_(const achar* path) override;
		
		/*
		virtual CubemapTexture* CreateCubemapTextureFrom6ImageFiles_(const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom) { return nullptr; }
		
		virtual CubemapTexture* CreateCubemapTextureFromMipmapImageFiles_(const achar* path, int32_t mipmapCount) { return nullptr; }

		virtual CubemapTexture* CreateCubemapTextureFromSingleImageFile_(const achar* path)  { return nullptr; }
		*/

		/**
			@brief	シェーダー(2D)を生成する。
			@param	shaderText						シェーダーのコード
			@return	シェーダー(2D)
		*/
		Shader2D* CreateShader2D_(const achar* shaderText) override;

		Shader2D* CreateShader2DFromBinary_(const achar* path) override;

		/**
			@brief	マテリアル(2D)を生成する。
			@param	shader	シェーダー
			@return	マテリアル(2D)
		*/
		Material2D* CreateMaterial2D_(Shader2D* shader);

		Shader3D* CreateShader3D_(const achar* shaderText) override;

		Material3D* CreateMaterial3D_(Shader3D* shader) override;

		MaterialPropertyBlock* CreateMaterialPropertyBlock_() override;

		/**
		@brief	メッシュを生成する。
		@return	メッシュ
		*/
		Mesh* CreateMesh_();

		Deformer* CreateDeformer_();

		Model* CreateModel_(const achar* path);

		MassModel* CreateMassModelFromModelFile_(const achar* path) override;

		MassModel* CreateMassModel_(const achar* path) override;

		Terrain3D* CreateTerrain3D_() override;

		Effect* CreateEffect_(const achar* path);

		Font* CreateFont_(const achar* path);

		Font* CreateDynamicFont_(const achar* font, int32_t fontSize, Color color, int32_t outlineSize, Color outlineColor) override;

		ImagePackage* CreateImagePackage_(const achar* path) override;

		MediaPlayer* CreateMediaPlayer_() override;

		VertexBuffer_Imp* CreateVertexBuffer_Imp_(int32_t size, int32_t count, bool isDynamic);
		
		IndexBuffer_Imp* CreateIndexBuffer_Imp_(int maxCount, bool isDynamic, bool is32bit);

#if !SWIG
	/**
	@brief	深度バッファを生成する。
	@param	width	横幅
	@param	height	縦幅
	*/
	DepthBuffer_Imp* CreateDepthBuffer_Imp(int32_t width, int32_t height);

	/**
		@brief	頂点バッファを生成する。
		@param	size	頂点のバッファサイズ
		@param	count	頂点数
		@param	isDynamic	内部メモリの変更を高速にできるようにするか?
	*/
	std::shared_ptr<VertexBuffer_Imp> CreateVertexBuffer_Imp(
		int32_t size, 
		int32_t count, 
		bool isDynamic) 
	{ return CreateSharedPtr(CreateVertexBuffer_Imp_(size, count, isDynamic)); }

	/**
	@brief	インデックスバッファを生成する。
	@param	maxCount	インデックス数
	@param	isDynamic	内部メモリの変更を高速にできるようにするか?
	@param	is32bit		内部メモリのインデックスをint32_tとして確保するか?
	*/
	std::shared_ptr<IndexBuffer_Imp> CreateIndexBuffer_Imp(
		int maxCount,
		bool isDynamic,
		bool is32bit)
	{
		return CreateSharedPtr(CreateIndexBuffer_Imp_(maxCount, isDynamic, is32bit));
	}

	/**
		@brief	シェーダーを生成する。
		@note
		基本的にShaderCacheを経由してシェーダーを生成するため、この関数を直接使う機会は殆ど無い。
	*/
	NativeShader_Imp* CreateShader_Imp_(
		const char* vertexShaderText,
		const char* vertexShaderFileName,
		const char* pixelShaderText,
		const char* pixelShaderFileName,
		std::vector <VertexLayout>& layout,
		bool is32Bit,
		std::vector <Macro>& macro);

	/**
	@brief	シェーダーを生成する。
	@note
	基本的にShaderCacheを経由してシェーダーを生成するため、この関数を直接使う機会は殆ど無い。
	*/
	NativeShader_Imp* CreateShader_Imp_(const uint8_t* vertexShader,
		int32_t vertexShaderSize,
		const uint8_t* pixelShader,
		int32_t pixelShaderSize,
		std::vector<VertexLayout>& layout,
		bool is32Bit);

	/**
	@brief	シェーダーを生成する。
	@note
	基本的にShaderCacheを経由してシェーダーを生成するため、この関数を直接使う機会は殆ど無い。
	*/
	std::shared_ptr<NativeShader_Imp> CreateShader_Imp(
		const char* vertexShaderText,
		const char* vertexShaderFileName,
		const char* pixelShaderText,
		const char* pixelShaderFileName,
		std::vector <VertexLayout>& layout,
		bool is32Bit,
		std::vector <Macro>& macro)
	{
		return CreateSharedPtr(CreateShader_Imp_(vertexShaderText, vertexShaderFileName, pixelShaderText, pixelShaderFileName, layout, is32Bit, macro));
	}

	/**
	@brief	シェーダキャッシュを取得する。
	@return	シェーダキャッシュ
	*/
	ShaderCache* GetShaderCache() { return m_shaderCache; }

	/**
		@brief	リソースコンテナを取得する。
		@return	リソースコンテナ
	*/
	//GraphicsResourceContainer* GetResourceContainer() { return m_resourceContainer; }

	/**
		@brief	描画のための頂点バッファを設定する。
	*/
	void SetVertexBuffer(VertexBuffer_Imp* vertexBuffer);

	/**
	@brief	描画のためのインデックスバッファを設定する。
	*/
	void SetIndexBuffer(IndexBuffer_Imp* indexBuffer);

	/**
	@brief	描画のためのシェーダーを設定する。
	*/
	void SetShader(NativeShader_Imp* shader);

	/**
	@brief	描画のためのレンダーステートを設定する。
	*/
	void SetRenderState(const RenderState& renderState);

	/**
	@brief	レンダーステートの変更を実際に適用する。
	@param	forced	設定の変更ありなし関係なく無条件に適用する。
	*/
	virtual void CommitRenderState(bool forced);

	/**
	@brief	ポリゴンを描画する。
	@param	count	ポリゴン数
	*/
	void DrawPolygon(int32_t count);

	/**
	@brief	ポリゴンを描画する。
	@param	offset	ポリゴン数オフセット
	@param	count	ポリゴン数
	*/
	void DrawPolygon(int32_t offset, int32_t count);

	/**
	@brief	ポリゴンを描画する。
	@param	count			ポリゴン数
	@param	instanceCount	インスタンス数
	*/
	void DrawPolygonInstanced(int32_t count, int32_t instanceCount);

	/**
	@brief	描画開始前のリセット処理を行う。
	*/
	void Begin();

	/**
	@brief	描画終了後のリセット処理を行う。
	*/
	void End();

	/** 
	@brief	リロード処理を行う。
	*/
	void Reload();

	/**
		@brief	描画先を設定する。
		@param	texture		描画先のテクスチャ
		@param	depthBuffer	描画先の深度バッファ
		@note
		textureがnullの場合は無条件で、デフォルトの描画先を使用する。
	*/
	virtual void SetRenderTarget(RenderTexture2D_Imp* texture, DepthBuffer_Imp* depthBuffer);

	/**
	@brief	描画先を設定する。
	@param	texture1		描画先のテクスチャ
	@param	depthBuffer	描画先の深度バッファ
	@note
	textureがnullの場合は無条件で、デフォルトの描画先を使用する。
	*/
	virtual void SetRenderTarget(RenderTexture2D_Imp* texture1, RenderTexture2D_Imp* texture2, RenderTexture2D_Imp* texture3, RenderTexture2D_Imp* texture4, DepthBuffer_Imp* depthBuffer);

	/**
		@brief	コンテキストの設定を行う。
		@note
		DirectXの場合は意味はない。
	*/
	virtual void MakeContextCurrent();

	/**
		@brief	GPUへの命令が終了するまで待機する。
		@note
		DirectXの場合は意味はない。
	*/
	virtual void FlushCommand();

	virtual void SetIsFullscreenMode(bool isFullscreenMode);

	virtual void SetWindowSize(Vector2DI size);

	/**
		@brief	スレッドを取得する。
		@return	スレッド
	*/
	std::shared_ptr<RenderingThread>& GetRenderingThread() { return m_renderingThread; }

	/**
		@brief	エフェクトのファイルインターフェースを取得する。
		@return	ファイルインターフェース
	*/
	Effekseer::FileInterface* GetEffectFileInterface() { return m_effectFileInterface; }

	/**
		@brief	エフェクトの設定を取得する。
		@return	設定
	*/
	Effekseer::Setting* GetEffectSetting() { return m_effectSetting; }

	public:
	bool GetIsInitializedAsDX9() { return isInitializedAsDX9; }

#ifdef _WIN32
	ID3D11Device* GetDevice() { return (ID3D11Device*)GetRHI()->GetInternalObjects()[0]; }
	ID3D11DeviceContext* GetContext() { return (ID3D11DeviceContext*)GetRHI()->GetInternalObjects()[1]; }
#endif

	GraphicsDeviceType GetGraphicsDeviceType() const { return (GraphicsDeviceType)GetRHI()->GetDeviceType(); }
#endif
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

}
