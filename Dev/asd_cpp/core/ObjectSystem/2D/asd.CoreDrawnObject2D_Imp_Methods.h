void AddChild(CoreDrawnObject2D* child,
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