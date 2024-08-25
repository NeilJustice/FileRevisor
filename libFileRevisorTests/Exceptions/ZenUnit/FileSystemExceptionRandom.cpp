#include "pch.h"
#include "libFileRevisorTests/Exceptions/ZenUnit/FileSystemExceptionRandom.h"

FileSystemException TestableRandomFileSystemException(const ZenUnit::RandomGenerator* randomGenerator)
{
   const FileExceptionType fileExceptionType = static_cast<FileExceptionType>(randomGenerator->Enum(static_cast<int>(FileExceptionType::MaxValue)));
   const string exceptionMessage = randomGenerator->String();
   const FileSystemException randomFileSystemException(fileExceptionType, exceptionMessage);
   return randomFileSystemException;
}

namespace ZenUnit
{
   template<>
   FileSystemException Random()
   {
      ZenUnit::RandomGenerator randomGenerator;
      const FileSystemException randomFileSystemException = TestableRandomFileSystemException(&randomGenerator);
      return randomFileSystemException;
   }
}
