
#include "EngineGraphics3DTest.h"

std::shared_ptr<ace::Scene> EngineGraphics3DTest::GetScene()
{
	return m_scene;
}

std::shared_ptr<ace::Layer3D> EngineGraphics3DTest::GetLayer3D()
{
	return m_layer3d;
}

void EngineGraphics3DTest::AddCamera()
{
	auto camera = std::make_shared<ace::CameraObject3D>();
	m_layer3d->AddObject(camera);
	m_cameras.push_back(camera);
}

std::shared_ptr<ace::CameraObject3D> EngineGraphics3DTest::GetCamera(int32_t index)
{
	if (m_cameras.size() <= index) return nullptr;
	return m_cameras[index];
}

void EngineGraphics3DTest::SetCameraParameter(float distance, float rotX, float rotY, float zn, float zf, float fov)
{
	auto cameraObj = GetCamera(0);
	m_cameraDistance = distance;
	m_cameraRotX = rotX;
	m_cameraRotY = rotY;
	
	cameraObj->SetFieldOfView(fov);
	cameraObj->SetZNear(zn);
	cameraObj->SetZFar(zf);
	cameraObj->SetWindowSize(ace::Vector2DI(WindowWidth, WindowHeight));
}

void EngineGraphics3DTest::OnStart()
{
	m_scene = std::make_shared<ace::Scene>();
	m_layer3d = std::make_shared<ace::Layer3D>();
	m_scene->AddLayer(m_layer3d);
	ace::Engine::ChangeScene(m_scene);

	AddCamera();

	{
		auto cameraObj = GetCamera(0);
		cameraObj->SetPosition(ace::Vector3DF(0, 2, 10));
		cameraObj->SetFocus(ace::Vector3DF(0, 0, 0));
		cameraObj->SetFieldOfView(20.0f);
		cameraObj->SetZNear(1.0f);
		cameraObj->SetZFar(20.0f);
		cameraObj->SetWindowSize(ace::Vector2DI(WindowWidth, WindowHeight));
	}

	m_mousePos = ace::Engine::GetMouse()->GetPosition();
}

void EngineGraphics3DTest::OnUpdating()
{
	{
		auto mousePos = ace::Engine::GetMouse()->GetPosition();
		auto d = mousePos - m_mousePos;
		if (ace::Engine::GetMouse()->GetLeftButton()->GetButtonState() == ace::eMouseButtonState::MOUSE_HOLD)
		{
			m_cameraRotX -= d.Y;
			m_cameraRotY += d.X;

			if (m_cameraRotX > 90.0f) m_cameraRotX = 90.0f;
			if (m_cameraRotX < -90.0f) m_cameraRotX = -90.0f;
			if (m_cameraRotY < -180.0f) m_cameraRotY = -180.0f;
			if (m_cameraRotY > 180.0f) m_cameraRotY = 180.0f;
		}

		if (ace::Engine::GetMouse()->GetRightButton()->GetButtonState() == ace::eMouseButtonState::MOUSE_HOLD)
		{
			ace::Vector3DF up(0, 1, 0);
			ace::Vector3DF right(1, 0, 0);

			ace::Matrix44 mat, mat_rot_x, mat_rot_y, look;
			ace::Vector3DF pos(0, 0, m_cameraDistance);
			mat_rot_x.SetRotationX(-m_cameraRotX / 180.0f * PI);
			mat_rot_y.SetRotationY(-m_cameraRotY / 180.0f * PI);
			mat = mat_rot_y * mat_rot_x;
			pos = mat.Transform3D(pos);

			look.SetLookAtRH(m_cameraFocus + pos, m_cameraFocus, up);
			look.SetInverted();
			look.Values[0][3] = 0.0f;
			look.Values[1][3] = 0.0f;
			look.Values[2][3] = 0.0f;

			up = look.Transform3D(up);
			right = look.Transform3D(right);

			auto dx = -d.X / 200.0f * m_cameraDistance;
			auto dy =  d.Y / 200.0f * m_cameraDistance;

			up.X = up.X * (dy);
			up.Y = up.Y * (dy);
			up.Z = up.Z * (dy);
			right.X = right.X * (dx);
			right.Y = right.Y * (dx);
			right.Z = right.Z * (dx);

			ace::Vector3DF v = up + right;
			m_cameraFocus += v;
		}

		if (ace::Engine::GetMouse()->GetMiddleButton()->GetRotation() != 0)
		{
			m_cameraDistance += ace::Engine::GetMouse()->GetMiddleButton()->GetRotation();
		}

		m_mousePos = mousePos;
	}

	if (m_isFreeView)
	{
		ace::Matrix44 mat, mat_rot_x, mat_rot_y;
		ace::Vector3DF pos(0, 0, m_cameraDistance);
		mat_rot_x.SetRotationX(-m_cameraRotX / 180.0f * PI);
		mat_rot_y.SetRotationY(-m_cameraRotY / 180.0f * PI);
		mat = mat_rot_y * mat_rot_x;
		pos = mat.Transform3D(pos);

		pos += m_cameraFocus;

		auto cameraObj = GetCamera(0);
		cameraObj->SetPosition(pos);
		cameraObj->SetFocus(m_cameraFocus);
	}
}

EngineGraphics3DTest::EngineGraphics3DTest(ace::astring title, bool isOpenGLMode, int exitTime, bool isFreeView)
:EngineTest(title, isOpenGLMode, exitTime)
, m_isFreeView(isFreeView)
{
}