using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal class EventToManageContent<TContent> : IRegistrationEvent
		where TContent : Content
	{
		public EventToManageContent(ContentsManager<TContent> contentManager, TContent content, RegistrationCommand command)
		{
			ContentManager = contentManager;
			Content = content;
			Command = command;
		}

		private ContentsManager<TContent> ContentManager { get; set; }
		public TContent Content { get; set; }
		public RegistrationCommand Command { get; set; }

		public object Owner
		{
			get { return ContentManager; }
		}

		object IRegistrationEvent.Content
		{
			get { return Content; }
		}

		public void Commit()
		{
			switch (Command)
			{
			case RegistrationCommand.Add:
				ContentManager.AddToContents(Content);
				break;

			case RegistrationCommand.Remove:
				ContentManager.RemoveFromContents(Content);
				break;
			}
		}
	}
}
