
#include "Animation.h"

void AnimationSource::WriteAnimationSource(ace::BinaryWriter *binaryWriter)
{
    binaryWriter->Push(ace::ToAString(animationName.c_str()));

    binaryWriter->Push((int32_t)keyFrameAnimations.size());
    for(int i=0;i<keyFrameAnimations.size();++i)
    {
        binaryWriter->Push(ace::ToAString(keyFrameAnimations[i].targetName.c_str()));

        binaryWriter->Push((int32_t)keyFrameAnimations[i].keyFrames.size());

        for(int j=0;j<keyFrameAnimations[i].keyFrames.size();++j)
        {
            binaryWriter->Push(keyFrameAnimations[i].keyFrames[j].keyValue);
            binaryWriter->Push(keyFrameAnimations[i].keyFrames[j].leftPosition);
            binaryWriter->Push(keyFrameAnimations[i].keyFrames[j].rightPosition);
            binaryWriter->Push(keyFrameAnimations[i].keyFrames[j].interpolation);
        }

    }
}
