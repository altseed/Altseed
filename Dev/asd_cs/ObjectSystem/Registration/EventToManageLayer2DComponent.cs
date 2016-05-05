namespace asd
{
	internal class EventToManageLayer2DComponent : IRegistrationEvent
	{
		private Layer2D Layer { get; }
		private Layer2DComponent Component { get; }
		private string Key { get; }
		private RegistrationCommand Command { get; }

		private EventToManageLayer2DComponent(
			Layer2D layer,
			Layer2DComponent component,
			string key,
			RegistrationCommand command)
		{
			Layer = layer;
			Component = component;
			Key = key;
			Command = command;
		}

		public void Manage()
		{
			switch (Command)
			{
			case RegistrationCommand.Add:
				Layer.AddComponent(Component, Key);
				break;

			case RegistrationCommand.Remove:
				Layer.RemoveComponent(Key);
				break;
			}
		}

		public static EventToManageLayer2DComponent GetAddEvent(Layer2D layer, Layer2DComponent component, string key)
		{
			return new EventToManageLayer2DComponent(layer, component, key, RegistrationCommand.Add);
		}

		public static EventToManageLayer2DComponent GetRemoveEvent(Layer2D layer, string key)
		{
			return new EventToManageLayer2DComponent(layer, null, key, RegistrationCommand.Remove);
		}
	}
}