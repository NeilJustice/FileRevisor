#include "pch.h"
#include "libFileRevisor/Exceptions/FileSystemException.h"
#include "libFileRevisor/Enums/FileExceptionType.h"

FileSystemException::FileSystemException(FileExceptionType fileExceptionType, string_view exceptionMessage)
   : _exceptionMessage(ENUM_TO_STRING(FileExceptionType, fileExceptionType) + ": " + string(exceptionMessage))
{
}

const char* FileSystemException::what() const noexcept
{
   return _exceptionMessage.c_str();
}
