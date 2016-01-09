#include <gtest/gtest.h>
#include <Altseed.h>
#include "../EngineTest.h"

using namespace std;
using namespace asd;

class FamilySystem_ChildManagement : public EngineTest
{
	class SpinObject : public TextureObject2D
	{
	public:
		typedef shared_ptr<SpinObject> Ptr;

	protected:
		void OnUpdate()
		{
			SetAngle(GetAngle() + 2);
		}
	};

	class Family
	{
		SpinObject::Ptr parent;
		SpinObject::Ptr child;
		SpinObject::Ptr childNotAdded;
		Vector2DF position;
		ChildManagementMode::Flags managementMode;

	public:
		Family(Vector2DF position, ChildManagementMode::Flags managementMode)
			: managementMode(managementMode)
			, position(position)
		{
		}

		void Initialize()
		{
			parent = make_shared<SpinObject>();
			child = make_shared<SpinObject>();
			childNotAdded = make_shared<SpinObject>();

			auto texture = Engine::GetGraphics()->CreateTexture2D(ToAString("Data/Texture/Cloud1.png").c_str());
			parent->SetTexture(texture);
			parent->SetCenterPosition(texture->GetSize().To2DF() / 2);
			parent->SetScale(Vector2DF(0.5f, 0.5f));
			parent->SetPosition(position);

			child->SetTexture(texture);
			child->SetCenterPosition(texture->GetSize().To2DF() / 2);
			child->SetScale(Vector2DF(0.5f, 0.5f));
			child->SetPosition(position + Vector2DF(0, 100));

			childNotAdded->SetTexture(texture);
			childNotAdded->SetCenterPosition(texture->GetSize().To2DF() / 2);
			childNotAdded->SetScale(Vector2DF(0.5f, 0.5f));
			childNotAdded->SetPosition(position + Vector2DF(0, 200));

			parent->AddChild(child, managementMode, ChildTransformingMode::Nothing);
			parent->AddChild(childNotAdded, managementMode, ChildTransformingMode::Nothing);

			Engine::AddObject2D(parent);
			if ((managementMode & ChildManagementMode::RegistrationToLayer) == 0)
			{
				Engine::AddObject2D(child);
			}
		}

		SpinObject::Ptr& GetParent()
		{
			return parent;
		}
	};

public:
	FamilySystem_ChildManagement(bool isOpenGLMode)
		: EngineTest(asd::ToAString("ChildManagement"), isOpenGLMode, 120)
		, count(0)
	{
	}

protected:
	shared_ptr<Family> nothing, isDrawnChanged, isUpdatedChanged, registrationToLayerChanged, vanished;
	int count;

	void OnStart()
	{
		nothing = make_shared<Family>(Vector2DF(70, 70), ChildManagementMode::Nothing);
		isDrawnChanged = make_shared<Family>(Vector2DF(190, 70), ChildManagementMode::IsDrawn);
		isUpdatedChanged = make_shared<Family>(Vector2DF(310, 70), ChildManagementMode::IsUpdated);
		registrationToLayerChanged = make_shared<Family>(Vector2DF(430, 70), ChildManagementMode::RegistrationToLayer);
		vanished = make_shared<Family>(Vector2DF(550, 70), ChildManagementMode::Vanishment);

		nothing->Initialize();
		isDrawnChanged->Initialize();
		isUpdatedChanged->Initialize();
		registrationToLayerChanged->Initialize();
		vanished->Initialize();
	}

	void OnUpdating()
	{
		++count;
		if (count == 60)
		{
			isDrawnChanged->GetParent()->SetIsDrawn(false);
			isUpdatedChanged->GetParent()->SetIsUpdated(false);
			Engine::RemoveObject2D(registrationToLayerChanged->GetParent());
			vanished->GetParent()->Dispose();
		}
	}
};

ENGINE_TEST(FamilySystem, ChildManagement)