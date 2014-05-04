#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../../ace.Core.Base_Imp.h"

#include "ace.Graphics.h"
#include "ace.GraphicsResourceContainer.h"

#include "Resource/ace.Texture2D_Imp.h"
#include "Resource/ace.RenderTexture2D_Imp.h"

#include <Graphics/ace.Color.h>
#include <Math/ace.Vector2DI.h>



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

#if !SWIG
	class ImageHelper
	{
	public:

		/**
			@brief	PNGファイルを読み込む。
			@param	data	データ
			@param	size	データサイズ
			@param	imagewidth	画像横幅
			@param	imageheight	画像縦幅
			@param	imagedst		出力結果(newで確保される)
			@return	成否
		*/
		static bool LoadPNGImage(void* data, int32_t size, bool rev, int32_t& imagewidth, int32_t& imageheight, void*& imagedst);

		/**
			@brief	1ピクセルのサイズを取得する。
			@param	format	フォーマット
			@return	サイズ
		*/
		static int32_t GetPitch(eTextureFormat format);
	};

	class EffectTextureLoader
		: public ::Effekseer::TextureLoader
	{
	protected:
		Graphics_Imp*	m_graphics = nullptr;

		std::map<astring, void*>		m_caches;

		virtual void* InternalLoad(Graphics_Imp* graphics, void* data, int32_t width, int32_t height ) = 0;
		virtual void InternalUnload(void* data) = 0;
		virtual bool IsReversed() = 0;
	public:

		EffectTextureLoader(Graphics_Imp* graphics);
		virtual ~EffectTextureLoader();

		void* Load(const EFK_CHAR* path);
		void Unload(void* data);
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

	private:
		std::set<DeviceObject*>	m_deviceObjects;

		VertexBuffer_Imp*	m_vertexBufferPtr;
		IndexBuffer_Imp*	m_indexBufferPtr;
		NativeShader_Imp*	m_shaderPtr;
		bool				m_isMultithreadingMode;

		Effekseer::Setting*	m_effectSetting = nullptr;

		ShaderCache*		m_shaderCache = nullptr;

		void AddDeviceObject(DeviceObject* o);
		void RemoveDeviceObject(DeviceObject* o);

	protected:
		void ResetDrawState();

		Vector2DI					m_size;
		RenderState_Imp*			m_renderState;
		GraphicsResourceContainer*	m_resourceContainer;

		Log*						m_log;

		/**
			@brief	PNGファイルを保存する。
			@param	filepath	保存先
			@param	width	横幅
			@param	height	縦幅
			@param	data	隙間なく敷き詰められた画素データ(1画素4byte固定)
			@param	rev		上下反転で保存する。
			*/
		void SavePNGImage(const achar* filepath, int32_t width, int32_t height, void* data, bool rev);

	protected:
		std::shared_ptr<RenderingThread>	m_renderingThread;

		static void StartRenderingThreadFunc(void* self);
		virtual void StartRenderingThread();

		static void EndRenderingThreadFunc(void* self);
		virtual void EndRenderingThread();

	protected:
		Texture2D* CreateTexture2D_(const achar* path) { return CreateTexture2D_Imp(path); }
		Texture2D* CreateEmptyTexture2D_(int32_t width, int32_t height, eTextureFormat format) { return CreateEmptyTexture2D_Imp(width, height, format); }

		RenderTexture2D* CreateRenderTexture2D_(int32_t width, int32_t height, eTextureFormat format) { return CreateRenderTexture2D_Imp(width, height, format); }
		Shader2D* CreateShader2D_( const achar* shaderText, ShaderVariableProperty* variableProperties, int32_t variablePropertiesCount);
		
	protected:
		virtual VertexBuffer_Imp* CreateVertexBuffer_Imp_(int32_t size, int32_t count, bool isDynamic) = 0;
		virtual IndexBuffer_Imp* CreateIndexBuffer_Imp_(int maxCount, bool isDynamic, bool is32bit) = 0;

		virtual void DrawPolygonInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr) = 0;
		virtual void DrawPolygonInstancedInternal(int32_t count, VertexBuffer_Imp* vertexBuffer, IndexBuffer_Imp* indexBuffer, NativeShader_Imp* shaderPtr, int32_t instanceCount) = 0;
		virtual void BeginInternal() = 0;
		virtual void EndInternal() {}

		/**
			@brief	テクスチャを生成する。
			@param	graphics	グラフィック
			@param	data		データ
			@param	size		データサイズ
			*/
		virtual Texture2D_Imp* CreateTexture2D_Imp_Internal(Graphics* graphics, uint8_t* data, int32_t size) = 0;

		virtual Texture2D_Imp* CreateEmptyTexture2D_Imp_Internal(Graphics* graphics, int32_t width, int32_t height, eTextureFormat format) = 0;

	public:
		Graphics_Imp(Vector2DI size, Log* log, bool isMultithreadingMode);
		virtual ~Graphics_Imp();

		static Graphics_Imp* Create(Window* window, bool isOpenGLMode, Log* log, bool isMultithreadingMode);

		static Graphics_Imp* Create(void* handle1, void* handle2, int32_t width, int32_t height, bool isOpenGLMode, Log* log, bool isMultithreadingMode);

		bool IsMultithreadingMode() { return m_isMultithreadingMode; }

		/**
		@brief	画面をクリアする。
		@param	isColorTarget	色をクリアするか
		@param	isDepthTarget	深度をクリアするぁ
		@param	color			クリアに使用する色
		*/
		virtual void Clear(bool isColorTarget, bool isDepthTarget, const Color& color) = 0;

		/**
		@brief	内部のバッファを画面に表示する。
		*/
		virtual void Present() = 0;

		/**
		@brief	スクリーンショットを保存する。
		@param	path	保存先
		@note
		OpenGLの現状の実装だと、RenderTarget等が指定されている状態では使えないため、
		使用できるのはDoEventsかPresent近辺のみである。
		*/
		virtual void SaveScreenshot(const achar* path) = 0;

		/**
		@brief	テクスチャを生成する。
		@param	path	パス
		@return	テクスチャ
		*/
		Texture2D_Imp* CreateTexture2D_Imp(const achar* path);

		/**
		@brief	空のテクスチャを生成する。
		@param	width	横幅
		@param	height	縦幅
		@param	format	フォーマット
		@return	テクスチャ
		*/
		Texture2D_Imp* CreateEmptyTexture2D_Imp(int32_t width, int32_t height, eTextureFormat format);

		/**
		@brief	描画先として指定可能なテクスチャを生成する。
		@param	width	横幅
		@param	height	縦幅
		@param	format	フォーマット
		*/
		virtual RenderTexture2D_Imp* CreateRenderTexture2D_Imp(int32_t width, int32_t height, eTextureFormat format) = 0;

		/**
			@brief	SWIG向けに記述
		*/
		virtual CubemapTexture* CreateCubemapTextureFrom6ImageFiles_(const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom) { return nullptr; }
		
		/**
			@brief	シェーダー(2D)を生成する。
			@param	shaderText						シェーダーのコード
			@param	pixelVariableProperties			シェーダーで使用可能な外部入力可能な変数
			@return	シェーダー(2D)
		*/
		Shader2D_Imp* CreateShader2D_Imp(
			const achar* shaderText,
			std::vector <ShaderVariableProperty>& variableProperties);

		/**
			@brief	マテリアル(2D)を生成する。
			@param	shader	シェーダー
			@return	マテリアル(2D)
		*/
		Material2D* CreateMaterial2D_(Shader2D* shader);

		/**
		@brief	メッシュを生成する。
		@return	メッシュ
		*/
		Mesh* CreateMesh_();

		Deformer* CreateDeformer_();

		Model* CreateModel_(const achar* path);

		Effect* CreateEffect_(const achar* path);

