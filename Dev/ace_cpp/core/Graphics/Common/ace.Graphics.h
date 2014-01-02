#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>
#include "../../ace.ReferenceObject.h"
#include "../../ace.Core.Base.h"

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
	virtual RenderTexture2D* CreateRenderTexture2D_(int32_t width, int32_t height, eTextureFormat format) = 0;
	virtual Shader2D* CreateShader2D_( const achar* shaderText, ShaderVariableProperty* variableProperties, int32_t variablePropertiesCount) = 0;
	virtual Material2D* CreateMaterial2D_(Shader2D* shader) = 0;
	virtual Mesh* CreateMesh_() = 0;
	virtual Armature* CreateArmature_() = 0;

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
	@brief	描画先として指定可能なテクスチャを生成する。
	@param	width	横幅
	@param	height	縦幅
	@return	テクスチャ
	*/
	std::shared_ptr<RenderTexture2D> CreateRenderTexture(int32_t width, int32_t height, eTextureFormat format){ return CreateSharedPtrWithReleaseDLL(CreateRenderTexture2D_(width, height, format)); }

	/**
	@brief	シェーダー(2D)を生成する。
	@param	shaderText						シェーダーのコード
	@param	variableProperties				シェーダーで使用可能な外部入力可能な変数
	@return	シェーダー(2D)
	*/
	std::shared_ptr<Shader2D> CreateShader2D(
		const achar* shaderText,
		std::vector <ShaderVariableProperty>& variableProperties)
	{
		return CreateSharedPtrWithReleaseDLL(
			CreateShader2D_(
			shaderText,
			variableProperties.size() > 0 ? &(variableProperties[0]) : nullptr,
			variableProperties.size()));
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
	@brief	メッシュを生成する。
	@return	メッシュ
	*/
	std::shared_ptr<Mesh> CreateMesh()
	{
		auto mesh = CreateMesh_();
		return CreateSharedPtrWithReleaseDLL(mesh);
	}

	/**
	@brief	アーマチュアを生成する。
	@return	アーマチュア
	*/
	std::shared_ptr<Armature> CreateArmature()
	{
		auto armature = CreateArmature_();
		return CreateSharedPtrWithReleaseDLL(armature);
	}

#endif

	/**
	@brief	描画ランタイムの種類を取得する。
	@return	種類
	*/
	virtual eGraphicsType GetGraphicsType() const = 0;

};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

}