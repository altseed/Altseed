using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	class EventToManageFamilyship2D : ICommitable
	{
		private Object2D Parent { get; set; }
		private Object2D Child { get; set; }
		private ChildManagementMode ManagementMode { get; set; }
		private ChildTransformingMode TransformingMode { get; set; }
		private RegistrationCommand Command { get; set; }

		public EventToManageFamilyship2D(Object2D parent, Object2D child)
		{
			Parent = parent;
			Child = child;
			Command = RegistrationCommand.Invalid;
		}

		public void SetUpAsAddEvent(
			ChildManagementMode managementMode,
			ChildTransformingMode transformingMode)
		{
			ManagementMode = managementMode;
			TransformingMode = transformingMode;
			Command = RegistrationCommand.Add;
		}

		public void SetUpAsRemoveEvent()
		{
			Command = RegistrationCommand.Remove;
		}

		public virtual void Commit()
		{
			if (!Parent.IsAlive)
			{
				return;
			}

			switch(Command)
			{
			case RegistrationCommand.Add:
				Parent.ImmediatelyAddChild(Child, ManagementMode, TransformingMode);
				break;

			case RegistrationCommand.Remove:
				Parent.ImmediatelyRemoveChild(Child);
				break;

			case RegistrationCommand.Invalid:
				throw new InvalidOperationException("EventToManageFamilyship2D イベントがセットアップされていません。");
			}
		}
	}
}
