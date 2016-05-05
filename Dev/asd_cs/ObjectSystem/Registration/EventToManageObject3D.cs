namespace asd
{
	internal class EventToAddObject3D : IRegistrationEvent
	{
		private Layer3D Layer { get; set; }
		private Object3D Obj { get; set; }
		private RegistrationCommand Command { get; set; }

		public EventToAddObject3D(Layer3D layer, Object3D obj, RegistrationCommand command)
		{
			Layer = layer;
			Obj = obj;
			Command = command;
		}

		public void Manage()
		{
			switch (Command)
			{
			case RegistrationCommand.Add:
				Layer.AddObject(Obj);
				break;

			case RegistrationCommand.Remove:
				Layer.RemoveObject(Obj);
				break;
			}
		}
	}
}