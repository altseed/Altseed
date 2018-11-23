#include "asd.MediaPlayer.h"

namespace asd
{

bool MediaPlayer::GetIsLoopingMode() const
{
	return isLoopingMode;
}

void MediaPlayer::SetIsLoopingMode(bool isLoopingMode)
{
	this->isLoopingMode = isLoopingMode;
}

}