#pragma once

namespace ace
{
	class SceneComponent
	{
		friend class Scene;

	private:
		void Update();

	protected:
		virtual void OnUpdate() = 0;

	public:
		virtual ~SceneComponent()
		{
		}
	};
}