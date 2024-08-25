#include "pch.h"
#include "libFileRevisorTests/Exceptions/ZenUnit/FileSystemExceptionRandom.h"

TESTS(FileSystemExceptionRandomTests)
AFACT(TestableRandomFileSystemException_ReturnsNewFileSystemExceptionWithRandomFileExceptionTypeAndRandomExceptionMessage)
EVIDENCE

TEST(TestableRandomFileSystemException_ReturnsNewFileSystemExceptionWithRandomFileExceptionTypeAndRandomExceptionMessage)
{
   ZenUnit::RandomGeneratorMock randomGeneratorMock;
   const FileExceptionType fileExceptionType = ZenUnit::RandomEnum<FileExceptionType>();
   randomGeneratorMock.EnumMock.Return(static_cast<int>(fileExceptionType));
   const string exceptionMessage = randomGeneratorMock.StringMock.ReturnRandom();
   //
   const FileSystemException randomFileSystemException = TestableRandomFileSystemException(&randomGeneratorMock);
   //
   METALMOCK(randomGeneratorMock.EnumMock.CalledOnceWith(static_cast<int>(FileExceptionType::MaxValue)));
   METALMOCK(randomGeneratorMock.StringMock.CalledOnce());
   const FileSystemException expectedRandomFileSystemException(fileExceptionType, exceptionMessage);
   ARE_EQUAL(expectedRandomFileSystemException.what(), randomFileSystemException.what());
}

RUN_TESTS(FileSystemExceptionRandomTests)
