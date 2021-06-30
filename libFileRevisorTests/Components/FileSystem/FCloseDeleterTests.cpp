#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FCloseDeleter.h"

TESTS(FCloseDeleterTests)
AFACT(DefaultConstructor_SetsFCloseFunctionPointer)
#if defined _WIN32
AFACT(CallOperator_PlaceholderFieldIsNull_DoesNothing)
AFACT(CallOperator_PlaceholderFieldIsNotNull_CallsFCloseOnFilePointerWhichReturns0_Returns)
AFACT(CallOperator_PlaceholderFieldIsNotNull_CallsFCloseOnFilePointerWhichReturnsNon0_ThrowsRuntimeError)
#endif
EVIDENCE

FCloseDeleter _fcloseDeleter;
METALMOCK_NONVOID1_FREE(int, _call_fclose, FILE*)

STARTUP
{
   _fcloseDeleter._call_fclose = BIND_1ARG_METALMOCK_OBJECT(_call_fcloseMock);
}

TEST(DefaultConstructor_SetsFCloseFunctionPointer)
{
   FCloseDeleter fcloseDeleter;
   STD_FUNCTION_TARGETS(fclose, fcloseDeleter._call_fclose);
}

#if defined _WIN32

TEST(CallOperator_PlaceholderFieldIsNull_DoesNothing)
{
   FILE filePointer{};
   IS_NULLPTR(filePointer._Placeholder);
   //
   _fcloseDeleter(&filePointer);
}

TEST(CallOperator_PlaceholderFieldIsNotNull_CallsFCloseOnFilePointerWhichReturns0_Returns)
{
   _call_fcloseMock.Return(0);
   FILE filePointer{};
   filePointer._Placeholder = reinterpret_cast<void*>(0x123);
   //
   _fcloseDeleter(&filePointer);
   //
   METALMOCK(_call_fcloseMock.CalledOnceWith(&filePointer));
}

TEST(CallOperator_PlaceholderFieldIsNotNull_CallsFCloseOnFilePointerWhichReturnsNon0_ThrowsRuntimeError)
{
   const int fcloseReturnValue = ZenUnit::RandomNon0<int>();
   _call_fcloseMock.Return(fcloseReturnValue);
   FILE filePointer{};
   filePointer._Placeholder = reinterpret_cast<void*>(0x123);
   //
   const string expectedExceptionMessage = String::ConcatValues("fclose(filePointer) returned ", fcloseReturnValue);
   THROWS_EXCEPTION(_fcloseDeleter(&filePointer),
      runtime_error, expectedExceptionMessage);
   //
   METALMOCK(_call_fcloseMock.CalledOnceWith(&filePointer));
}

#endif

RUN_TESTS(FCloseDeleterTests)
