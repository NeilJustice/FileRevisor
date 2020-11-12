#pragma once
#include "libFileRevisor/Compiler/CompilerHints.h"
class Console;
class FileSystemExceptionMaker;
struct FileRevisorArgs;

class RecursiveFileDeleter
{
   friend class RecursiveFileDeleterTests;
private:
   // Constant Components
   unique_ptr<const Console> _console;
   unique_ptr<const FileSystemExceptionMaker> _fileSystemExceptionMaker;
   // Function Callers
#ifdef _WIN32
   function<DWORD(const char*)> _call_GetFileAttributesA;
   function<DWORD(const char*, DWORD)> _call_SetFileAttributesA;
#endif
public:
   RecursiveFileDeleter();
   virtual ~RecursiveFileDeleter();
   virtual void RecursivelyDeleteAllFilesInDirectory(const char* directoryPath, const FileRevisorArgs& args) const;
#ifdef _WIN32
   virtual void RemoveReadonlyFlagFromConstCharPointerFilePath(const char* filePath) const;
   virtual void RemoveReadonlyFlagFromFileSystemFilePath(const fs::path& filePath) const;
#endif
private:
   virtual void ThrowFileSystemExceptionExceptIfSkipFilesInUseModeIsTrueAndErrnoIsPermissionDenied(
      const char* filePath, const FileRevisorArgs& args) const;
#ifdef _WIN32
   void ThrowFileSystemExceptionIfFindFirstFileExReturnedInvalidHandleValue(
      HANDLE findFirstFileExHandle, const char* directoryPathSearchPatternChars) const;
#endif
};
