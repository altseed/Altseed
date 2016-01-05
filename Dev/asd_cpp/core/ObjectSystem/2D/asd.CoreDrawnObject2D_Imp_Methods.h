void AddDrawnChild(CoreDrawnObject2D* child,
	ChildManagementMode::Flags managementMode,
	ChildTransformingMode transformingMode,
	ChildDrawingMode::Flags drawingMode)
{
	if (child == nullptr || children.find(child) != children.end()) return;
	children.insert(child);
	child->SetParent(*this, managementMode, transformingMode, drawingMode);
}

void SetParent(CoreDrawnObject2D& parent,
	ChildManagementMode::Flags managementMode,
	ChildTransformingMode transformingMode,
	ChildDrawingMode::Flags drawingMode)
{
	m_parentInfo = std::make_shared<DrawnParentInfo2D>(&parent, managementMode, transformingMode, drawingMode);
}

Color GetColor() const override { return CoreDrawnObject2D_Imp::GetColor(); }
void SetColor(Color value) override { CoreDrawnObject2D_Imp::SetColor(value); }

int GetDrawingPriority() const override { return CoreDrawnObject2D_Imp::GetDrawingPriority(); }
void SetDrawingPriority(int value) override { CoreDrawnObject2D_Imp::SetDrawingPriority(value); }

Color GetAbsoluteColor() const override { return CoreDrawnObject2D_Imp::GetAbsoluteColor(); }
int GetAbsoluteDrawingPriority() const override { return CoreDrawnObject2D_Imp::GetAbsoluteDrawingPriority(); }