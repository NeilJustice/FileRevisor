#include "pch.h"
#include "libFileRevisor/Components/ErrorHandling/ErrorCodeTranslator.h"
#include "libFileRevisor/Components/Exceptions/FileSystemExceptionMaker.h"
#include "libFileRevisor/Components/FileSystem/FileSystemExceptions.h"

FileSystemExceptionMaker::FileSystemExceptionMaker()
   // Constant Components
   : _errorCodeTranslator(make_unique<ErrorCodeTranslator>())
{
}

FileSystemExceptionMaker::~FileSystemExceptionMaker()
{
}

int FileSystemExceptionMaker::GetErrnoValue() const
{
   const int errnoValue = _errorCodeTranslator->GetErrnoValue();
   return errnoValue;
}

pair<int, string> FileSystemExceptionMaker::GetErrnoWithDescription() const
{
   pair<int, string> errnoWithDescription = _errorCodeTranslator->GetErrnoWithDescription();
   return errnoWithDescription;
}

FileSystemException FileSystemExceptionMaker::MakeFileSystemExceptionForFailedToOpenFileWithFOpen(const fs::path& filePath, const char* fileOpenMode) const
{
   const pair<int, string> errnoWithDescription = _errorCodeTranslator->GetErrnoWithDescription();
   const string exceptionMessage = Utils::String::ConcatValues("fopen() returned nullptr for filePath=\"", filePath.string(),
      "\" and fileOpenMode=\"", fileOpenMode, "\". errno=",
      errnoWithDescription.first, " (", errnoWithDescription.second, ")");
   FileSystemException fileSystemException(FileExceptionType::FailedToOpenFile, exceptionMessage);
   return fileSystemException;
}

FileSystemException FileSystemExceptionMaker::MakeFileSystemExceptionForFailedToOpenFileWithFStream(const fs::path& filePath) const
{
   const string exceptionMessage = Utils::String::ConcatStrings("Failed to open file: \"", filePath.string(), "\"");
   FileSystemException fileSystemException(FileExceptionType::FailedToOpenFile, exceptionMessage);
   return fileSystemException;
}

FileSystemException FileSystemExceptionMaker::MakeFileSystemExceptionForFailedToCloseFile(const fs::path& filePath) const
{
   const string exceptionMessage = "Failed to close file: \"" + filePath.string() + "\"";
   FileSystemException fileSystemException(FileExceptionType::FailedToCloseFile, exceptionMessage);
   return fileSystemException;
}

FileSystemException FileSystemExceptionMaker::MakeFileSystemExceptionForRemoveAllFailedToDeleteDirectory(
   const fs::path& fileOrFolderPath, int errorCodeValue) const
{
   const string systemErrorDescription = _errorCodeTranslator->GetSystemErrorDescription(errorCodeValue);
   const string exceptionMessage = Utils::String::ConcatValues(
      "fs::remove_all(\"", fileOrFolderPath.string(), "\", errorCode) failed with error code ", errorCodeValue, " (", systemErrorDescription, ")");
   FileSystemException fileSystemException(FileExceptionType::FailedToDeleteDirectory, exceptionMessage);
   return fileSystemException;
}

FileSystemException FileSystemExceptionMaker::MakeFileSystemExceptionForFailedToRenameDirectory(
   const fs::path& directoryPath, const fs::path& renamedFolderPath, const error_code& renameErrorCode) const
{
   const int renameErrorCodeValue = renameErrorCode.value();
   const string renameErrorCodeMessage = renameErrorCode.message();
   const string exceptionMessage = Utils::String::ConcatValues(
      "Failed to rename directory ", directoryPath.string(), " to ", renamedFolderPath.string(),
      ". renameErrorCode=", renameErrorCodeValue,
      ", renameErrorCodeMessage=\"", renameErrorCodeMessage, "\"");
   FileSystemException fileSystemException(FileExceptionType::FailedToRenameDirectory, exceptionMessage);
   return fileSystemException;
}

FileSystemException FileSystemExceptionMaker::MakeFileSystemExceptionForFailedToDeleteFile(const char* filePath) const
{
   const pair<int, string> errnoWithDescription = _errorCodeTranslator->GetErrnoWithDescription();
   const string exceptionMessage = Utils::String::ConcatValues("unlink(\"", filePath,
      "\") failed with errno ", errnoWithDescription.first, " (", errnoWithDescription.second, ")");
   FileSystemException fileSystemException(FileExceptionType::FailedToDeleteFile, exceptionMessage);
   return fileSystemException;
}

#ifdef _WIN32
FileSystemException FileSystemExceptionMaker::MakeFileSystemExceptionForFindFirstFileExHavingReturnedInvalidHandleValue(
   const char* directoryPathSearchPatternChars) const
{
   const string exceptionMessage = Utils::String::ConcatStrings(
      "FindFirstFileEx(\"", directoryPathSearchPatternChars, "\") returned INVALID_HANDLE_VALUE. Administrator rights are likely required in order to query this folder.");
   FileSystemException fileSystemException(FileExceptionType::WindowsFindFirstFileExReturnedInvalidHandleValue, exceptionMessage);
   return fileSystemException;
}

FileSystemException FileSystemExceptionMaker::MakeFileSystemExceptionForFailedToSetFileAttribute(const char* filePath, DWORD fileAttributes) const
{
   const pair<DWORD, string> windowsLastErrorWithDescription = _errorCodeTranslator->GetWindowsLastErrorWithDescription();
   const string exceptionMessage = Utils::String::ConcatValues(
      "SetFileAttributes(\"", filePath, "\", ", fileAttributes, ") failed. GetLastError()=",
      windowsLastErrorWithDescription.first, " (", windowsLastErrorWithDescription.second, ")");
   FileSystemException fileSystemException(FileExceptionType::WindowsFailedToSetFileAttribute, exceptionMessage);
   return fileSystemException;
}
#endif
