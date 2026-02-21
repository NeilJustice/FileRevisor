#include "pch.h"
#include "libFileRevisor/Components/Exceptions/FileSystemExceptionMaker.h"
#include "libFileRevisorTests/Components/ErrorHandling/MetalMock/ErrorCodeTranslatorMock.h"

TESTS(FileSystemExceptionMakerTests)
AFACT(Constructor_NewsErrorCodeTranslator)
AFACT(GetErrnoValue_ReturnsErrorCodeTranslatorGetErrnoValue)
AFACT(GetErrnoWithDescription_ReturnsErrorCodeTranslatorGetErrnoWithDescription)
AFACT(MakeFileSystemExceptionForFailedToOpenFileWithFOpen_ReturnsExpectedFileSystemException)
AFACT(MakeFileSystemExceptionForFailedToOpenFileWithFStream_ReturnsExpectedFileSystemException)
AFACT(MakeFileSystemExceptionForFailedToCloseFile_ReturnsExpectedFileSystemException)
AFACT(MakeFileSystemExceptionForRemoveAllFailedToDeleteDirectory_ReturnsExpectedFileSystemException)
AFACT(MakeFileSystemExceptionForFailedToRenameDirectory_ReturnsExpectedFileSystemException)
AFACT(MakeFileSystemExceptionForFailedToDeleteFile_ReturnsExpectedFileSystemException)
#ifdef _WIN32
AFACT(MakeFileSystemExceptionForFindFirstFileExHavingReturnedInvalidHandleValue_ReturnsExpectedFileSystemException)
AFACT(MakeFileSystemExceptionForFailedToSetFileAttribute_ReturnsExpectedFileSystemException)
#endif
EVIDENCE

FileSystemExceptionMaker _fileSystemExceptionMaker;
// Constant Components
ErrorCodeTranslatorMock* _errorCodeTranslatorMock = nullptr;
// Testing Fields
fs::path _filePath;
string _fileOpenMode;

STARTUP
{
   // Constant Components
   _fileSystemExceptionMaker._errorCodeTranslator.reset(_errorCodeTranslatorMock = new ErrorCodeTranslatorMock);
   // Testing Fields
   _filePath = ZenUnit::Random<fs::path>();
   _fileOpenMode = ZenUnit::Random<string>();
}

TEST(Constructor_NewsErrorCodeTranslator)
{
   FileSystemExceptionMaker fileSystemExceptionThrower;
   // Constant Components
   DELETE_TO_ASSERT_NEWED(fileSystemExceptionThrower._errorCodeTranslator);
}

TEST(GetErrnoValue_ReturnsErrorCodeTranslatorGetErrnoValue)
{
   const int errnoValue = _errorCodeTranslatorMock->GetErrnoValueMock.ReturnRandom();
   //
   const int returnedErrnoValue = _fileSystemExceptionMaker.GetErrnoValue();
   //
   METALMOCK(_errorCodeTranslatorMock->GetErrnoValueMock.CalledOnce());
   ARE_EQUAL(errnoValue, returnedErrnoValue);
}

TEST(GetErrnoWithDescription_ReturnsErrorCodeTranslatorGetErrnoWithDescription)
{
   const pair<int, string> errnoWithDescription(ZenUnit::Random<int>(), ZenUnit::Random<string>());
   _errorCodeTranslatorMock->GetErrnoWithDescriptionMock.Return(errnoWithDescription);
   //
   const pair<int, string> returnedErrnoWithDescription = _fileSystemExceptionMaker.GetErrnoWithDescription();
   //
   METALMOCK(_errorCodeTranslatorMock->GetErrnoWithDescriptionMock.CalledOnce());
   ARE_EQUAL(errnoWithDescription, returnedErrnoWithDescription);
}

