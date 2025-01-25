#include "pch.h"
#include "libFileRevisor/Components/Exceptions/FileSystemExceptionMaker.h"
#include "libFileRevisor/Components/FileSystem/RecursiveFileDeleter.h"
#include "libFileRevisor/Components/Utility/Threading/ThreadIdGetter.h"

RecursiveFileDeleter::RecursiveFileDeleter()
#if defined __linux__
   // Constant Components
   : _console(make_unique<Console>())
#elif defined _WIN32
   // Function Pointers
   : _call_GetFileAttributesA(::GetFileAttributesA)
   , _call_SetFileAttributesA(::SetFileAttributesA)
   // Constant Components
   , _console(make_unique<Console>())
#endif
   , _fileSystemExceptionMaker(make_unique<FileSystemExceptionMaker>())
   , _threadIdGetter(make_unique<ThreadIdGetter>())
{
}

RecursiveFileDeleter::~RecursiveFileDeleter()
{
}

void RecursiveFileDeleter::PrintDeletedFileMessage(const char* filePath) const
{
   const string deletedFileMessage = String::ConcatStrings("Deleted ", filePath);
   _console->ProgramNameThreadIdWriteLine(deletedFileMessage);
}

#if defined __linux__
void RecursiveFileDeleter::RecursivelyDeleteAllFilesInDirectory(const char* directoryPath, const FileRevisorArgs& args) const
{
   DIR* const dirPointer = opendir(directoryPath);
   release_assert(dirPointer != nullptr);
   const dirent* directoryEntry = nullptr;
   char filePathOrSubdirectoryPathChars[PATH_MAX];
   while ((directoryEntry = readdir(dirPointer)) != nullptr)
   {
      const char* const fileNameOrSubdirectoryName = directoryEntry->d_name;
      const bool isDotOrDotDot =
         strcmp(fileNameOrSubdirectoryName, ".") == 0 ||
         strcmp(fileNameOrSubdirectoryName, "..") == 0;
      if (!isDotOrDotDot)
      {
         char* writePointer = stpcpy(filePathOrSubdirectoryPathChars, directoryPath);
         *writePointer++ = '/';
         writePointer = stpcpy(writePointer, fileNameOrSubdirectoryName);
         *writePointer = 0;
         if (directoryEntry->d_type == DT_DIR)
         {
            const char* const subdirectoryPath = filePathOrSubdirectoryPathChars;
            RecursivelyDeleteAllFilesInDirectory(subdirectoryPath, args);
         }
         else
         {
            const char* const filePath = filePathOrSubdirectoryPathChars;
            if (args.dryrun)
            {
               const string wouldDeleteFileMessage = String::ConcatStrings("DryRun: Would delete file ", filePath);
               _console->ProgramNameThreadIdWriteLine(wouldDeleteFileMessage);
            }
            else
            {
               const int unlinkReturnValue = unlink(filePath);
               PrintDeletedFileMessageIfDeleteSucceededOtherwiseThrowFileSystemException(filePath, unlinkReturnValue, args);
            }
         }
      }
   }
   const int closeDirReturnValue = closedir(dirPointer);
   release_assert(closeDirReturnValue == 0);
}

#elif _WIN32

