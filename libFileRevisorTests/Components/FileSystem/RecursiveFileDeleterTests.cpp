#include "pch.h"
#include "libFileRevisor/Components/FileSystem/RecursiveFileDeleter.h"
#include "libFileRevisorTests/Components/Exceptions/ZenMock/ErrorCodeTranslatorMock.h"
#include "libFileRevisorTests/Components/Exceptions/ZenMock/FileSystemExceptionMakerMock.h"

TESTS(RecursiveFileDeleterTests)
AFACT(Constructor_NewsErrorCodeTranslator)
AFACT(OptionallyThrowFileSystemExceptionDueToUnlinkFailing_SkipFilesInUseIsFalse_ThrowsFileSystemException)
AFACT(OptionallyThrowFileSystemExceptionDueToUnlinkFailing_SkipFilesInUseIsTrue_ErrnoIs13_WritesSkippingMessage_DoesNotThrowException)
AFACT(OptionallyThrowFileSystemExceptionDueToUnlinkFailing_SkipFilesInUseIsTrue_ErrnoIsNot13_ThrowsFileSystemException)
#ifdef _WIN32
AFACT(ThrowFileSystemExceptionIfFindFirstFileExReturnedInvalidHandleValue_FindFirstFileExHandleIs0_DoesNothing)
AFACT(ThrowFileSystemExceptionIfFindFirstFileExReturnedInvalidHandleValue_FindFirstFileExHandleIsInvalidHandleValue_ThrowsFileSystemException)
AFACT(RemoveReadonlyFlagFromConstCharPointerFilePath_GetsFileAttributesWhichDoesNotIncludeReadonlyAttribute_DoesNothing)
FACTS(RemoveReadonlyFlagFromConstCharPointerFilePath_GetsFileAttributesWhichIncludeReadonlyAttribute_RemovesReadonlyAttributeWhichSuceeds_Returns)
FACTS(RemoveReadonlyFlagFromConstCharPointerFilePath_GetsFileAttributesWhichIncludeReadonlyAttribute_RemovesReadonlyAttributeWhichFails_ThrowsFileSystemException)
#endif
EVIDENCE

RecursiveFileDeleter _recursiveFileDeleter;
ConsoleMock* _consoleMock = nullptr;
FileSystemExceptionMakerMock* _fileSystemExceptionMakerMock = nullptr;
#ifdef _WIN32
ZENMOCK_NONVOID1_FREE(DWORD, GetFileAttributesA, const char*)
ZENMOCK_NONVOID2_FREE(DWORD, SetFileAttributesA, const char*, DWORD)
#endif

STARTUP
{
   _recursiveFileDeleter._console.reset(_consoleMock = new ConsoleMock);
   _recursiveFileDeleter._fileSystemExceptionMaker.reset(_fileSystemExceptionMakerMock = new FileSystemExceptionMakerMock);
#ifdef _WIN32
   _recursiveFileDeleter._call_GetFileAttributesA = BIND_1ARG_ZENMOCK_OBJECT(GetFileAttributesAMock);
   _recursiveFileDeleter._call_SetFileAttributesA = BIND_2ARG_ZENMOCK_OBJECT(SetFileAttributesAMock);
#endif
}

TEST(Constructor_NewsErrorCodeTranslator)
{
   RecursiveFileDeleter recursiveFileDeleter;
   DELETE_TO_ASSERT_NEWED(recursiveFileDeleter._fileSystemExceptionMaker);
}

TEST(OptionallyThrowFileSystemExceptionDueToUnlinkFailing_SkipFilesInUseIsFalse_ThrowsFileSystemException)
{
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.skipFilesInUse = false;

   const FileSystemException fileSystemException =
      _fileSystemExceptionMakerMock->MakeFileSystemExceptionForFailedToDeleteFileMock.ReturnRandom();

   const char* const filePath = ZenUnit::Random<const char*>();
   //
   THROWS_EXCEPTION(_recursiveFileDeleter.OptionallyThrowFileSystemExceptionDueToUnlinkFailing(filePath, args),
      FileSystemException, fileSystemException.what());
   //
   ZENMOCK(_fileSystemExceptionMakerMock->
      MakeFileSystemExceptionForFailedToDeleteFileMock.CalledOnceWith(filePath));
}

TEST(OptionallyThrowFileSystemExceptionDueToUnlinkFailing_SkipFilesInUseIsTrue_ErrnoIs13_WritesSkippingMessage_DoesNotThrowException)
{
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.skipFilesInUse = true;

   _fileSystemExceptionMakerMock->GetErrnoValueMock.Return(ErrnoValue::PermissionDenied);

   _consoleMock->WriteLineMock.Expect();

   const char* const filePath = ZenUnit::Random<const char*>();
   //
   _recursiveFileDeleter.OptionallyThrowFileSystemExceptionDueToUnlinkFailing(filePath, args);
   //
   const string expectedSkippingFileMessage = String::Concat(
      "[FileRevisor] Skipped: File \"", filePath,
      "\" because permission was denied when attempting to delete it");
   ZENMOCK(_fileSystemExceptionMakerMock->GetErrnoValueMock.CalledOnce());
   ZENMOCK(_consoleMock->WriteLineMock.CalledOnceWith(expectedSkippingFileMessage));
}

