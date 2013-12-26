#pragma once

namespace ace
{
	class Scene;

	class SceneComponent
	{
		friend class Scene;

	private:
		Scene* m_scene;
		bool m_isUpdated;
		bool m_isAlive;

		void Update();
		void SetOwner(Scene* value);

	protected:
		virtual void OnUpdate() = 0;

	public:
		SceneComponent();
		virtual ~SceneComponent()
		{
		}

		Scene* GetOwner() const;

		bool GetIsUpdated() const;
		void SetIsUpdated(bool value);

		bool GetIsAlive() const;

		void Vanish();
	};
}
