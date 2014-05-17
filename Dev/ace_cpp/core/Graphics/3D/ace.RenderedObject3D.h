
#pragma once

#include <ace.common.Base.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Matrix44.h>
#include <Graphics/ace.Color.h>

#include "../../ace.Core.Base.h"
#include "../../ace.Core.Base_Imp.h"

#include "../../ace.ReferenceObject.h"


namespace ace
{
	class RenderingProperty
	{
	public:
		bool		IsLightweightMode = false;
		bool		IsDepthMode = false;

		Matrix44	CameraMatrix;
		Matrix44	ProjectionMatrix;

		// 軽量描画用
		Color		DirectionalLightColor;
		Vector3DF	DirectionalLightDirection;
		Color		SkyLightColor;
		Color		GroundLightColor;

		float		DepthRange;
		float		ZFar;
		float		ZNear;

		Matrix44	LightCameraMatrix;
		Matrix44	LightProjectionMatrix;
	};

	/**
		@brief	描画されるオブジェクトのクラス
		@note
		実装は以下の仕様に従う。
		
		-関数・変数名
		 メインスレッドのみ使用可能な関数-特に何もなし
		 レンダリングスレッドのみ使用可能な関数-サフィックスに_RT(RenderingThread)がつく。もしくはregionで指定されている。

		-Flip
		  メインスレッドとレンダリングスレッドのデータの同期を行う。
	*/
	class RenderedObject3D
		: public ReferenceObject
	{
	private:
		Graphics_Imp*	m_graphics;

		struct  
		{
			Vector3DF	position;
			Vector3DF	rotation;
			Vector3DF	scale;
			bool		isChanged;

		} m_commonValues;

		struct
		{
			Vector3DF	position;
			Vector3DF	rotation;
			Vector3DF	scale;
			Matrix44	localMatrix;
			bool		isChanged;

		} m_commonValues_RT;
		
		Matrix44 CalcLocalMatrix(Vector3DF& t, Vector3DF& r, Vector3DF& s);

	protected:
		
		Matrix44 CalcLocalMatrix();

		Graphics_Imp* GetGraphics() { return m_graphics;  }
	
	public:
		RenderedObject3D(Graphics* graphics);
		virtual ~RenderedObject3D();

		Vector3DF GetPosition() const;
		void SetPosition(const Vector3DF& pos);

		Vector3DF GetRotation() const;
		void SetRotation(const Vector3DF& rot);

		Vector3DF GetScale() const;
		void SetScale(const Vector3DF& scale);

		/**
			@brief	レンダラーに追加された時に呼ばれる
			@param	renderer	レンダラー
		*/
		virtual void OnAdded(Renderer3D* renderer) {}

		/**
			@brief	レンダラーから外れる時に呼ばれる
			@param	renderer	レンダラー
		*/
		virtual void OnRemoving(Renderer3D* renderer) {}

		virtual void CalculateMatrix_RT();

		virtual void Flip();

		virtual void Rendering(RenderingProperty& prop) = 0;

		Vector3DF GetPosition_RT();

		const Matrix44& GetMatrix_RT();

		const Matrix44& GetLocalMatrix_RT();

		virtual eRenderedObject3DType GetObjectType() const { return RENDERED_OBJECT3D_TYPE_UNKNOWN; }
	};
}