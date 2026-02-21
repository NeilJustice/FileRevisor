#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FCloseDeleter.h"
#include "libFileRevisor/Components/FileSystem/FileOpenerCloser.h"
#include "libFileRevisorTests/Components/ErrorHandling/MetalMock/ErrorCodeTranslatorMock.h"

TESTS(FileOpenerCloserTests)
AFACT(DefaultConstructor_SetsFieldsToDefaultValues)
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
METALMOCK_NONVOID1_STATIC_OR_FREE(int, fclose, FILE*)
#if defined __linux__
METALMOCK_NONVOID2_STATIC_OR_FREE(FILE*, fopen, const char*, const char*)
#elif _WIN32
METALMOCK_NONVOID3_STATIC_OR_FREE(FILE*, _wfsopen, const wchar_t*, const wchar_t*, int)
#endif
// Constant Components
ErrorCodeTranslatorMock* _errorCodeTranslatorMock = nullptr;

STARTUP
{
   // Function Pointers
   _fileOpenerCloser._call_fclose = BIND_1ARG_METALMOCK_OBJECT(fcloseMock);
#if defined __linux__
   _fileOpenerCloser._call_fopen = BIND_2ARG_METALMOCK_OBJECT(fopenMock);
#elif _WIN32
   _fileOpenerCloser._call_wfsopen = BIND_3ARG_METALMOCK_OBJECT(_wfsopenMock);
#endif
   // Constant Components
   _fileOpenerCloser._errorCodeTranslator.reset(_errorCodeTranslatorMock = new ErrorCodeTranslatorMock);
}

TEST(DefaultConstructor_SetsFieldsToDefaultValues)
{
   const FileOpenerCloser fileOpenerCloser;
   STD_FUNCTION_TARGETS(::fclose, fileOpenerCloser._call_fclose);
#if defined __linux__
   STD_FUNCTION_TARGETS(fopen, fileOpenerCloser._call_fopen);
#elif _WIN32
   STD_FUNCTION_TARGETS(_wfsopen, fileOpenerCloser._call_wfsopen);
#endif
}

TEST(OpenReadModeBinaryFile_ReturnsFilePointerOpenedInBinaryReadMode)
{
   FILE* const readModeBinaryRawFilePointer = tmpfile();
#if defined __linux__
   fopenMock.Return(readModeBinaryRawFilePointer);
#elif _WIN32
   _wfsopenMock.Return(readModeBinaryRawFilePointer);
#endif
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const bool skipFilesInUse = ZenUnit::Random<bool>();
   //
   const shared_ptr<FILE> returnedReadModeBinaryFilePointer = _fileOpenerCloser.OpenReadModeBinaryFile(filePath, skipFilesInUse);
   //
#if defined __linux__
   METALMOCK(fopenMock.CalledOnceWith(filePath.c_str(), "rb"));
#elif _WIN32
   METALMOCK(_wfsopenMock.CalledOnceWith(filePath.c_str(), L"rb", _SH_DENYWR));
#endif
   ARE_EQUAL(readModeBinaryRawFilePointer, returnedReadModeBinaryFilePointer.get());
}

TEST(CloseFile_CallsFCloseOnFilePointerWhichReturns0_Returns)
{
   fcloseMock.Return(0);
   FILE* const rawFilePointer = tmpfile();
   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   _fileOpenerCloser.CloseFile(rawFilePointer, filePath);
   //
   METALMOCK(fcloseMock.CalledOnceWith(rawFilePointer));
}

TEST(CloseFile_CallsFCloseOnFilePointerWhichReturnsNon0_ThrowsRuntimeError)
{
   const int non0FCloseReturnValue = ZenUnit::RandomNon0<int>();
   fcloseMock.Return(non0FCloseReturnValue);

   const pair<int, string> errnoWithDescription = _errorCodeTranslatorMock->GetErrnoWithDescriptionMock.ReturnRandom();

   FILE* const rawFilePointer = tmpfile();
   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   const string expectedExceptionMessage = Utils::String::ConcatValues("fclose(FILE*) unexpectedly returned ", non0FCloseReturnValue,
      ". filePath=\"", filePath.string(), "\", errno=", errnoWithDescription.first, " (", errnoWithDescription.second, ")");
   THROWS_EXCEPTION(_fileOpenerCloser.CloseFile(rawFilePointer, filePath),
      runtime_error, expectedExceptionMessage);
   //
   METALMOCK(_errorCodeTranslatorMock->GetErrnoWithDescriptionMock.CalledOnce());
   METALMOCK(fcloseMock.CalledOnceWith(rawFilePointer));
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
   const string expectedExceptionMessage = Utils::String::ConcatValues("fopen() returned nullptr. filePath=\"",
      filePath.string(), "\". errno=", errnoWithDescription.first, " (", errnoWithDescription.second, ")");
   THROWS_EXCEPTION(_fileOpenerCloser.ThrowFileOpenExceptionIfFileOpenFailed(nullptr, filePath, throwIfFileNotOpenable),
      runtime_error, expectedExceptionMessage);
   //
   METALMOCK(_errorCodeTranslatorMock->GetErrnoWithDescriptionMock.CalledOnce());
}

TEST(ThrowFileOpenExceptionIfFileOpenFailed_FilePointerIsNotNullptr_ThrowIfFileNotOpenableIsTrueOrFalse_DoesNothing)
{
   const shared_ptr<FILE> nonNullRawFilePointer(tmpfile(), FCloseDeleter());
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const bool throwIfFileNotOpenable = ZenUnit::Random<bool>();
   //
   _fileOpenerCloser.ThrowFileOpenExceptionIfFileOpenFailed(nonNullRawFilePointer.get(), filePath, throwIfFileNotOpenable);
}

RUN_TESTS(FileOpenerCloserTests)
