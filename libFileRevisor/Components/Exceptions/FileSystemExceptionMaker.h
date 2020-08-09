#pragma once
#include "libFileRevisor/Enums/FileExceptionType.h"
#include "libFileRevisor/Exceptions/FileSystemException.h"
class ErrorCodeTranslator;
enum class FileExceptionType;

class FileSystemExceptionMaker
{
   friend class FileSystemExceptionMakerTests;
private:
   unique_ptr<const ErrorCodeTranslator> _errorCodeTranslator;
public:
   FileSystemExceptionMaker();
   virtual ~FileSystemExceptionMaker();

   virtual int GetErrnoValue() const;

   virtual FileSystemException MakeFileSystemExceptionForFailedToOpenFileWithFOpen(
      const fs::path& filePath, const char* fileOpenMode) const;

   virtual FileSystemException MakeFileSystemExceptionForFailedToOpenFileWithFStream(
      const fs::path& filePath) const;

   virtual FileSystemException MakeFileSystemExceptionForFailedToCloseFile(
      const fs::path& filePath) const;

   virtual FileSystemException MakeFileSystemExceptionForFailedToDeleteDirectory(
      const fs::path& fileOrDirectoryPath,
      long long removeAllReturnValue,
      int errorCodeValue) const;

   virtual FileSystemException MakeFileSystemExceptionForFailedToRenameDirectory(
      const fs::path& directoryPath, const fs::path& renamedDirectoryPath, const error_code& renameErrorCode) const;

   virtual FileSystemException MakeFileSystemExceptionForFailedToDeleteFile(const char* filePath) const;

#ifdef _WIN32
   virtual FileSystemException MakeFileSystemExceptionForFindFirstFileExHavingReturnedInvalidHandleValue(
      const char* directoryPathSearchPatternChars) const;

   virtual FileSystemException MakeFileSystemExceptionForFailedToSetFileAttribute(
      const char* filePath, DWORD fileAttributes) const;
#endif
};
