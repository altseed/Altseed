using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal interface IRegistrationEvent : ICommitable
	{
		object Owner { get; }
		object Content { get; }
		RegistrationCommand Command { get; }
	}

	internal class RegistrationManager
	{
		private List<IRegistrationEvent> Events { get; set; }

		public RegistrationManager()
		{
			Events = new List<IRegistrationEvent>();
		}

		public bool? Push(IRegistrationEvent registrationEvent)
		{
			var duplicated = Events.FirstOrDefault(x =>
				x.Content == registrationEvent.Content
				&& x.Owner == registrationEvent.Owner);
			if (duplicated != null)
			{
				if (duplicated.Command == RegistrationCommand.Add && registrationEvent.Command == RegistrationCommand.Remove
					|| duplicated.Command == RegistrationCommand.Remove && registrationEvent.Command == RegistrationCommand.Add)
				{
					Events.Remove(duplicated);
					return true;
				}
				return false;
			}

			Events.Add(registrationEvent);
			return null;
		}

		public void Commit()
		{
			foreach (var registrationEvent in Events)
			{
				registrationEvent.Commit();
			}
			Events.Clear();
		}

		public IEnumerable<T> GetActualContents<T>(object owner, IEnumerable<T> source)
		{
			var second = Events.Where(x => x.Content is T)
				.Where(x => x.Owner == owner)
				.ToArray();
			var removed = second.Where(x => x.Command == RegistrationCommand.Remove)
				.Select(x => x.Content)
				.Cast<T>();
			var added = second.Where(x => x.Command == RegistrationCommand.Add)
				.Select(x => x.Content)
				.Cast<T>();
			return source.Except(removed).Concat(added);
		}
	}
}