TEST(MakeFileSystemExceptionForFailedToOpenFileWithFOpen_ReturnsExpectedFileSystemException)
{
   const pair<int, string> errnoWithDescription = ZenUnit::RandomPair<int, string>();
   _errorCodeTranslatorMock->GetErrnoWithDescriptionMock.Return(errnoWithDescription);
   //
   const FileSystemException fileSystemException =
      _fileSystemExceptionMaker.MakeFileSystemExceptionForFailedToOpenFileWithFOpen(_filePath, _fileOpenMode.c_str());
   //
   METALMOCK(_errorCodeTranslatorMock->GetErrnoWithDescriptionMock.CalledOnce());
   const string expectedExceptionMessage = Utils::String::ConcatValues("fopen() returned nullptr for filePath=\"",
      _filePath.string(), "\" and fileOpenMode=\"", _fileOpenMode, "\". errno=",
      errnoWithDescription.first, " (", errnoWithDescription.second, ")");
   const FileSystemException expectedFileSystemException(
      FileExceptionType::FailedToOpenFile, expectedExceptionMessage);
   ARE_EQUAL(expectedFileSystemException.what(), fileSystemException.what());
}

TEST(MakeFileSystemExceptionForFailedToOpenFileWithFStream_ReturnsExpectedFileSystemException)
{
   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   const FileSystemException fileSystemException = _fileSystemExceptionMaker.MakeFileSystemExceptionForFailedToOpenFileWithFStream(filePath);
   //
   const string expectedExceptionMessage = Utils::String::ConcatStrings("Failed to open file: \"", filePath.string(), "\"");
   const FileSystemException expectedFileSystemException(
      FileExceptionType::FailedToOpenFile, expectedExceptionMessage);
   ARE_EQUAL(expectedFileSystemException.what(), fileSystemException.what());
}

TEST(MakeFileSystemExceptionForFailedToCloseFile_ReturnsExpectedFileSystemException)
{
   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   const FileSystemException fileSystemException = _fileSystemExceptionMaker.MakeFileSystemExceptionForFailedToCloseFile(filePath);
   //
   const string expectedExceptionMessage = Utils::String::ConcatStrings("Failed to close file: \"", filePath.string(), "\"");
   const FileSystemException expectedFileSystemException(FileExceptionType::FailedToCloseFile, expectedExceptionMessage);
   ARE_EQUAL(expectedFileSystemException.what(), fileSystemException.what());
}

TEST(MakeFileSystemExceptionForRemoveAllFailedToDeleteDirectory_ReturnsExpectedFileSystemException)
{
   const string systemErrorDescription = _errorCodeTranslatorMock->GetSystemErrorDescriptionMock.ReturnRandom();

   const fs::path fileOrFolderPath = ZenUnit::Random<fs::path>();
   const int errorCodeValue = ZenUnit::Random<int>();
   //
   const FileSystemException fileSystemException =
      _fileSystemExceptionMaker.MakeFileSystemExceptionForRemoveAllFailedToDeleteDirectory(fileOrFolderPath, errorCodeValue);
   //
   METALMOCK(_errorCodeTranslatorMock->GetSystemErrorDescriptionMock.CalledOnceWith(errorCodeValue));
   const string exceptionMessage = Utils::String::ConcatValues(
      "fs::remove_all(\"", fileOrFolderPath.string(), "\", errorCode) failed with error code ", errorCodeValue, " (", systemErrorDescription, ")");
   const FileSystemException expectedFileSystemException(
      FileExceptionType::FailedToDeleteDirectory, exceptionMessage);
   ARE_EQUAL(expectedFileSystemException.what(), fileSystemException.what());
}

TEST(MakeFileSystemExceptionForFailedToRenameDirectory_ReturnsExpectedFileSystemException)
{
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   const fs::path renamedFolderPath = ZenUnit::Random<fs::path>();
   const error_code renameErrorCode = ZenUnit::Random<error_code>();
   //
   const FileSystemException fileSystemException =
      _fileSystemExceptionMaker.MakeFileSystemExceptionForFailedToRenameDirectory(directoryPath, renamedFolderPath, renameErrorCode);
   //
   const int renameErrorCodeValue = renameErrorCode.value();
   const string renameErrorCodeMessage = renameErrorCode.message();
   const string exceptionMessage = Utils::String::ConcatValues(
      "Failed to rename directory ", directoryPath.string(), " to ", renamedFolderPath.string(),
      ". renameErrorCode=", renameErrorCodeValue,
      ", renameErrorCodeMessage=\"", renameErrorCodeMessage, "\"");
   const FileSystemException expectedFileSystemException(
      FileExceptionType::FailedToRenameDirectory, exceptionMessage);
   ARE_EQUAL(expectedFileSystemException.what(), fileSystemException.what());
}

