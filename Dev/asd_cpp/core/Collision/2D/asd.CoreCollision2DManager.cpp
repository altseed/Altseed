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

		for (auto addedCollision : addedCollisions) {
			delete addedCollision.second;
		}

		for (auto addedCollider : addedColliders) {
			auto colliderImp = CoreCollider2D_Imp::CoreCollider2DToImp(addedCollider);
			colliderImp->SetOwnerObject2D(nullptr);
			colliderImp->SetCollisionManager(nullptr);
			SafeRelease(addedCollider);
		}

	}

	void CoreCollision2DManager::AddCollider(CoreCollider2D* collider) {
		auto colliderImp = CoreCollider2D_Imp::CoreCollider2DToImp(collider);
		if (addedCollidersImp.find(colliderImp) != addedCollidersImp.end()) {
			return;
		}

		addedColliders.insert(collider);
		addedCollidersImp.insert(colliderImp);

		SafeAddRef(collider);

		auto object = culling2d_aabb::Object::Create((void*)colliderImp, cullingWorld);
		object->SetUserData((void*)colliderImp);
		colliderImp->SetCullingObject(object);
		cullingWorld->AddObject(object);
	}

	void CoreCollision2DManager::RemoveCollider(CoreCollider2D* collider) {
		auto colliderImp = CoreCollider2D_Imp::CoreCollider2DToImp(collider);

		if (addedCollidersImp.find(colliderImp) == addedCollidersImp.end()) {
			return;
		}

		addedColliders.erase(collider);
		addedCollidersImp.erase(colliderImp);

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


				auto query = ColliderPair(transformedCollider, elseColliderImp);
				auto targetCollision = addedCollisions.find(query);
				if (targetCollision == addedCollisions.end() && transformedCollider->GetAABB().GetCollision(elseColliderImp->GetAABB())) {
					auto contact = new CoreCollision2D(transformedCollider, elseColliderImp);
					addedCollisions.emplace(query, contact);
				}
				else if(targetCollision != addedCollisions.end() && targetCollision->second->GetIsShouldDestroy()){
					delete targetCollision->second;
					addedCollisions.erase(query);
				}

			}
		}

		// イベント一覧の初期化
		for (auto collider : addedCollidersImp) {
			auto ownerObject2D = collider->GetOwnerObject2D();
			ownerObject2D->currentFrameCollisionEvents.clear();
		}

		// 当たり判定の更新とイベントの積み上げ
		for (auto collision : addedCollisions) {
			collision.second->Update();
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