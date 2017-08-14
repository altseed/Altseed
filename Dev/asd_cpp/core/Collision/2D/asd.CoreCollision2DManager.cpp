#include "asd.CoreCollision2DManager.h"
#include "Culling2D_AABB.h"
#include "asd.CoreCollider2D.h"
#include "asd.CoreCollider2D_Imp.h"

#if defined(_WIN32)

#ifdef _WIN64

#if _DEBUG
#pragma comment(lib,"x64/Debug/Culling2D_AABB_debug.lib")
#else
#pragma comment(lib,"x64/Release/Culling2D_AABB.lib")
#endif

#else

#if _DEBUG
#pragma comment(lib,"x86/Debug/Culling2D_AABB_debug.lib")
#else
#pragma comment(lib,"x86/Release/Culling2D_AABB.lib")
#endif

#endif

#endif

namespace asd {

	CoreCollision2DManager::CoreCollision2DManager() {
		cullingWorld = new culling2d_aabb::World(6, culling2d::RectF(-5000, -5000, 10000, 10000));

	}

	CoreCollision2DManager::~CoreCollision2DManager() {
		culling2d::SafeDelete(cullingWorld);

		for (auto registeredCollision : registeredCollisions) {
			CoreCollision2D::Destroy(registeredCollision);
		}

		for (auto registeredCollider : registeredColliders) {
			SafeRelease(registeredCollider);
		}

	}

	void CoreCollision2DManager::RegisterCollider(CoreCollider2D* collider) {
		auto colliderImp = CoreCollider2D_Imp::CoreCollider2DToImp(collider);
		if (registeredCollidersImp.find(colliderImp) != registeredCollidersImp.end()) {
			return;
		}

		registeredColliders.insert(collider);
		registeredCollidersImp.insert(colliderImp);

		SafeAddRef(collider);

		auto object = culling2d_aabb::Object::Create((void*)colliderImp, cullingWorld);
		object->SetUserData((void*)colliderImp);
		colliderImp->SetCullingObject(object);
		cullingWorld->AddObject(object);
	}

	void CoreCollision2DManager::UnregisterCollider(CoreCollider2D* collider) {
		auto colliderImp = CoreCollider2D_Imp::CoreCollider2DToImp(collider);

		registeredColliders.erase(collider);
		registeredCollidersImp.erase(colliderImp);

		SafeRelease(collider);

		auto cullingObject = colliderImp->GetCullingObject();
		cullingWorld->RemoveObject(cullingObject);
	}

	void CoreCollision2DManager::UpdateCollisionState() {

		// カリング向けAABBの更新
		for (auto transformedCollider : lastTransformedColliders) {
			transformedCollider->Update();

			auto cullingObject = transformedCollider->GetCullingObject();
			auto aabb = transformedCollider->GetAABB();
			cullingObject->SetAABB(aabb);
		}

		// カリングの更新
		cullingWorld->Update();

		// 衝突情報の作成と削除
		for (auto transformedCollider : lastTransformedColliders) {

			auto culledObjects = cullingWorld->GetCullingObjects(transformedCollider->GetAABB());
			for (auto culledObject : culledObjects) {
				auto elseColliderImp = (CoreCollider2D_Imp*)culledObject->GetUserData();
				if (transformedCollider == elseColliderImp)
					continue;

				CoreCollision2D* targetCollision = nullptr;

				for (auto registeredCollision : registeredCollisions) {

					auto colliderAImp = registeredCollision->GetColliderA_Imp();
					auto colliderBImp = registeredCollision->GetColliderB_Imp();

					if (
						colliderAImp->GetOwnerObject2D() != colliderBImp->GetOwnerObject2D() &&
						(colliderAImp == transformedCollider &&
						colliderBImp == elseColliderImp ||
						colliderBImp == transformedCollider &&
						colliderAImp == elseColliderImp)
						) {
						targetCollision = registeredCollision;
						break;
					}
				}
				if (targetCollision == nullptr && transformedCollider->GetAABB().GetCollision(elseColliderImp->GetAABB())) {
					auto contact = CoreCollision2D::Create(transformedCollider, elseColliderImp);
					registeredCollisions.insert(contact);
				}
				else if(targetCollision != nullptr && targetCollision->GetIsShouldDestroy()){
					registeredCollisions.erase(targetCollision);
					CoreCollision2D::Destroy(targetCollision);
				}

			}
		}

		// イベント一覧の初期化
		for (auto collider : registeredCollidersImp) {
			auto ownerObject2D = collider->GetOwnerObject2D();
			ownerObject2D->currentFrameCollisionEvents.clear();
		}

		// 当たり判定の更新とイベントの積み上げ
		for (auto collision : registeredCollisions) {
			collision->Update();
		}

		//変形したオブジェクト一覧の初期化
		lastTransformedColliders.clear();
	}

	void CoreCollision2DManager::NotifyLastTransformed(CoreCollider2D_Imp* colliderImp) {
		if (colliderImp != nullptr) {
			lastTransformedColliders.insert(colliderImp);
		}
	}
}