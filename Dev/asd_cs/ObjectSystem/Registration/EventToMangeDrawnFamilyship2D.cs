using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	class EventToMangeDrawnFamilyship2D : ICommitable
	{
		private DrawnObject2D Parent { get; set; }
		private DrawnObject2D Child { get; set; }
		private ChildManagementMode ManagementMode { get; set; }
		private ChildTransformingMode TransformingMode { get; set; }
		private ChildDrawingMode DrawingMode { get; set; }
		private RegistrationCommand Command { get; set; }

		public EventToMangeDrawnFamilyship2D(DrawnObject2D parent, DrawnObject2D child)
		{
			Parent = parent;
			Child = child;
			Command = RegistrationCommand.Invalid;
		}

		public void SetUpAsAddEvent(
			ChildManagementMode managementMode,
			ChildTransformingMode transformingMode,
			ChildDrawingMode drawingMode)
		{
			ManagementMode = managementMode;
			DrawingMode = drawingMode;
			TransformingMode = transformingMode;
			Command = RegistrationCommand.Add;
		}

		public void SetUpAsRemoveEvent()
		{
			Command = RegistrationCommand.Remove;
		}
		
		public void Commit()
		{
			if (!Parent.IsAlive)
			{
				return;
			}

			if(Command == RegistrationCommand.Add)
			{
				Parent.ImmediatelyAddDrawnChild(Child, ManagementMode, TransformingMode, DrawingMode);
			}
			else if(Command == RegistrationCommand.Remove)
			{
				Parent.ImmediatelyRemoveChild(Child);
			}
			else if(Command == RegistrationCommand.Invalid)
			{
				throw new InvalidOperationException("EventToMangeDrawnFamilyship2D イベントがセットアップされていません。");
			}
		}
	}
}
