#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystemExceptions.h"
#include "libFileRevisor/Enums/FileExceptionType.h"

namespace Utils
{
   FileNotFoundException::FileNotFoundException(const fs::path& filePath)
      : runtime_error(filePath.string())
   {
   }
}

FileSystemException::FileSystemException(FileExceptionType fileExceptionType, string_view exceptionMessage)
   // Constant Fields
   : _exceptionMessage(ENUM_AS_STRING(FileExceptionType, fileExceptionType) + ": "s + string(exceptionMessage))
{
}

FileSystemException::~FileSystemException()
{
}

const char* FileSystemException::what() const noexcept
{
   return _exceptionMessage.c_str();
}
