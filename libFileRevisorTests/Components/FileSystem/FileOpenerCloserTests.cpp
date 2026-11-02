#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FCloseDeleter.h"
#include "libFileRevisor/Components/FileSystem/FileOpenerCloser.h"
#include "libFileRevisorTests/Components/ErrorHandling/MetalMock/ErrorCodeTranslatorMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/RawCFileAPIMock.h"

TESTS(FileOpenerCloserTests)
AFACT(OpenReadModeBinaryFile_DoesSo)
AFACT(CloseFile_FCloseReturns0_Returns)
AFACT(CloseFile_FCloseReturnsNot0_ThrowsRuntimeError)
// Private Functions
AFACT(ThrowFileOpenExceptionIfFileOpenFailed_FilePointerIsNullptr_ThrowIfFileNotOpenableIsFalse_DoesNothing)
AFACT(ThrowFileOpenExceptionIfFileOpenFailed_FilePointerIsNullptr_ThrowIfFileNotOpenableIsTrue_ThrowsFileOpenException)
AFACT(ThrowFileOpenExceptionIfFileOpenFailed_FilePointerIsNotNullptr_ThrowIfFileNotOpenableIsTrueOrFalse_DoesNothing)
EVIDENCE

FileOpenerCloser _fileOpenerCloser;
// Constant Components
ErrorCodeTranslatorMock* _errorCodeTranslatorMock = nullptr;
Utils::RawCFileAPIMock* _rawCFileAPIMock = nullptr;

STARTUP
{
   // Constant Components
   _fileOpenerCloser._errorCodeTranslator.reset(_errorCodeTranslatorMock = new ErrorCodeTranslatorMock);
   _fileOpenerCloser._rawCFileAPI.reset(_rawCFileAPIMock = new Utils::RawCFileAPIMock);
}

TEST(OpenReadModeBinaryFile_DoesSo)
{
   FILE* const readModeBinaryRawFilePointer = tmpfile();
#ifdef __linux__
   _rawCFileAPIMock->fopenMock.Return(readModeBinaryRawFilePointer);
#elif _WIN32
   _rawCFileAPIMock->wfsopenMock.Return(readModeBinaryRawFilePointer);
#endif
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const bool skipFilesInUse = ZenUnit::Random<bool>();
   //
   const shared_ptr<FILE> returnedReadModeBinaryFilePointer = _fileOpenerCloser.OpenReadModeBinaryFile(filePath, skipFilesInUse);
   //
#ifdef __linux__
   METALMOCK(_rawCFileAPIMock->fopenMock.CalledOnceWith(filePath.c_str(), "rb"));
#elif _WIN32
   METALMOCK(_rawCFileAPIMock->wfsopenMock.CalledOnceWith(filePath.c_str(), "rb", _SH_DENYWR));
#endif
   ARE_EQUAL(readModeBinaryRawFilePointer, returnedReadModeBinaryFilePointer.get());
}

TEST(CloseFile_FCloseReturns0_Returns)
{
   _rawCFileAPIMock->fcloseMock.Return(0);
   FILE* const rawFilePointer = tmpfile();
   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   _fileOpenerCloser.CloseFile(rawFilePointer, filePath);
   //
   METALMOCK(_rawCFileAPIMock->fcloseMock.CalledOnceWith(rawFilePointer));
}

TEST(CloseFile_FCloseReturnsNot0_ThrowsRuntimeError)
{
   const int non0FCloseReturnValue = ZenUnit::RandomNon0<int>();
   _rawCFileAPIMock->fcloseMock.Return(non0FCloseReturnValue);

   const pair<int, string> errnoWithDescription = _errorCodeTranslatorMock->GetErrnoWithDescriptionMock.ReturnRandom();

   FILE* const rawFilePointer = tmpfile();
   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   const string expectedExceptionMessage = Utils::String::ConcatValues(
      "fclose(FILE*) unexpectedly returned ", non0FCloseReturnValue,
      ". filePath=\"", filePath.string(), "\", errno=", errnoWithDescription.first, " (", errnoWithDescription.second, ")");
   THROWS_EXCEPTION(_fileOpenerCloser.CloseFile(rawFilePointer, filePath),
      runtime_error, expectedExceptionMessage);
   //
   METALMOCKTHEN(_rawCFileAPIMock->fcloseMock.CalledOnceWith(rawFilePointer)).Then(
   METALMOCKTHEN(_errorCodeTranslatorMock->GetErrnoWithDescriptionMock.CalledOnce()));
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
   const string expectedExceptionMessage = Utils::String::ConcatValues(
      "fopen() returned nullptr. filePath=\"",
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
