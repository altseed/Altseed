namespace asd
{
	internal class EventToManageSceneComponent : IRegistrationEvent
	{
		private Scene Scene { get; }
		private SceneComponent Component { get; }
		private string Key { get; }
		private RegistrationCommand Command { get; }

		private EventToManageSceneComponent(Scene scene, SceneComponent component, string key, RegistrationCommand command)
		{
			Scene = scene;
			Component = component;
			Key = key;
			Command = command;
		}

		public void Manage()
		{
			switch (Command)
			{
			case RegistrationCommand.Add:
				Scene.AddComponent(Component, Key);
				break;

			case RegistrationCommand.Remove:
				Scene.RemoveComponent(Key);
				break;
			}
		}

		public static EventToManageSceneComponent GetAddEvent(Scene scene, SceneComponent component, string key)
		{
			return new EventToManageSceneComponent(scene, component, key, RegistrationCommand.Add);
		}

		public static EventToManageSceneComponent GetRemoveEvent(Scene scene, string key)
		{
			return new EventToManageSceneComponent(scene, null, key, RegistrationCommand.Remove);
		}
	}
}