using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal class EventToManageComponent<TComponent> : ICommitable
		where TComponent : Component
	{
		public EventToManageComponent()
		{
			Command = RegistrationCommand.Invalid;
		}

		private IImmediateComponentmanager<TComponent> Manager { get; set; }
		private TComponent Component { get; set; }
		private string Key { get; set; }
		private RegistrationCommand Command { get; set; }

		public void Commit()
		{
			if(Command == RegistrationCommand.Add)
			{
				Manager.ImmediatelyAddComponent(Component, Key);
			}
			else if (Command == RegistrationCommand.Remove)
			{
				Manager.ImmediatelyRemoveComponent(Key);
			}
		}

		public void SetUpAsAddEvent(
			ComponentManager<TComponent> manager,
			TComponent component,
			string key)
		{
			Manager = manager;
			Component = component;
			Key = key;
			Command = RegistrationCommand.Add;
		}

		public void SetUpAsRemoveEvent(
			ComponentManager<TComponent> manager,
			string key)
		{
			Manager = manager;
			Key = key;
			Command = RegistrationCommand.Remove;
		} 
	}
}
