#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileOpenerCloser.h"
#include "libFileRevisorTests/Components/ErrorHandling/MetalMock/ErrorCodeTranslatorMock.h"

TESTS(FileOpenerCloserTests)
AFACT(DefaultConstructor_SetsFunctionPointers)
AFACT(OpenReadModeBinaryFile_ReturnsFilePointerOpenedInBinaryReadMode)
AFACT(CloseFile_CallsFCloseOnFilePointerWhichReturns0_Returns)
AFACT(CloseFile_CallsFCloseOnFilePointerWhichReturnsNon0_ThrowsRuntimeError)
// Private Functions
AFACT(ThrowFileOpenExceptionIfFileOpenFailed_FilePointerIsNullptr_ThrowIfFileNotOpenableIsFalse_DoesNothing)
AFACT(ThrowFileOpenExceptionIfFileOpenFailed_FilePointerIsNullptr_ThrowIfFileNotOpenableIsTrue_ThrowsFileOpenException)
AFACT(ThrowFileOpenExceptionIfFileOpenFailed_FilePointerIsNotNullptr_ThrowIfFileNotOpenableIsTrueOrFalse_DoesNothing)
EVIDENCE

FileOpenerCloser _fileOpenerCloser;
// Function Pointers
METALMOCK_NONVOID1_FREE(int, fclose, FILE*)
#if defined __linux__ || defined __APPLE__
METALMOCK_NONVOID2_FREE(FILE*, fopen, const char*, const char*)
#elif _WIN32
METALMOCK_NONVOID3_FREE(FILE*, _wfsopen, const wchar_t*, const wchar_t*, int)
#endif
// Constant Components
ErrorCodeTranslatorMock* _errorCodeTranslatorMock = nullptr;

STARTUP
{
   // Function Pointers
   _fileOpenerCloser._call_fclose = BIND_1ARG_METALMOCK_OBJECT(fcloseMock);
#if defined __linux__ || defined __APPLE__
   _fileOpenerCloser._call_fopen = BIND_2ARG_METALMOCK_OBJECT(fopenMock);
#elif _WIN32
   _fileOpenerCloser._call_wfsopen = BIND_3ARG_METALMOCK_OBJECT(_wfsopenMock);
#endif
   // Constant Components
   _fileOpenerCloser._errorCodeTranslator.reset(_errorCodeTranslatorMock = new ErrorCodeTranslatorMock);
}

TEST(DefaultConstructor_SetsFunctionPointers)
{
   const FileOpenerCloser fileOpenerCloser;
   STD_FUNCTION_TARGETS(::fclose, fileOpenerCloser._call_fclose);
#if defined __linux__ || defined __APPLE__
   STD_FUNCTION_TARGETS(fopen, fileOpenerCloser._call_fopen);
#elif _WIN32
   STD_FUNCTION_TARGETS(_wfsopen, fileOpenerCloser._call_wfsopen);
#endif
}

TEST(OpenReadModeBinaryFile_ReturnsFilePointerOpenedInBinaryReadMode)
{
   FILE readModeBinaryFilePointer{};
#if defined __linux__ || defined __APPLE__
   fopenMock.Return(&readModeBinaryFilePointer);
#elif _WIN32
   _wfsopenMock.Return(&readModeBinaryFilePointer);
#endif
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const bool skipFilesInUse = ZenUnit::Random<bool>();
   //
   const shared_ptr<FILE> returnedReadModeBinaryFilePointer = _fileOpenerCloser.OpenReadModeBinaryFile(filePath, skipFilesInUse);
   //
#if defined __linux__ || defined __APPLE__
   METALMOCK(fopenMock.CalledOnceWith(filePath.c_str(), "rb"));
#elif _WIN32
   METALMOCK(_wfsopenMock.CalledOnceWith(filePath.c_str(), L"rb", _SH_DENYWR));
#endif
   ARE_EQUAL(&readModeBinaryFilePointer, returnedReadModeBinaryFilePointer.get());
}

TEST(CloseFile_CallsFCloseOnFilePointerWhichReturns0_Returns)
{
   fcloseMock.Return(0);
   FILE FilePointer{};
   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   _fileOpenerCloser.CloseFile(&FilePointer, filePath);
   //
   METALMOCK(fcloseMock.CalledOnceWith(&FilePointer));
}

TEST(CloseFile_CallsFCloseOnFilePointerWhichReturnsNon0_ThrowsRuntimeError)
{
   const int non0FCloseReturnValue = ZenUnit::RandomNon0<int>();
   fcloseMock.Return(non0FCloseReturnValue);

   const pair<int, string> errnoWithDescription = _errorCodeTranslatorMock->GetErrnoWithDescriptionMock.ReturnRandom();

   FILE FilePointer{};
   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   const string expectedExceptionMessage = String::ConcatValues("fclose(FILE*) unexpectedly returned ", non0FCloseReturnValue,
      ". filePath=\"", filePath.string(), "\", errno=", errnoWithDescription.first, " (", errnoWithDescription.second, ")");
   THROWS_EXCEPTION(_fileOpenerCloser.CloseFile(&FilePointer, filePath),
      runtime_error, expectedExceptionMessage);
   //
   METALMOCK(_errorCodeTranslatorMock->GetErrnoWithDescriptionMock.CalledOnce());
   METALMOCK(fcloseMock.CalledOnceWith(&FilePointer));
}

// Private Functions

TEST(ThrowFileOpenExceptionIfFileOpenFailed_FilePointerIsNullptr_ThrowIfFileNotOpenableIsFalse_DoesNothing)
{
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const bool throwIfFileNotOpenable = false;
   //
   _fileOpenerCloser.ThrowFileOpenExceptionIfFileOpenFailed(nullptr, filePath, throwIfFileNotOpenable);
}

TEST(ThrowFileOpenExceptionIfFileOpenFailed_FilePointerIsNullptr_ThrowIfFileNotOpenableIsTrue_ThrowsFileOpenException)
{
   const pair<int, string> errnoWithDescription = _errorCodeTranslatorMock->GetErrnoWithDescriptionMock.ReturnRandom();
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const bool throwIfFileNotOpenable = true;
   //
   const string expectedExceptionMessage = String::ConcatValues("fopen() returned nullptr. filePath=\"",
      filePath.string(), "\". errno=", errnoWithDescription.first, " (", errnoWithDescription.second, ")");
   THROWS_EXCEPTION(_fileOpenerCloser.ThrowFileOpenExceptionIfFileOpenFailed(nullptr, filePath, throwIfFileNotOpenable),
      runtime_error, expectedExceptionMessage);
   //
   METALMOCK(_errorCodeTranslatorMock->GetErrnoWithDescriptionMock.CalledOnce());
}

TEST(ThrowFileOpenExceptionIfFileOpenFailed_FilePointerIsNotNullptr_ThrowIfFileNotOpenableIsTrueOrFalse_DoesNothing)
{
   FILE nonNullFilePointer{};
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const bool throwIfFileNotOpenable = ZenUnit::Random<bool>();
   //
   _fileOpenerCloser.ThrowFileOpenExceptionIfFileOpenFailed(&nonNullFilePointer, filePath, throwIfFileNotOpenable);
}

RUN_TESTS(FileOpenerCloserTests)
