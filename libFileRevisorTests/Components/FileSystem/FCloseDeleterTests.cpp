#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FCloseDeleter.h"

TESTS(FCloseDeleterTests)
AFACT(DefaultConstructor_SetsFCloseFunctionPointer)
AFACT(CallOperator_RawFilePointerIsNullptr_DoesNothing)
AFACT(CallOperator_RawFilePointerIsNotNullptr_FClosesFileWhichReturns0_DoesNotThrowException)
AFACT(CallOperator_RawFilePointerIsNotNullptr_FClosesFileWhichReturnsNon0_ThrowsRuntimeError)
EVIDENCE

FCloseDeleter _fcloseDeleter;
// Function Pointers
METALMOCK_NONVOID1_STATIC_OR_FREE(int, _call_fclose, FILE*)

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

TEST(CallOperator_RawFilePointerIsNullptr_DoesNothing)
{
   _fcloseDeleter(nullptr);
}

TEST(CallOperator_RawFilePointerIsNotNullptr_FClosesFileWhichReturns0_DoesNotThrowException)
{
   _call_fcloseMock.Return(0);
   FILE* const rawFilePointer = tmpfile();
   //
   _fcloseDeleter(rawFilePointer);
   //
   METALMOCK(_call_fcloseMock.CalledOnceWith(rawFilePointer));
}

TEST(CallOperator_RawFilePointerIsNotNullptr_FClosesFileWhichReturnsNon0_ThrowsRuntimeError)
{
   const int fcloseReturnValue = ZenUnit::RandomNon0<int>();
   _call_fcloseMock.Return(fcloseReturnValue);
   FILE* const rawFilePointer = tmpfile();
   //
   const string expectedExceptionMessage = String::ConcatValues(
      "fclose(rawFilePointer) in FCloseDeleter::operator(FILE* rawFilePointer) failed with return value ", fcloseReturnValue);
   THROWS_EXCEPTION(_fcloseDeleter(rawFilePointer),
      runtime_error, expectedExceptionMessage);
   //
   METALMOCK(_call_fcloseMock.CalledOnceWith(rawFilePointer));
}

RUN_TESTS(FCloseDeleterTests)
