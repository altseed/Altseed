

#include "../asd.Core.Base_Imp.h"
#include "asd.Joystick.h"
#include "asd.Joystick_Imp.h"
namespace asd{

	Joystick_Imp::Joystick_Imp(int id, ap::Joystick* joystick)
	{
		this->joystick = joystick;
		m_connectId = id;
		m_joystickName = ToAString("");
	}

	Joystick_Imp* Joystick_Imp::Create(int id, ap::Joystick* joystick)
	{
		return new Joystick_Imp(id,  joystick);
	}

	Joystick_Imp::~Joystick_Imp()
	{

	}

	const asd::achar* Joystick_Imp::GetJoystickName()
	{
		return m_joystickName.c_str();
	}

	JoystickType Joystick_Imp::GetJoystickType() const
	{
		return (JoystickType)joystick->GetJoystickType(m_connectId);
	}

	int Joystick_Imp::GetButtonsCount()
	{
		return joystick->GetButtonCount(m_connectId);
	}

	int Joystick_Imp::GetAxesCount()
	{
		return joystick->GetAxisCount(m_connectId);
	}

	JoystickButtonState Joystick_Imp::GetButtonState(int at)
	{
		return (JoystickButtonState)joystick->GetButtonState(m_connectId, at);
	}

	JoystickButtonState Joystick_Imp::GetButtonStateAt(JoystickButtonType type)
	{
		return (JoystickButtonState)joystick->GetButtonState(m_connectId, (ap::JoystickButtonType)type);
	}

	float Joystick_Imp::GetAxisState(int at)
	{
		return joystick->GetAxisState(m_connectId, at);
	}

	float Joystick_Imp::GetAxisStateAt(JoystickAxisType type)
	{
		return joystick->GetAxisState(m_connectId, (ap::JoystickAxisType)type);
	}
};
