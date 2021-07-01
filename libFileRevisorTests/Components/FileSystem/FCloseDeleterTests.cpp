#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FCloseDeleter.h"

TESTS(FCloseDeleterTests)
AFACT(DefaultConstructor_SetsFCloseFunctionPointer)
#if defined __linux__ || defined __APPLE__
AFACT(CallOperator_FILEFilenoFieldIs0_DoesNothing)
AFACT(CallOperator_FILEFilenoFieldIsNot0_CallsFCloseOnFilePointerWhichReturns0_DoesNotThrowRuntimeError)
AFACT(CallOperator_FILEFilenoFieldIsNot0_CallsFCloseOnFilePointerWhichReturnsNon0_ThrowsRuntimeError)
#elif defined _WIN32
AFACT(CallOperator_FILEPlaceholderFieldIsNull_DoesNothing)
AFACT(CallOperator_FILEPlaceholderFieldIsNotNull_CallsFCloseOnFilePointerWhichReturns0_DoesNotThrowRuntimeError)
AFACT(CallOperator_FILEPlaceholderFieldIsNotNull_CallsFCloseOnFilePointerWhichReturnsNon0_ThrowsRuntimeError)
#endif
EVIDENCE

FCloseDeleter _fcloseDeleter;
// Function Pointers
METALMOCK_NONVOID1_FREE(int, _call_fclose, FILE*)

STARTUP
{
   // Function Pointers
   _fcloseDeleter._call_fclose = BIND_1ARG_METALMOCK_OBJECT(_call_fcloseMock);
}

TEST(DefaultConstructor_SetsFCloseFunctionPointer)
{
   FCloseDeleter fcloseDeleter;
   // Function Pointers
   STD_FUNCTION_TARGETS(fclose, fcloseDeleter._call_fclose);
}

#if defined __linux__ || defined __APPLE__

TEST(CallOperator_FILEFilenoFieldIs0_DoesNothing)
{
   FILE filePointer{};
   IS_ZERO(filePointer._fileno);
   //
   _fcloseDeleter(&filePointer);
}

TEST(CallOperator_FILEFilenoFieldIsNot0_CallsFCloseOnFilePointerWhichReturns0_DoesNotThrowRuntimeError)
{

}

TEST(CallOperator_FILEFilenoFieldIsNot0_CallsFCloseOnFilePointerWhichReturnsNon0_ThrowsRuntimeError)
{

}

#elif defined _WIN32

TEST(CallOperator_FILEPlaceholderFieldIsNull_DoesNothing)
{
   FILE filePointer{};
   IS_NULLPTR(filePointer._Placeholder);
   //
   _fcloseDeleter(&filePointer);
}

TEST(CallOperator_FILEPlaceholderFieldIsNotNull_CallsFCloseOnFilePointerWhichReturns0_DoesNotThrowRuntimeError)
{
   _call_fcloseMock.Return(0);
   FILE filePointer{};
   filePointer._Placeholder = reinterpret_cast<void*>(0x123);
   //
   _fcloseDeleter(&filePointer);
   //
   METALMOCK(_call_fcloseMock.CalledOnceWith(&filePointer));
}

TEST(CallOperator_FILEPlaceholderFieldIsNotNull_CallsFCloseOnFilePointerWhichReturnsNon0_ThrowsRuntimeError)
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
