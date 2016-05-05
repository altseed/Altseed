namespace asd
{
	internal class EventToManageObject2D : IRegistrationEvent
	{
		private Layer2D Layer { get; }
		private Object2D Obj { get; }
		private RegistrationCommand Command { get; }

		public EventToManageObject2D(Layer2D layer, Object2D obj, RegistrationCommand command)
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