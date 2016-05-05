namespace asd
{
	internal class EventToManageObject2DComponent : IRegistrationEvent
	{
		private Object2D Obj { get; }
		private Object2DComponent Component { get; }
		private string Key { get; }
		private RegistrationCommand Command { get; }

		private EventToManageObject2DComponent(
			Object2D obj,
			Object2DComponent component,
			string key,
			RegistrationCommand command)
		{
			Obj = obj;
			Component = component;
			Key = key;
			Command = command;
		}

		public void Manage()
		{
			switch (Command)
			{
			case RegistrationCommand.Add:
				Obj.AddComponent(Component, Key);
				break;

			case RegistrationCommand.Remove:
				Obj.RemoveComponent(Key);
				break;
			}
		}

		public static EventToManageObject2DComponent GetAddEvent(Object2D obj, Object2DComponent component, string key)
		{
			return new EventToManageObject2DComponent(obj, component, key, RegistrationCommand.Add);
		}

		public static EventToManageObject2DComponent GetRemoveEvent(Object2D obj, string key)
		{
			return new EventToManageObject2DComponent(obj, null, key, RegistrationCommand.Remove);
		}
	}
}