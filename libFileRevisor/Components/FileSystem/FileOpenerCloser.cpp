#include "pch.h"
#include "libFileRevisor/Components/ErrorHandling/ErrorCodeTranslator.h"
#include "libFileRevisor/Components/FileSystem/FCloseDeleter.h"
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

shared_ptr<FILE> FileOpenerCloser::OpenReadModeBinaryFile(const fs::path& filePath, bool throwIfFileNotOpenable) const
{
#if defined __linux__ || defined __APPLE__
   shared_ptr<FILE> readModeBinaryFilePointer = OpenFileOnLinux(filePath, "rb", throwIfFileNotOpenable);
#elif _WIN32
   shared_ptr<FILE> readModeBinaryFilePointer = OpenFileOnWindows(filePath, L"rb", throwIfFileNotOpenable);
#endif
   return readModeBinaryFilePointer;
}

void FileOpenerCloser::CloseFile(FILE* rawFilePointer, const fs::path& filePath) const
{
   const int fcloseReturnValue = _call_fclose(rawFilePointer);
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

shared_ptr<FILE> FileOpenerCloser::OpenFileOnLinux(const fs::path& filePath, const char* fileOpenMode, bool throwIfFileNotOpenable) const
{
   FILE* const rawFilePointer = _call_fopen(filePath.c_str(), fileOpenMode);
   ThrowFileOpenExceptionIfFileOpenFailed(rawFilePointer, filePath, throwIfFileNotOpenable);
   shared_ptr<FILE> autoClosingFilePointer(rawFilePointer, FCloseDeleter());
   return autoClosingFilePointer;
}

#elif _WIN32

shared_ptr<FILE> FileOpenerCloser::OpenFileOnWindows(const fs::path& filePath, const wchar_t* fileOpenMode, bool throwIfFileNotOpenable) const
{
   FILE* const rawFilePointer = _call_wfsopen(filePath.c_str(), fileOpenMode, _SH_DENYWR);
   ThrowFileOpenExceptionIfFileOpenFailed(rawFilePointer, filePath, throwIfFileNotOpenable);
   shared_ptr<FILE> autoClosingFilePointer(rawFilePointer, FCloseDeleter());
   return autoClosingFilePointer;
}

#endif

void FileOpenerCloser::ThrowFileOpenExceptionIfFileOpenFailed(const FILE* rawFilePointer, const fs::path& filePath, bool throwIfFileNotOpenable) const
{
   if (rawFilePointer == nullptr)
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
