int AddRef() { return ReferenceObject::AddRef(); }
int Release() { return ReferenceObject::Release(); }
int GetRef() { return ReferenceObject::GetRef(); }

virtual int32_t GetCameraGroup() const override { return CoreObject2D_Imp::GetCameraGroup(); }
virtual void SetCameraGroup(int32_t group) override { CoreObject2D_Imp::SetCameraGroup(group); }

virtual bool GetIsDrawn() const override { return CoreObject2D_Imp::GetIsDrawn(); }
virtual void SetIsDrawn(bool value) override { return CoreObject2D_Imp::SetIsDrawn(value); }
virtual bool GetIsAlive() const override { return CoreObject2D_Imp::GetIsAlive(); }
virtual void SetIsAlive(bool value) override { return CoreObject2D_Imp::SetIsAlive(value); }
virtual void SetLayer(CoreLayer2D* layer) override { return CoreObject2D_Imp::SetLayer(layer); }
virtual CoreLayer2D* GetLayer() override { return CoreObject2D_Imp::GetLayer(); }


virtual Vector2DF GetPosition() const override { return CoreObject2D_Imp::GetPosition(); }
virtual void SetPosition(Vector2DF value) override { return CoreObject2D_Imp::SetPosition(value); }
virtual Vector2DF GetGlobalPosition() override { return CoreObject2D_Imp::GetGlobalPosition(); }
virtual float GetAngle() const override { return CoreObject2D_Imp::GetAngle(); }
virtual void SetAngle(float value) override { return CoreObject2D_Imp::SetAngle(value); }
virtual Vector2DF GetScale() const override { return CoreObject2D_Imp::GetScale(); }
virtual void SetScale(Vector2DF value) override { return CoreObject2D_Imp::SetScale(value); }

virtual Matrix33 GetMatrixToTranslate() override { return CoreObject2D_Imp::GetMatrixToTranslate(); }
virtual Matrix33 GetMatrixToTransform() override { return CoreObject2D_Imp::GetMatrixToTransform(); }
virtual Matrix33 GetParentsMatrix() override { return CoreObject2D_Imp::GetParentsMatrix(); }

virtual void SetParent(CoreObject2D& parent, ChildManagementMode::Flags managementMode, ChildTransformingMode transformingMode) override
{
	return CoreObject2D_Imp::SetParent(parent, managementMode, transformingMode);
}
virtual void ClearParent() override { return CoreObject2D_Imp::ClearParent(); }
virtual const ParentInfo2D* GetParentInfo() const override { return CoreObject2D_Imp::GetParentInfo(); }

void AddChild(CoreObject2D* child, ChildManagementMode::Flags managementMode, ChildTransformingMode transformingMode)
{
	if (child == nullptr || children.find(child) != children.end()) return;
	children.insert(child);
	child->SetParent(*this, managementMode, transformingMode);
}

void RemoveChild(CoreObject2D* child)
{
	if (child == nullptr) return;
	children.erase(child);
	child->ClearParent();
}

Vector2DF GetAbsolutePosition() override { return CoreObject2D_Imp::GetAbsolutePosition(); }
Matrix33 GetAbsoluteMatrixToTranslate() override { return CoreObject2D_Imp::GetAbsoluteMatrixToTranslate(); }
Matrix33 GetAbsoluteMatrixToTransform() override { return CoreObject2D_Imp::GetAbsoluteMatrixToTransform(); }
bool GetAbsoluteBeingDrawn() const override { return CoreObject2D_Imp::GetAbsoluteBeingDrawn(); }
