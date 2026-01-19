#include "pch.h"
#include "libFileRevisor/Enums/FileExceptionType.h"
#include "libFileRevisor/Exceptions/FileSystemException.h"

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