void RecursiveFileDeleter::RecursivelyDeleteAllFilesInDirectory(const char* directoryPath, const FileRevisorArgs& args) const
{
   const size_t directoryPathLength = strlen(directoryPath);
   char directoryPathSearchPatternChars[MAX_PATH];
   memcpy(directoryPathSearchPatternChars, directoryPath, directoryPathLength);
   directoryPathSearchPatternChars[directoryPathLength] = '\\';
   directoryPathSearchPatternChars[directoryPathLength + 1] = '*';
   directoryPathSearchPatternChars[directoryPathLength + 2] = 0;

   WIN32_FIND_DATA win32FindData{};
   const HANDLE findFirstFilePointer = FindFirstFileEx(directoryPathSearchPatternChars, FindExInfoBasic, &win32FindData, FindExSearchNameMatch, NULL, NULL);
   ThrowFileSystemExceptionIfFindFirstFileExReturnedInvalidHandleValue(findFirstFilePointer, directoryPathSearchPatternChars);

   do
   {
      const char* const fileNameOrSubdirectoryName = win32FindData.cFileName;
      const bool isDotOrDotDot =
         strcmp(fileNameOrSubdirectoryName, ".") == 0 ||
         strcmp(fileNameOrSubdirectoryName, "..") == 0;
      if (!isDotOrDotDot)
      {
         char filePathOrSubdirectoryPathChars[MAX_PATH];
         memcpy(filePathOrSubdirectoryPathChars, directoryPath, directoryPathLength);
         filePathOrSubdirectoryPathChars[directoryPathLength] = '\\';

         const size_t fileNameOrSubdirectoryNameLength = strlen(win32FindData.cFileName);
         memcpy(filePathOrSubdirectoryPathChars + directoryPathLength + 1, win32FindData.cFileName, fileNameOrSubdirectoryNameLength);
         filePathOrSubdirectoryPathChars[directoryPathLength + 1 + fileNameOrSubdirectoryNameLength] = 0;

         if (win32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
         {
            const char* const subdirectoryPath = filePathOrSubdirectoryPathChars;
            RecursivelyDeleteAllFilesInDirectory(subdirectoryPath, args);
         }
         else
         {
            const char* const filePath = filePathOrSubdirectoryPathChars;
            if (args.dryrun)
            {
               const string wouldDeleteFileMessage = String::ConcatStrings("DryRun: Would delete file ", filePath);
               _console->ProgramNameThreadIdWriteLine(wouldDeleteFileMessage);
            }
            else
            {
               RemoveReadonlyFlagFromConstCharPointerFilePath(filePath);
               const int unlinkReturnValue = _unlink(filePath);
               PrintDeletedFileMessageIfDeleteSucceededOtherwiseThrowFileSystemException(filePath, unlinkReturnValue, args);
            }
         }
      }
   } while (FindNextFile(findFirstFilePointer, &win32FindData) != FALSE);
   const DWORD lastError = GetLastError();
   release_assert(lastError == ERROR_NO_MORE_FILES);
   const BOOL didCloseFilePointer = FindClose(findFirstFilePointer);
   release_assert(didCloseFilePointer == TRUE);
}

void RecursiveFileDeleter::ThrowFileSystemExceptionIfFindFirstFileExReturnedInvalidHandleValue(
   const HANDLE findFirstFileExHandle, const char* directoryPathSearchPatternChars) const
{
   if (findFirstFileExHandle == INVALID_HANDLE_VALUE)
   {
      const FileSystemException fileSystemException = _fileSystemExceptionMaker->
         MakeFileSystemExceptionForFindFirstFileExHavingReturnedInvalidHandleValue(
            directoryPathSearchPatternChars);
      throw fileSystemException;
   }
}

#endif

void RecursiveFileDeleter::ThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDenied(
   const char* filePath, const FileRevisorArgs& args) const
{
   if (args.skipFilesInUse)
   {
      const int errnoValue = _fileSystemExceptionMaker->GetErrnoValue();
      if (errnoValue == ErrnoValue::PermissionDenied)
      {
         const string skippingFileMessage = String::ConcatStrings(
            "Skipped file: \"", filePath, "\" because of error 13 (permission denied) when attempting to delete it");
         _console->ProgramNameThreadIdWriteLine(skippingFileMessage);
         return;
      }
   }
   const FileSystemException fileSystemException = _fileSystemExceptionMaker->MakeFileSystemExceptionForFailedToDeleteFile(filePath);
   throw fileSystemException;
}

#ifdef _WIN32

void RecursiveFileDeleter::RemoveReadonlyFlagFromConstCharPointerFilePath(const char* filePath) const
{
   const DWORD fileAttributes = _call_GetFileAttributesA(filePath);
   if (fileAttributes & FILE_ATTRIBUTE_READONLY)
   {
      const DWORD fileAttributesMinusReadonlyAttribute = fileAttributes & ~FILE_ATTRIBUTE_READONLY;
      const BOOL didSetFileAttributes = _call_SetFileAttributesA(filePath, fileAttributesMinusReadonlyAttribute);
      if (didSetFileAttributes == FALSE)
      {
         const FileSystemException fileSystemException = _fileSystemExceptionMaker->
            MakeFileSystemExceptionForFailedToSetFileAttribute(filePath, fileAttributesMinusReadonlyAttribute);
         throw fileSystemException;
      }
   }
}

void RecursiveFileDeleter::RemoveReadonlyFlagFromFileSystemFilePath(const fs::path& filePath) const
{
   RemoveReadonlyFlagFromConstCharPointerFilePath(filePath.string().c_str());
}

#endif

void RecursiveFileDeleter::PrintDeletedFileMessageIfDeleteSucceededOtherwiseThrowFileSystemException(
   const char* filePath, int unlinkReturnValue, const FileRevisorArgs& args) const
{
   if (unlinkReturnValue == 0)
   {
      if (!args.quiet)
      {
         PrintDeletedFileMessage(filePath);
      }
   }
   else
   {
      ThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDenied(filePath, args);
   }
}
