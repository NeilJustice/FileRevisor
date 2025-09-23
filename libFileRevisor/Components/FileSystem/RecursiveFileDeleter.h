#pragma once
class Console;
class FileSystemExceptionMaker;
class ThreadIdGetter;

class RecursiveFileDeleter
{
   friend class RecursiveFileDeleterTests;
private:
   // Function Pointers
#ifdef _WIN32
   function<DWORD(const char*)> _call_GetFileAttributesA;
   function<BOOL(const char*, DWORD)> _call_SetFileAttributesA;
#endif
   // Constant Components
   unique_ptr<const Console> p_console;
   unique_ptr<const FileSystemExceptionMaker> _fileSystemExceptionMaker;
   unique_ptr<const ThreadIdGetter> _threadIdGetter;
public:
   RecursiveFileDeleter();
   virtual ~RecursiveFileDeleter();
   virtual void RecursivelyDeleteAllFilesInDirectory(const char* directoryPath, const FileRevisorArgs& args) const;
#ifdef _WIN32
   virtual void RemoveReadonlyFlagFromConstCharPointerFilePath(const char* filePath) const;
   virtual void RemoveReadonlyFlagFromFileSystemFilePath(const fs::path& filePath) const;
#endif
private:
   void PrintDeletedFileMessage(const char* filePath) const;
   virtual void ThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDenied(
      const char* filePath, const FileRevisorArgs& args) const;
#ifdef _WIN32
   void ThrowFileSystemExceptionIfFindFirstFileExReturnedInvalidHandleValue(
      const HANDLE findFirstFileExHandle, const char* directoryPathSearchPatternChars) const;
#endif
   void PrintDeletedFileMessageIfDeleteSucceededOtherwiseThrowFileSystemException(
      const char* filePath, int unlinkReturnValue, const FileRevisorArgs& args) const;
};
