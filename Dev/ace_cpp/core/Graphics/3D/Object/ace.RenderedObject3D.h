
#pragma once

#include <ace.common.Base.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Matrix44.h>
#include <Graphics/ace.Color.h>

#include "../../../ace.Core.Base.h"
#include "../../../ace.Core.Base_Imp.h"

#include "../../../ace.ReferenceObject.h"


namespace ace
{
	class RenderingProperty
	{
	public:
		bool		IsLightweightMode = false;
		bool		IsDepthMode = false;

		Matrix44	CameraMatrix;
		Matrix44	ProjectionMatrix;

		std::shared_ptr<Texture2D>	DummyTextureWhite;
		std::shared_ptr<Texture2D>	DummyTextureBlack;
		std::shared_ptr<Texture2D>	DummyTextureNormal;

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
		@brief	描画スレッド上のRenderedObject3D
		@note
		実際に描画スレッド上で描画に使用される。
	*/
	class RenderedObject3DProxy
		: public ReferenceObject
	{
		friend class RenderedObject3D;

	private:
		bool		isSRTChanged = false;
		Matrix44	localMatrix;
	public:
		Vector3DF	Position;
		Vector3DF	Rotation;
		Vector3DF	Scale;
		
		const Matrix44& GetLocalMatrix();

		// 将来的に親子関係実装
		const Matrix44& GetGlobalMatrix();
		Vector3DF GetGlobalPosition();

		virtual void OnUpdateAsync() {}

		virtual void Rendering(RenderingCommandHelper* helper, RenderingProperty& prop) {}
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
		friend class RenderedObject3DProxy;
	private:
		Graphics_Imp*	m_graphics;

		Vector3DF	position;
		Vector3DF	rotation;
		Vector3DF	scale;
		Matrix44	localMatrix;
		bool		isSRTChanged;
		
		static Matrix44 CalcLocalMatrix(Vector3DF& t, Vector3DF& r, Vector3DF& s);

	protected:
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

		const Matrix44& GetLocalMatrix();

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

		virtual RenderedObject3DProxy* GetProxy() const = 0;

		virtual void Flip();

		virtual eRenderedObject3DType GetObjectType() const { return RENDERED_OBJECT3D_TYPE_UNKNOWN; }
	};
}