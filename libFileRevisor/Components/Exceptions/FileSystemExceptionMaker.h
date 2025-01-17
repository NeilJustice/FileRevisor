#pragma once
#include "libFileRevisor/Exceptions/FileSystemException.h"
class ErrorCodeTranslator;

class FileSystemExceptionMaker
{
   friend class FileSystemExceptionMakerTests;
private:
   // Constant Components
   unique_ptr<const ErrorCodeTranslator> _errorCodeTranslator;
public:
   FileSystemExceptionMaker();
   virtual ~FileSystemExceptionMaker();

   virtual int GetErrnoValue() const;

   virtual pair<int, string> GetErrnoWithDescription() const;

   virtual FileSystemException MakeFileSystemExceptionForFailedToOpenFileWithFOpen(
      const fs::path& filePath, const char* fileOpenMode) const;

   virtual FileSystemException MakeFileSystemExceptionForFailedToOpenFileWithFStream(
      const fs::path& filePath) const;

   virtual FileSystemException MakeFileSystemExceptionForFailedToCloseFile(
      const fs::path& filePath) const;

   virtual FileSystemException MakeFileSystemExceptionForRemoveAllFailedToDeleteDirectory(
      const fs::path& fileOrFolderPath, int errorCodeValue) const;

   virtual FileSystemException MakeFileSystemExceptionForFailedToRenameDirectory(
      const fs::path& directoryPath, const fs::path& renamedFolderPath, const error_code& renameErrorCode) const;

   virtual FileSystemException MakeFileSystemExceptionForFailedToDeleteFile(const char* filePath) const;

#ifdef _WIN32
   virtual FileSystemException MakeFileSystemExceptionForFindFirstFileExHavingReturnedInvalidHandleValue(
      const char* directoryPathSearchPatternChars) const;

   virtual FileSystemException MakeFileSystemExceptionForFailedToSetFileAttribute(
      const char* filePath, DWORD fileAttributes) const;
#endif
};
