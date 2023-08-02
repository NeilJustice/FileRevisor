#include "pch.h"
#include "libFileRevisor/Components/Utility/Strings/ConstCharPointerGetter.h"

TESTS(ConstCharPointerGetterTests)
AFACT(GetStringConstCharPointer_ReturnsCStringForStdString)
EVIDENCE

ConstCharPointerGetter _constCharPointerGetter;

TEST(GetStringConstCharPointer_ReturnsCStringForStdString)
{
   const string stdString = ZenUnit::Random<string>();
   //
   const char* const constCharPointer = _constCharPointerGetter.GetStringConstCharPointer(stdString);
   //
   ARE_EQUAL(stdString.c_str(), constCharPointer);
}

RUN_TESTS(ConstCharPointerGetterTests)