TEST(MakeFileSystemExceptionForFailedToDeleteFile_ReturnsExpectedFileSystemException)
{
   const pair<int, string> errnoWithDescription(ZenUnit::Random<int>(), ZenUnit::Random<string>());
   _errorCodeTranslatorMock->GetErrnoWithDescriptionMock.Return(errnoWithDescription);
   const char* const filePath = ZenUnit::Random<const char*>();
   //
   const FileSystemException fileSystemException = _fileSystemExceptionMaker.MakeFileSystemExceptionForFailedToDeleteFile(filePath);
   //
   METALMOCK(_errorCodeTranslatorMock->GetErrnoWithDescriptionMock.CalledOnce());
   const string expectedExceptionMessage = Utils::String::ConcatValues(
      "unlink(\"", filePath, "\") failed with errno ", errnoWithDescription.first, " (", errnoWithDescription.second, ")");
   const FileSystemException expectedFileSystemException(
      FileExceptionType::FailedToDeleteFile, expectedExceptionMessage);
   ARE_EQUAL(expectedFileSystemException.what(), fileSystemException.what());
}

#ifdef _WIN32
TEST(MakeFileSystemExceptionForFindFirstFileExHavingReturnedInvalidHandleValue_ReturnsExpectedFileSystemException)
{
   const char* const directoryPathSearchPatternChars = ZenUnit::Random<const char*>();
   //
   const FileSystemException fileSystemException =
      _fileSystemExceptionMaker.MakeFileSystemExceptionForFindFirstFileExHavingReturnedInvalidHandleValue(directoryPathSearchPatternChars);
   //
   const string exceptionMessage = Utils::String::ConcatStrings(
      "FindFirstFileEx(\"", directoryPathSearchPatternChars, "\") returned INVALID_HANDLE_VALUE. Administrator rights are likely required in order to query this folder.");
   const FileSystemException expectedFileSystemException(
      FileExceptionType::WindowsFindFirstFileExReturnedInvalidHandleValue, exceptionMessage);
   ARE_EQUAL(expectedFileSystemException.what(), fileSystemException.what());
}

TEST(MakeFileSystemExceptionForFailedToSetFileAttribute_ReturnsExpectedFileSystemException)
{
   const pair<DWORD, string> windowsLastErrorWithDescription(ZenUnit::Random<DWORD>(), ZenUnit::Random<string>());
   _errorCodeTranslatorMock->GetWindowsLastErrorWithDescriptionMock.Return(windowsLastErrorWithDescription);
   const char* const filePath = ZenUnit::Random<const char*>();
   const DWORD fileAttributes = ZenUnit::Random<DWORD>();
   //
   const FileSystemException fileSystemException = _fileSystemExceptionMaker.MakeFileSystemExceptionForFailedToSetFileAttribute(filePath, fileAttributes);
   //
   METALMOCK(_errorCodeTranslatorMock->GetWindowsLastErrorWithDescriptionMock.CalledOnce());
   const string expectedExceptionMessage = Utils::String::ConcatValues(
      "SetFileAttributes(\"", filePath, "\", ", fileAttributes, ") failed. GetLastError()=",
      windowsLastErrorWithDescription.first, " (", windowsLastErrorWithDescription.second, ")");
   const FileSystemException expectedFileSystemException(
      FileExceptionType::WindowsFailedToSetFileAttribute, expectedExceptionMessage);
   ARE_EQUAL(expectedFileSystemException.what(), fileSystemException.what());
}
#endif

RUN_TESTS(FileSystemExceptionMakerTests)
