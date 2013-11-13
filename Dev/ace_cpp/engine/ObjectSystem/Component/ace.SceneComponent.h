#pragma once

namespace ace
{
	class Scene;

	class SceneComponent
	{
		friend class Scene;

	private:
		Scene* m_scene;

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
	};
}
