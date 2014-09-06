#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include "../ace.ReferenceObject.h"
#include "../ace.Core.Base.h"

#include "Resource/ace.Texture2D.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class Graphics
	: public ReferenceObject
{
protected:
	virtual Texture2D* CreateTexture2D_(const achar* path) = 0;
	virtual Texture2D* CreateEmptyTexture2D_(int32_t width, int32_t height, TextureFormat format) = 0;
	virtual RenderTexture2D* CreateRenderTexture2D_(int32_t width, int32_t height, TextureFormat format) = 0;
	virtual CubemapTexture* CreateCubemapTextureFrom6ImageFiles_(const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom) = 0;
	virtual CubemapTexture* CreateCubemapTextureFromMipmapImageFiles_(const achar* path, int32_t mipmapCount) = 0;

	virtual Shader2D* CreateShader2D_( const achar* shaderText) = 0;
	virtual Material2D* CreateMaterial2D_(Shader2D* shader) = 0;

	virtual Shader3D* CreateShader3D_(const achar* shaderText) = 0;
	virtual Material3D* CreateMaterial3D_(Shader3D* shader) = 0;

	virtual MaterialPropertyBlock* CreateMaterialPropertyBlock_() = 0;

	virtual Mesh* CreateMesh_() = 0;
	virtual Deformer* CreateDeformer_() = 0;
	virtual Model* CreateModel_(const achar* path) = 0;
	virtual Effect* CreateEffect_(const achar* path) = 0;
	virtual Font* CreateFont_(const achar* path) = 0;
	virtual Chip2D* CreateChip2D_() = 0;

public:
	Graphics(){}
	virtual ~Graphics(){}

#ifndef SWIG

	/**
	@brief	テクスチャを生成する。
	@param	path	パス
	@return	テクスチャ
	*/
	std::shared_ptr<Texture2D> CreateTexture2D(const achar* path) { return CreateSharedPtrWithReleaseDLL(CreateTexture2D_(path)); }

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
	@return	キューブマップ
	*/
	std::shared_ptr<CubemapTexture> CreateCubemapTextureFromMipmapImageFiles(const achar* path, int32_t mipmapCount)
	{
		return CreateSharedPtrWithReleaseDLL(
			CreateCubemapTextureFromMipmapImageFiles_(
			path, mipmapCount));
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
	@brief	エフェクトを生成する。
	@param	path	パス
	@return	エフェクト
	*/
	std::shared_ptr<Effect> CreateEffect(const achar* path)
	{
		auto effect = CreateEffect_(path);
		return CreateSharedPtrWithReleaseDLL(effect);
	}

	/**
	@brief	2Dチップを生成する。
	@param	texture	テクスチャ
	@return	2Dチップ
	*/
	std::shared_ptr<Chip2D> CreateChip2D()
	{
		auto chip = CreateChip2D_();
		return CreateSharedPtrWithReleaseDLL(chip);
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

#endif

	/**
	@brief	1フレーム間に実行された描画命令の回数を取得する。
	@return	描画命令の回数
	@note
	現在、エフェクトの描画回数はカウントされない。
	*/
	virtual int32_t GetDrawCallCount() const = 0;

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