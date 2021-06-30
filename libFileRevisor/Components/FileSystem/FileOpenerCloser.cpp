#include "pch.h"
#include "libFileRevisor/Components/ErrorHandling/ErrorCodeTranslator.h"
#include "libFileRevisor/Components/FileSystem/FileOpenerCloser.h"

FileOpenerCloser::FileOpenerCloser()
   // Function Pointers
   : _call_fclose(fclose)
#if defined __linux__ || defined __APPLE__
   , _call_fopen(fopen)
#elif _WIN32
   , _call_wfsopen(_wfsopen)
#endif
   // Constant Components
   , _errorCodeTranslator(make_unique<ErrorCodeTranslator>())
{
}

FileOpenerCloser::~FileOpenerCloser()
{
}

FILE* FileOpenerCloser::OpenReadModeBinaryFile(const fs::path& filePath, bool throwIfFileNotOpenable) const
{
#if defined __linux__ || defined __APPLE__
   FILE* const readModeBinaryFilePointer = OpenFileOnLinux(filePath, "rb", throwIfFileNotOpenable);
#elif _WIN32
   FILE* const readModeBinaryFilePointer = OpenFileOnWindows(filePath, L"rb", throwIfFileNotOpenable);
#endif
   return readModeBinaryFilePointer;
}

void FileOpenerCloser::CloseFile(FILE* filePointer, const fs::path& filePath) const
{
   const int fcloseReturnValue = _call_fclose(filePointer);
   if (fcloseReturnValue != 0)
   {
      const pair<int, string> errnoWithDescription = _errorCodeTranslator->GetErrnoWithDescription();
      const string exceptionMessage = String::ConcatValues("fclose(FILE*) unexpectedly returned ", fcloseReturnValue,
         ". filePath=\"", filePath.string(), "\", errno=", errnoWithDescription.first, " (", errnoWithDescription.second, ")");
      throw runtime_error(exceptionMessage);
   }
}

// Private Functions

#if defined __linux__ || defined __APPLE__

FILE* FileOpenerCloser::OpenFileOnLinux(const fs::path& filePath, const char* fileOpenMode, bool throwIfFileNotOpenable) const
{
   FILE* const filePointer = _call_fopen(filePath.c_str(), fileOpenMode);
   ThrowFileOpenExceptionIfFileOpenFailed(filePointer, filePath, throwIfFileNotOpenable);
   return filePointer;
}

#elif _WIN32

FILE* FileOpenerCloser::OpenFileOnWindows(const fs::path& filePath, const wchar_t* fileOpenMode, bool throwIfFileNotOpenable) const
{
   FILE* const filePointer = _call_wfsopen(filePath.c_str(), fileOpenMode, _SH_DENYWR);
   ThrowFileOpenExceptionIfFileOpenFailed(filePointer, filePath, throwIfFileNotOpenable);
   return filePointer;
}

#endif

void FileOpenerCloser::ThrowFileOpenExceptionIfFileOpenFailed(FILE* filePointer, const fs::path& filePath, bool throwIfFileNotOpenable) const
{
   if (filePointer == nullptr)
   {
      if (throwIfFileNotOpenable)
      {
         const pair<int, string> errnoWithDescription = _errorCodeTranslator->GetErrnoWithDescription();
         const string exceptionMessage = String::ConcatValues("fopen() returned nullptr. filePath=\"",
            filePath.string(), "\". errno=", errnoWithDescription.first, " (", errnoWithDescription.second, ")");
         throw runtime_error(exceptionMessage);
      }
   }
}
