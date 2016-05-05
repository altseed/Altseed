namespace asd
{
	internal class EventToManageLayer : IRegistrationEvent
	{
		public EventToManageLayer(Scene scene, Layer layer, RegistrationCommand command)
		{
			Scene = scene;
			Layer = layer;
			Command = command;
		}

		private Scene Scene { get; set; }
		private Layer Layer { get; set; }
		private RegistrationCommand Command { get; set; }

		public void Manage()
		{
			switch (Command)
			{
			case RegistrationCommand.Add:
				Scene.AddLayer(Layer);
				break;

			case RegistrationCommand.Remove:
				Scene.RemoveLayer(Layer);
				break;
			}
		}
	}
}