#if !SWIG
	/**
	@brief	深度バッファを生成する。
	@param	width	横幅
	@param	height	縦幅
	*/
	virtual DepthBuffer_Imp* CreateDepthBuffer_Imp(int32_t width, int32_t height) = 0;

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
	virtual NativeShader_Imp* CreateShader_Imp_(
		const char* vertexShaderText,
		const char* vertexShaderFileName,
		const char* pixelShaderText,
		const char* pixelShaderFileName,
		std::vector <VertexLayout>& layout,
		std::vector <Macro>& macro) = 0;

	std::shared_ptr<NativeShader_Imp> CreateShader_Imp(
		const char* vertexShaderText,
		const char* vertexShaderFileName,
		const char* pixelShaderText,
		const char* pixelShaderFileName,
		std::vector <VertexLayout>& layout,
		std::vector <Macro>& macro)
	{
		return CreateSharedPtr(CreateShader_Imp_(vertexShaderText, vertexShaderFileName, pixelShaderText, pixelShaderFileName, layout, macro));
	}


	/**
		@brief	レンダーステートを取得する。
		@return	レンダーステート
	*/
	RenderState_Imp* GetRenderState() { return m_renderState; };

	/**
	@brief	シェーダキャッシュを取得する。
	@return	シェーダキャッシュ
	*/
	ShaderCache* GetShaderCache() { return m_shaderCache; }

	/**
		@brief	リソースコンテナを取得する。
		@return	リソースコンテナ
	*/
	GraphicsResourceContainer* GetResourceContainer() { return m_resourceContainer; }

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
	@brief	ポリゴンを描画する。
	@param	count	ポリゴン数
	*/
	void DrawPolygon(int32_t count);

	/**
	@brief	描画開始前のリセット処理を行う。
	*/
	void Begin();

	/**
	@brief	描画終了後のリセット処理を行う。
	*/
	void End();

	/**
		@brief	描画先を設定する。
		@param	texture		描画先のテクスチャ
		@param	depthBuffer	描画先の深度バッファ
		@note
		textureがnullの場合は無条件で、デフォルトの描画先を使用する。
	*/
	virtual void SetRenderTarget(RenderTexture2D_Imp* texture, DepthBuffer_Imp* depthBuffer) = 0;

	/**
		@brief	コンテキストの設定を行う。
		@note
		DirectXの場合は意味はない。
	*/
	virtual void MakeContextCurrent() = 0;

	/**
		@brief	GPUへの命令が終了するまで待機する。
		@note
		DirectXの場合は意味はない。
	*/
	virtual void FlushCommand() = 0;

	/**
		@brief	スレッドを取得する。
		@return	スレッド
	*/
	std::shared_ptr<RenderingThread>& GetRenderingThread() { return m_renderingThread; }

	/**
		@brief	エフェクトの設定を取得する。
		@return	設定
	*/
	Effekseer::Setting* GetSetting() { return m_effectSetting; }
#endif
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

}
