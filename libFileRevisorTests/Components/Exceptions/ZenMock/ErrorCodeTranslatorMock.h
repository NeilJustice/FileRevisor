#pragma once
#include "libFileRevisor/Components/Exceptions/ErrorCodeTranslator.h"

class ErrorCodeTranslatorMock : public Zen::Mock<ErrorCodeTranslator>
{
public:
   ZENMOCK_NONVOID0_CONST(int, GetErrnoValue)
   using IntStringPairType = pair<int, string>;
   ZENMOCK_NONVOID0_CONST(IntStringPairType, GetErrnoWithDescription)
   ZENMOCK_NONVOID1_CONST(string, GetErrnoDescription, int)
   ZENMOCK_NONVOID1_CONST(string, GetSystemErrorDescription, int)

#ifdef _WIN32
   using DWORDStringPairType = pair<DWORD, string>;
   ZENMOCK_NONVOID0_CONST(DWORDStringPairType, GetWindowsLastErrorWithDescription)
   ZENMOCK_NONVOID1_CONST(string, GetWindowsLastErrorDescription, DWORD)
#endif
};
