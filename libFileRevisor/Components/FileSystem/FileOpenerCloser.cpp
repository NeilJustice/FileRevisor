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

FILE* FileOpenerCloser::CreateWriteModeBinaryFile(const fs::path& filePath) const
{
#if defined __linux__ || defined __APPLE__
   FILE* const writeModeBinaryFileHandle = OpenFileOnLinux(filePath, "wb", true);
#elif _WIN32
   FILE* const writeModeBinaryFileHandle = OpenFileOnWindows(filePath, L"wb", true);
#endif
   return writeModeBinaryFileHandle;
}

FILE* FileOpenerCloser::CreateWriteModeTextFile(const fs::path& filePath) const
{
#if defined __linux__ || defined __APPLE__
   FILE* const writeModeTextFileHandle = OpenFileOnLinux(filePath, "w", true);
#elif _WIN32
   FILE* const writeModeTextFileHandle = OpenFileOnWindows(filePath, L"w", true);
#endif
   return writeModeTextFileHandle;
}

FILE* FileOpenerCloser::OpenReadModeBinaryFile(const fs::path& filePath, bool throwIfFileNotOpenable) const
{
#if defined __linux__ || defined __APPLE__
   FILE* const readModeBinaryFileHandle = OpenFileOnLinux(filePath, "rb", throwIfFileNotOpenable);
#elif _WIN32
   FILE* const readModeBinaryFileHandle = OpenFileOnWindows(filePath, L"rb", throwIfFileNotOpenable);
#endif
   return readModeBinaryFileHandle;
}

FILE* FileOpenerCloser::OpenReadModeTextFile(const fs::path& filePath) const
{
#if defined __linux__ || defined __APPLE__
   FILE* const readModeTextFileHandle = OpenFileOnLinux(filePath, "r", true);
#elif _WIN32
   FILE* const readModeTextFileHandle = OpenFileOnWindows(filePath, L"r", true);
#endif
   return readModeTextFileHandle;
}

FILE* FileOpenerCloser::OpenAppendModeTextFile(const fs::path& filePath) const
{
#if defined __linux__ || defined __APPLE__
   FILE* const appendModeTextFileHandle = OpenFileOnLinux(filePath, "a", true);
#elif _WIN32
   FILE* const appendModeTextFileHandle = OpenFileOnWindows(filePath, L"a", true);
#endif
   return appendModeTextFileHandle;
}

void FileOpenerCloser::CloseFile(FILE* filePointer, const fs::path& filePath) const
{
   const int fcloseReturnValue = _call_fclose(filePointer);
   if (fcloseReturnValue != 0)
   {
      const pair<int, string> errnoWithDescription = _errorCodeTranslator->GetErrnoWithDescription();
      const string exceptionMessage = String::Concat("fclose(FILE*) unexpectedly returned ", fcloseReturnValue,
         ". filePath=\"", filePath.string(), "\", errno=", errnoWithDescription.first, " (", errnoWithDescription.second, ")");
      throw runtime_error(exceptionMessage);
   }
}

// Private Functions

#if defined __linux__ || defined __APPLE__

FILE* FileOpenerCloser::OpenFileOnLinux(const fs::path& filePath, const char* fileOpenMode, bool throwIfFileNotOpenable) const
{
   FILE* const fileHandle = _call_fopen(filePath.c_str(), fileOpenMode);
   ThrowFileOpenExceptionIfFileOpenFailed(fileHandle, filePath, throwIfFileNotOpenable);
   return fileHandle;
}

#elif _WIN32

FILE* FileOpenerCloser::OpenFileOnWindows(const fs::path& filePath, const wchar_t* fileOpenMode, bool throwIfFileNotOpenable) const
{
   FILE* const fileHandle = _call_wfsopen(filePath.c_str(), fileOpenMode, _SH_DENYWR);
   ThrowFileOpenExceptionIfFileOpenFailed(fileHandle, filePath, throwIfFileNotOpenable);
   return fileHandle;
}

#endif

void FileOpenerCloser::ThrowFileOpenExceptionIfFileOpenFailed(
   FILE* fileHandle, const fs::path& filePath, bool throwIfFileNotOpenable) const
{
   if (fileHandle == nullptr)
   {
      if (throwIfFileNotOpenable)
      {
         const pair<int, string> errnoWithDescription = _errorCodeTranslator->GetErrnoWithDescription();
         const string exceptionMessage = String::Concat("fopen() returned nullptr. filePath=\"",
            filePath.string(), "\". errno=", errnoWithDescription.first, " (", errnoWithDescription.second, ")");
         throw runtime_error(exceptionMessage);
      }
   }
}
