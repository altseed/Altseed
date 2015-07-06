#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <asd.common.Base.h>
#include "../asd.ReferenceObject.h"
#include "../asd.Core.Base.h"

#include "Resource/asd.Texture2D.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class Graphics
	: public ReferenceObject
{
protected:
	virtual Texture2D* CreateTexture2D_(const achar* path) = 0;
	virtual Texture2D* CreateTexture2DAsRawData_(const achar* path) = 0;
	virtual Texture2D* CreateEmptyTexture2D_(int32_t width, int32_t height, TextureFormat format) = 0;
	virtual RenderTexture2D* CreateRenderTexture2D_(int32_t width, int32_t height, TextureFormat format) = 0;
	virtual CubemapTexture* CreateCubemapTextureFrom6ImageFiles_(const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom) = 0;
	virtual CubemapTexture* CreateCubemapTextureFromMipmapImageFiles_(const achar* path, int32_t mipmapCount) = 0;
	virtual CubemapTexture* CreateCubemapTextureFromSingleImageFile_(const achar* path) = 0;

	virtual Shader2D* CreateShader2D_( const achar* shaderText) = 0;
	virtual Material2D* CreateMaterial2D_(Shader2D* shader) = 0;

	virtual Shader3D* CreateShader3D_(const achar* shaderText) = 0;
	virtual Material3D* CreateMaterial3D_(Shader3D* shader) = 0;

	virtual MaterialPropertyBlock* CreateMaterialPropertyBlock_() = 0;

	virtual Mesh* CreateMesh_() = 0;
	virtual Deformer* CreateDeformer_() = 0;
	virtual Model* CreateModel_(const achar* path) = 0;
	virtual MassModel* CreateMassModelFromModelFile_(const achar* path) = 0;
	virtual MassModel* CreateMassModel_(const achar* path) = 0;
	virtual Terrain3D* CreateTerrain3D_() = 0;

	virtual Effect* CreateEffect_(const achar* path) = 0;
	virtual Font* CreateFont_(const achar* path) = 0;
	virtual Font* CreateDynamicFont_(const achar* font, int32_t fontSize, Color color, int32_t outlineSize, Color outlineColor) = 0;

	virtual ImagePackage* CreateImagePackage_(const achar* path) = 0;
public:
	Graphics(){}
	virtual ~Graphics(){}

#ifndef SWIG

	/**
	@brief	テクスチャを生成する。
	@param	path	画像ファイルへの相対パス
	@return	テクスチャ
	@warning	読み込める画像形式はPNGまたはDDSのみ。(DDSはDirectX動作時のみ)
	*/
	std::shared_ptr<Texture2D> CreateTexture2D(const achar* path) { return CreateSharedPtrWithReleaseDLL(CreateTexture2D_(path)); }

	/**
	@brief	テクスチャを生成する。
	@param	path	画像ファイルへの相対パス
	@return	テクスチャ
	@note
	画素がRGB空間にマッピングされている時に使用する。(ノーマルマップ等)
	@warning	読み込める画像形式はPNGまたはDDSのみ。(DDSはDirectX動作時のみ)
	*/
	std::shared_ptr<Texture2D> CreateTexture2DAsRawData(const achar* path) { return CreateSharedPtrWithReleaseDLL(CreateTexture2DAsRawData_(path)); }

	/**
	@brief	テクスチャを生成する。
	@param	width	横幅
	@param	height	縦幅
	@param	format	フォーマット
	@return	テクスチャ
	*/
	std::shared_ptr<Texture2D> CreateEmptyTexture2D(int32_t width, int32_t height, TextureFormat format)
	{
		return CreateSharedPtrWithReleaseDLL(CreateEmptyTexture2D_(width, height, format));
	}

	/**
	@brief	描画先として指定可能なテクスチャを生成する。
	@param	width	横幅
	@param	height	縦幅
	@param	format	フォーマット
	@return	テクスチャ
	*/
	std::shared_ptr<RenderTexture2D> CreateRenderTexture2D(int32_t width, int32_t height, TextureFormat format){ return CreateSharedPtrWithReleaseDLL(CreateRenderTexture2D_(width, height, format)); }

	/**
	@brief	6枚の画像ファイルからキューブマップテクスチャを生成する。
	@param	front	前方向の画像ファイルへの相対パス
	@param	left	左方向の画像ファイルへの相対パス
	@param	back	後ろ方向の画像ファイルへの相対パス
	@param	right	右方向の画像ファイルへの相対パス
	@param	top		上方向の画像ファイルへの相対パス
	@param	bottom	下方向の画像ファイルへの相対パス
	@warning	読み込める画像形式はPNGまたはDDSのみ。(DDSはDirectX動作時のみ)
	@return	キューブマップ
	*/
	std::shared_ptr<CubemapTexture> CreateCubemapTextureFrom6ImageFiles(const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom)
	{
		return CreateSharedPtrWithReleaseDLL(
			CreateCubemapTextureFrom6ImageFiles_(
			front, left, back, right, top, bottom));
	}

	/**
	@brief	複数の画像ファイルからミップマップ付のキューブマップテクスチャを生成する。
	@param	path		ファイルの名称の先頭
	@param	mipmapCount	ミップマップ数
	@warning	読み込める画像形式はPNGまたはDDSのみ。(DDSはDirectX動作時のみ)
	@return	キューブマップ
	*/
	std::shared_ptr<CubemapTexture> CreateCubemapTextureFromMipmapImageFiles(const achar* path, int32_t mipmapCount)
	{
		return CreateSharedPtrWithReleaseDLL(
			CreateCubemapTextureFromMipmapImageFiles_(
			path, mipmapCount));
	}

	/**
	@brief	1つのファイルからミップマップ付のキューブマップテクスチャを生成する。
	@param	path		画像ファイルへの相対パス
	@return	キューブマップ
	@warning	読み込める画像形式はPNGまたはDDSのみ。(DDSはDirectX動作時のみ)
	*/
	std::shared_ptr<CubemapTexture> CreateCubemapTextureFromSingleImageFile(const achar* path)
	{
		return CreateSharedPtrWithReleaseDLL(
			CreateCubemapTextureFromSingleImageFile_(
			path));
	}

	/**
	@brief	シェーダー(2D)を生成する。
	@param	shaderText						シェーダーのコード
	@return	シェーダー(2D)
	*/
	std::shared_ptr<Shader2D> CreateShader2D(const achar* shaderText)
	{
		return CreateSharedPtrWithReleaseDLL(CreateShader2D_(shaderText));
	}

	/**
	@brief	マテリアル(2D)を生成する。
	@param	shader	シェーダー
	@return	マテリアル(2D)
	*/
	std::shared_ptr<Material2D> CreateMaterial2D(std::shared_ptr<Shader2D> shader)
	{
		auto material = CreateMaterial2D_(shader.get());
		return CreateSharedPtrWithReleaseDLL(material);
	}

	/**
	@brief	シェーダー(3D)を生成する。
	@param	shaderText						シェーダーのコード
	@return	シェーダー(3D)
	*/
	std::shared_ptr<Shader3D> CreateShader3D(const achar* shaderText)
	{
		return CreateSharedPtrWithReleaseDLL(CreateShader3D_(shaderText));
	}

	/**
	@brief	マテリアル(3D)を生成する。
	@param	shader	シェーダー
	@return	マテリアル(3D)
	*/
	std::shared_ptr<Material3D> CreateMaterial3D(std::shared_ptr<Shader3D> shader)
	{
		auto material = CreateMaterial3D_(shader.get());
		return CreateSharedPtrWithReleaseDLL(material);
	}

	/**
	@brief	マテリアルプロパティブロックを生成する。
	@return	マテリアルプロパティブロック
	*/
	std::shared_ptr<MaterialPropertyBlock> CreateMaterialPropertyBlock()
	{
		auto block = CreateMaterialPropertyBlock_();
		return CreateSharedPtrWithReleaseDLL(block);
	}

	/**
	@brief	メッシュを生成する。
	@return	メッシュ
	*/
	std::shared_ptr<Mesh> CreateMesh()
	{
		auto mesh = CreateMesh_();
		return CreateSharedPtrWithReleaseDLL(mesh);
	}

	/**
	@brief	デフォーマーを生成する。
	@return	デフォーマー
	*/
	std::shared_ptr<Deformer> CreateDeformer()
	{
		auto deformer = CreateDeformer_();
		return CreateSharedPtrWithReleaseDLL(deformer);
	}

	/**
	@brief	モデルを生成する。
	@param	path	パス
	@return	モデル
	*/
	std::shared_ptr<Model> CreateModel(const achar* path)
	{
		auto model = CreateModel_(path);
		return CreateSharedPtrWithReleaseDLL(model);
	}

	/**
	@brief	大量描画用モデルを生成する。
	@param	path	パス
	@return	大量描画用モデル
	*/
	std::shared_ptr<MassModel> CreateMassModelFromModelFile(const achar* path)
	{
		auto model = CreateMassModelFromModelFile_(path);
		return CreateSharedPtrWithReleaseDLL(model);
	}

	/**
	@brief	大量描画用モデルを生成する。
	@param	path	パス
	@return	大量描画用モデル
	*/
	std::shared_ptr<MassModel> CreateMassModel(const achar* path)
	{
		auto model = CreateMassModel_(path);
		return CreateSharedPtrWithReleaseDLL(model);
	}

	/**
	@brief	地形を生成する。
	@return	地形
	*/
	std::shared_ptr<Terrain3D> CreateTerrain3D()
	{
		auto terrain = CreateTerrain3D_();
		return CreateSharedPtrWithReleaseDLL(terrain);
	}

	/**
	@brief	エフェクトを生成する。
	@param	path	パス
	@return	エフェクト
	*/
	std::shared_ptr<Effect> CreateEffect(const achar* path)
	{
		auto effect = CreateEffect_(path);
		return CreateSharedPtrWithReleaseDLL(effect);
	}

#undef CreateFont
	/**
	@brief	フォントを生成する。
	@param	path	パス
	@return	フォント
	*/
	std::shared_ptr<Font> CreateFont(const achar* path)
	{
		auto font = CreateFont_(path);
		return CreateSharedPtrWithReleaseDLL(font);
	}

	/**
	@brief	必要に応じて動的に生成されるフォントを生成する。
	@param	font			フォント名/フォントパス
	@param	fontSize		フォントサイズ
	@param	color			フォントの色
	@param	outlineSize		外枠の太さ
	@param	outlineColor	外枠の色
	@return	フォント
	@note
	文字を表示する時に必要な文字の分だけフォントを生成するフォントクラスを生成する。
	fontには、フォント名、もしくはフォントファイルへのパスを指定する。
	何もfontに文字を指定しないと標準フォントが使用される。
	事前に専用のフォントファイルを用意する必要はないが、アプリケーションを実行する環境に指定したフォントが存在する必要がある。
	*/
	std::shared_ptr<Font> CreateDynamicFont(const achar* font, int32_t fontSize, Color color, int32_t outlineSize, Color outlineColor)
	{
		auto font_ = CreateDynamicFont_(font, fontSize, color, outlineSize, outlineColor);
		return CreateSharedPtrWithReleaseDLL(font_);
	}


	/**
	@brief	画像パッケージを生成する。
	@param	path	パス
	@return	画像パッケージ
	*/
	std::shared_ptr<ImagePackage> CreateImagePackage(const achar* path)
	{
		auto ip = CreateImagePackage_(path);
		return CreateSharedPtrWithReleaseDLL(ip);
	}

#endif

	/**
	@brief	1フレーム間に実行された描画命令の回数を取得する。
	@return	描画命令の回数
	@note
	現在、エフェクトの描画回数はカウントされない。
	*/
	virtual int32_t GetDrawCallCount() const = 0;

	/**
		@brief	現在使用済みのVRAM容量を取得する。
		@return	VRAM容量
		@note
		値は推測値である。
	*/
	virtual int32_t GetUsedVRAMSize() const = 0;

	/**
	@brief	描画ランタイムの種類を取得する。
	@return	種類
	*/
	virtual GraphicsDeviceType GetGraphicsDeviceType() const = 0;

};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

}