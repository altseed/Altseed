

#include "../asd.Core.Base_Imp.h"
#include "asd.Joystick.h"
#include "asd.Joystick_Imp.h"
namespace asd{

	Joystick_Imp::Joystick_Imp(int id)
	{
		m_connectId = id;
		m_buttonsCount = 0;
		m_axesCount = 0;
		m_joystickName = ToAString("");

		for (int i = 0; i < MAX_BUTTONS_NUM; ++i)
		{
			m_currentButtonHit[i] = false;
			m_preButtonHit[i] = false;
		}

		for (int i = 0; i < MAX_AXES_NUM; ++i)
		{
			m_currentAxes[i] = 0;
		}
	}

	Joystick_Imp* Joystick_Imp::Create(int id)
	{
		return new Joystick_Imp(id);
	}

	Joystick_Imp::~Joystick_Imp()
	{

	}

	void Joystick_Imp::RefreshInputState()
	{

		const float* ax = glfwGetJoystickAxes(m_connectId, &m_axesCount);
		for (int i = 0; i < m_axesCount; ++i)
		{
			m_currentAxes[i] = ax[i];
		}

		const unsigned char* btns = glfwGetJoystickButtons(m_connectId, &m_buttonsCount);
		for (int i = 0; i < m_buttonsCount; ++i)
		{
			m_preButtonHit[i] = m_currentButtonHit[i];
			m_currentButtonHit[i] = (bool)btns[i];
		}
	}

	const asd::achar* Joystick_Imp::GetJoystickName()
	{

		m_joystickName = ToAString(glfwGetJoystickName(m_connectId));
		return m_joystickName.c_str();
	}

	int Joystick_Imp::GetButtonsCount()
	{
		return m_buttonsCount;
	}


	int Joystick_Imp::GetAxesCount()
	{
		return m_axesCount;
	}


	JoystickButtonState Joystick_Imp::GetButtonState(int at)
	{
		if (m_currentButtonHit[at] && m_preButtonHit[at]) return JoystickButtonState::Hold;
		else if (m_currentButtonHit[at] && !m_preButtonHit[at]) return JoystickButtonState::Push;
		else if (!m_currentButtonHit[at] && m_preButtonHit[at]) return JoystickButtonState::Release;
		else return JoystickButtonState::Free;
	}


	float Joystick_Imp::GetAxisState(int at)
	{
		return m_currentAxes[at];
	}
};
