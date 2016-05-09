using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal class EventToManageObject<TObject> : ICommitable
		where TObject : AltseedObject
	{
		public EventToManageObject(
			ObjectManager<TObject> objectManager,
			TObject content,
			RegistrationCommand command,
			bool raiseEvent)
		{
			ObjectManager = objectManager;
			Content = content;
			Command = command;
			RaiseEvent = raiseEvent;
		}

		private ObjectManager<TObject> ObjectManager { get; set; }
		public TObject Content { get; set; }
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
