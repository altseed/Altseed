﻿namespace asd
{
	internal class EventToManageLayer : ICommitable
	{
		public EventToManageLayer(Scene scene, Layer layer, RegistrationCommand command, bool raiseEvent)
		{
			Scene = scene;
			Layer = layer;
			Command = command;
			RaiseEvent = raiseEvent;
		}

		private Scene Scene { get; set; }
		private Layer Layer { get; set; }
		public RegistrationCommand Command { get; set; }
		private bool RaiseEvent { get; set; }

		public void Commit()
		{
			if (!Scene.IsAlive)
			{
				return;
			}

			switch (Command)
			{
			case RegistrationCommand.Add:
				Scene.ImmediatelyAddLayer(Layer, RaiseEvent);
				break;

			case RegistrationCommand.Remove:
				Scene.ImmediatelyRemoveLayer(Layer, RaiseEvent);
				break;
			}
		}
	}
}
