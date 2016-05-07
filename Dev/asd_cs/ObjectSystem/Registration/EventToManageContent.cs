using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal class EventToManageContent<TLayer> : ICommitable
		where TLayer : Layer
	{
		public EventToManageContent(
			ObjectManager<TLayer> objectManager,
			AltseedObject<TLayer> content,
			RegistrationCommand command,
			bool raiseEvent)
		{
			ObjectManager = objectManager;
			Content = content;
			Command = command;
			RaiseEvent = raiseEvent;
		}

		private ObjectManager<TLayer> ObjectManager { get; set; }
		public AltseedObject<TLayer> Content { get; set; }
		public RegistrationCommand Command { get; set; }
		public bool RaiseEvent { get; set; }

		public void Commit()
		{
			switch (Command)
			{
			case RegistrationCommand.Add:
				ObjectManager.AddToContents(Content, RaiseEvent);
				break;

			case RegistrationCommand.Remove:
				ObjectManager.RemoveFromContents(Content, RaiseEvent);
				break;
			}
		}
	}
}
