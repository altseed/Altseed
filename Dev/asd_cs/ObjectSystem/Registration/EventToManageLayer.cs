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
		public RegistrationCommand Command { get; set; }

		public object Owner
		{
			get { return Scene; }
		}

		public object Content
		{
			get { return Layer; }
		}

		public void Commit()
		{
			if (!Scene.IsAlive)
			{
				return;
			}

			switch (Command)
			{
			case RegistrationCommand.Add:
				Scene.DirectlyAddLayer(Layer);
				break;

			case RegistrationCommand.Remove:
				Scene.DirectlyRemoveLayer(Layer);
				break;
			}
		}
	}
}
