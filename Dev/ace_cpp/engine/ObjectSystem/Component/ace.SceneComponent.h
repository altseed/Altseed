#pragma once

namespace ace
{
	class SceneComponent
	{
		friend class Scene;

	private:
		Scene* m_scene;

		void Update();
		void SetScene(Scene* value);

	protected:
		virtual void OnUpdate() = 0;

	public:
		SceneComponent();
		virtual ~SceneComponent()
		{
		}

		Scene* GetScene() const;
	};
}