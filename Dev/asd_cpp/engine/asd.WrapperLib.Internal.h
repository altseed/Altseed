#pragma once

#include <stdio.h>
#include <stdint.h>
#include <memory>
#include <asd.common.Base.h>

typedef void*(ASD_STDCALL *CreateWrapperDLLFunc)();
typedef void(ASD_STDCALL *DeleteWrapperDLLFunc)(void*);

namespace asd {

void InitializeWrapper(CreateWrapperDLLFunc func);
void TerminateWrapper(DeleteWrapperDLLFunc func);

}
