#include "pch.h"
#include "libFileRevisor/Components/ErrorHandling/ErrorCodeTranslator.h"
#include "libFileRevisor/Components/FileSystem/FCloseDeleter.h"
#include "libFileRevisor/Components/FileSystem/FileOpenerCloser.h"
#include "libFileRevisor/Components/FileSystem/RawCFileAPI.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidThreeArgMemberFunctionCaller.h"

FileOpenerCloser::FileOpenerCloser()
   // Function Callers
   : _caller_ThrowFileOpenExceptionIfFileOpenFailed(make_unique<_caller_ThrowFileOpenExceptionIfFileOpenFailedType>())
   // Constant Components
   , _errorCodeTranslator(make_unique<ErrorCodeTranslator>())
   , _rawCFileAPI(make_unique<Utils::RawCFileAPI>())
{
}

FileOpenerCloser::~FileOpenerCloser()
{
}

shared_ptr<FILE> FileOpenerCloser::OpenReadModeBinaryFile(const fs::path& filePath, bool throwIfFileNotOpenable) const
{
#if defined __linux__
   FILE* const rawFilePointer = _rawCFileAPI->fopen(filePath.c_str(), "rb");
#elif _WIN32
   FILE* const rawFilePointer = _rawCFileAPI->wfsopen(filePath.c_str(), "rb", _SH_DENYWR);
#endif
   _caller_ThrowFileOpenExceptionIfFileOpenFailed->CallConstMemberFunction(
      this, &FileOpenerCloser::ThrowFileOpenExceptionIfFileOpenFailed,
      rawFilePointer,
      filePath,
      throwIfFileNotOpenable);
   shared_ptr<FILE> autoClosingFilePointer(rawFilePointer, FCloseDeleter());
   return autoClosingFilePointer;
}

void FileOpenerCloser::CloseFile(FILE* rawFilePointer, const fs::path& filePath) const
{
   const int fcloseReturnValue = _rawCFileAPI->fclose(rawFilePointer);
   [[unlikely]] if (fcloseReturnValue != 0)
   {
      const pair<int, string> errnoWithDescription = _errorCodeTranslator->GetErrnoWithDescription();
      const string exceptionMessage = Utils::String::ConcatValues("fclose(FILE*) unexpectedly returned ", fcloseReturnValue,
         ". filePath=\"", filePath.string(), "\", errno=", errnoWithDescription.first, " (", errnoWithDescription.second, ")");
      throw runtime_error(exceptionMessage);
   }
}

// Private Functions

void FileOpenerCloser::ThrowFileOpenExceptionIfFileOpenFailed(
   const FILE* rawFilePointer,
   const fs::path& filePath,
   bool throwIfFileNotOpenable) const
{
   if (rawFilePointer == nullptr)
   {
      if (throwIfFileNotOpenable)
      {
         const pair<int, string> errnoWithDescription = _errorCodeTranslator->GetErrnoWithDescription();
         const string exceptionMessage = Utils::String::ConcatValues("fopen() returned nullptr. filePath=\"",
            filePath.string(), "\". errno=", errnoWithDescription.first, " (", errnoWithDescription.second, ")");
         throw runtime_error(exceptionMessage);
      }
   }
}