TEST(OptionallyThrowFileSystemExceptionDueToUnlinkFailing_SkipFilesInUseIsTrue_ErrnoIsNot13_ThrowsFileSystemException)
{
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.skipFilesInUse = true;

   const FileSystemException fileSystemException =
      _fileSystemExceptionMakerMock->MakeFileSystemExceptionForFailedToDeleteFileMock.ReturnRandom();

   const int errnoValueThatIsNotPermissionDenied =
      ZenUnit::RandomBetween<int>(0, ErrnoValue::PermissionDenied - 1);
   _fileSystemExceptionMakerMock->GetErrnoValueMock.Return(errnoValueThatIsNotPermissionDenied);

   const char* const filePath = ZenUnit::Random<const char*>();
   //
   THROWS_EXCEPTION(_recursiveFileDeleter.
      OptionallyThrowFileSystemExceptionDueToUnlinkFailing(filePath, args),
      FileSystemException, fileSystemException.what());
   //
   ZENMOCK(_fileSystemExceptionMakerMock->GetErrnoValueMock.CalledOnce());
   ZENMOCK(_fileSystemExceptionMakerMock->
      MakeFileSystemExceptionForFailedToDeleteFileMock.CalledOnceWith(filePath));
}

#ifdef _WIN32
TEST(ThrowFileSystemExceptionIfFindFirstFileExReturnedInvalidHandleValue_FindFirstFileExHandleIs0_DoesNothing)
{
   const HANDLE findFirstFileExHandle = 0;
   const char* const directoryPathSearchPatternChars = ZenUnit::Random<const char*>();
   _recursiveFileDeleter.ThrowFileSystemExceptionIfFindFirstFileExReturnedInvalidHandleValue(
      findFirstFileExHandle, directoryPathSearchPatternChars);
}

TEST(ThrowFileSystemExceptionIfFindFirstFileExReturnedInvalidHandleValue_FindFirstFileExHandleIsInvalidHandleValue_ThrowsFileSystemException)
{
   const FileSystemException fileSystemException = _fileSystemExceptionMakerMock->
      MakeFileSystemExceptionForFindFirstFileExHavingReturnedInvalidHandleValueMock.ReturnRandom();
   const HANDLE findFirstFileExHandle = INVALID_HANDLE_VALUE;
   const char* const directoryPathSearchPatternChars = ZenUnit::Random<const char*>();
   //
   THROWS_EXCEPTION(_recursiveFileDeleter.ThrowFileSystemExceptionIfFindFirstFileExReturnedInvalidHandleValue(
      findFirstFileExHandle, directoryPathSearchPatternChars),
      FileSystemException, fileSystemException.what());
   //
   ZENMOCK(_fileSystemExceptionMakerMock->
      MakeFileSystemExceptionForFindFirstFileExHavingReturnedInvalidHandleValueMock.
      CalledOnceWith(directoryPathSearchPatternChars));
}

TEST(RemoveReadonlyFlagFromConstCharPointerFilePath_GetsFileAttributesWhichDoesNotIncludeReadonlyAttribute_DoesNothing)
{
   const DWORD fileAttributes = 0;
   GetFileAttributesAMock.Return(fileAttributes);
   const char* const filePath = ZenUnit::Random<const char*>();
   //
   _recursiveFileDeleter.RemoveReadonlyFlagFromConstCharPointerFilePath(filePath);
   //
   ZENMOCK(GetFileAttributesAMock.CalledOnceWith(filePath));
}

TEST1X1(RemoveReadonlyFlagFromConstCharPointerFilePath_GetsFileAttributesWhichIncludeReadonlyAttribute_RemovesReadonlyAttributeWhichSuceeds_Returns,
   DWORD fileAttributes,
   FILE_ATTRIBUTE_READONLY,
   FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN)
{
   GetFileAttributesAMock.Return(fileAttributes);
   SetFileAttributesAMock.Return(TRUE);
   const char* const filePath = ZenUnit::Random<const char*>();
   //
   _recursiveFileDeleter.RemoveReadonlyFlagFromConstCharPointerFilePath(filePath);
   //
   const DWORD fileAttributesMinusReadonlyAttribute = fileAttributes & ~FILE_ATTRIBUTE_READONLY;
   ZENMOCK(GetFileAttributesAMock.CalledOnceWith(filePath));
   ZENMOCK(SetFileAttributesAMock.CalledOnceWith(filePath, fileAttributesMinusReadonlyAttribute));
}

TEST1X1(RemoveReadonlyFlagFromConstCharPointerFilePath_GetsFileAttributesWhichIncludeReadonlyAttribute_RemovesReadonlyAttributeWhichFails_ThrowsFileSystemException,
   DWORD fileAttributes,
   FILE_ATTRIBUTE_READONLY,
   FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN)
{
   const FileSystemException fileSystemException = _fileSystemExceptionMakerMock->
      MakeFileSystemExceptionForFailedToSetFileAttributeMock.ReturnRandom();
   GetFileAttributesAMock.Return(fileAttributes);
   SetFileAttributesAMock.Return(FALSE);
   const char* const filePath = ZenUnit::Random<const char*>();
   //
   THROWS_EXCEPTION(_recursiveFileDeleter.RemoveReadonlyFlagFromConstCharPointerFilePath(filePath),
      FileSystemException, fileSystemException.what());
   //
   const DWORD expectedFileAttributesMinusReadonlyAttribute = fileAttributes & ~FILE_ATTRIBUTE_READONLY;
   ZENMOCK(GetFileAttributesAMock.CalledOnceWith(filePath));
   ZENMOCK(SetFileAttributesAMock.CalledOnceWith(filePath, expectedFileAttributesMinusReadonlyAttribute));
   ZENMOCK(_fileSystemExceptionMakerMock->
      MakeFileSystemExceptionForFailedToSetFileAttributeMock.CalledOnceWith(
         filePath, expectedFileAttributesMinusReadonlyAttribute));
}
#endif

RUN_TESTS(RecursiveFileDeleterTests)
