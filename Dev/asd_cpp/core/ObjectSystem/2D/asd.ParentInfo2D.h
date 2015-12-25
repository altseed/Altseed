#pragma once
#include <memory>
#include "asd.CoreObject2D.h"
#include "../asd.ChildTransformingMode.h"
#include "../asd.ChildManagementMode.h"
#include "../common/Graphics/asd.Color.h"

namespace asd
{
	class ParentInfo2D
	{
	protected:
		CoreObject2D* m_parent;
		ChildTransformingMode m_transformingMode;
		ChildManagementMode::Flags m_managementMode;

	public:
		typedef std::shared_ptr<ParentInfo2D> Ptr;

		ParentInfo2D(CoreObject2D* parent, ChildManagementMode::Flags managementMode, ChildTransformingMode transformingMode)
			: m_parent(parent)
			, m_transformingMode(transformingMode)
			, m_managementMode(managementMode)
		{
			assert(parent != nullptr);
			SafeAddRef(parent);
		}

		~ParentInfo2D()
		{
			SafeRelease(m_parent);
		}

		CoreObject2D& GetParent() const
		{
			return *m_parent;
		}

		ChildTransformingMode GetChildTransformingMode() const
		{
			return m_transformingMode;
		}

		void SetChildTransformingMode(ChildTransformingMode mode)
		{
			m_transformingMode = mode;
		}

		/**
			@brief	親オブジェクトから継承される変換行列があれば返す。無ければ単位行列を返す。
		*/
		virtual Matrix33 GetInheritedMatrix() const
		{
			switch (m_transformingMode)
			{
			case ChildTransformingMode::Position:
				return m_parent->GetParentsMatrix() * m_parent->GetMatrixToTranslate();

			case ChildTransformingMode::All:
				return m_parent->GetParentsMatrix() * m_parent->GetMatrixToTransform();

			case ChildTransformingMode::Nothing:
				return m_parent->GetParentsMatrix() * Matrix33();
			}
			ACE_ASSERT(false, "不正なTransformingModeが指定されました。");
		}

		/**
			@brief	親オブジェクトから継承される描画フラグの値があれば返す。無ければ true を返す。
		*/
		virtual bool GetInheritedBeingDrawn() const
		{
			return (m_managementMode & ChildManagementMode::IsDrawn == 0) || m_parent->GetIsDrawn();
		}

		/**
			@brief	親オブジェクトから継承される色があればそれを返す。無ければ白色を返す。
		*/
		virtual Color GetInheritedColor() const
		{
			return Color(255, 255, 255, 255);
		}

		/**
			@brief	親オブジェクトから継承される描画優先度があればそれを返す。無ければ 0 を返す。
		*/
		virtual int GetInheritedDrawingPriority() const
		{
			return 0;
		}
	};
}