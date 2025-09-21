#include "pch.h"
#include "libFileRevisor/Components/FileSystem/RecursiveFileDeleter.h"
#include "libFileRevisorTests/Components/Exceptions/MetalMock/FileSystemExceptionMakerMock.h"
#include "libFileRevisorTests/Components/Console/MetalMock/ConsoleMock.h"

TESTS(RecursiveFileDeleterTests)
AFACT(Constructor_NewsComponents_SetsFunctionCallers)
AFACT(ThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDenied_SkipFilesInUseIsFalse_ThrowsFileSystemException)
AFACT(ThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDenied_SkipFilesInUseIsTrue_ErrnoIs13_WritesSkippingMessage_DoesNotThrowException)
AFACT(ThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDenied_SkipFilesInUseIsTrue_ErrnoIsNot13_ThrowsFileSystemException)
#ifdef _WIN32
AFACT(ThrowFileSystemExceptionIfFindFirstFileExReturnedInvalidHandleValue_FindFirstFileExHandleIs0_DoesNothing)
AFACT(ThrowFileSystemExceptionIfFindFirstFileExReturnedInvalidHandleValue_FindFirstFileExHandleIsInvalidHandleValue_ThrowsFileSystemException)
AFACT(RemoveReadonlyFlagFromConstCharPointerFilePath_GetsFileAttributesWhichDoesNotIncludeReadonlyAttribute_DoesNothing)
FACTS(RemoveReadonlyFlagFromConstCharPointerFilePath_GetsFileAttributesWhichIncludeReadonlyAttribute_RemovesReadonlyAttributeWhichSuceeds_Returns)
FACTS(RemoveReadonlyFlagFromConstCharPointerFilePath_GetsFileAttributesWhichIncludeReadonlyAttribute_RemovesReadonlyAttributeWhichFails_ThrowsFileSystemException)
#endif
AFACT(PrintDeletedFileMessageIfDeleteSucceededOtherwiseThrowFileSystemException_UnlinkReturnValueIs0_QuietIsTrue_DoesNothing)
AFACT(PrintDeletedFileMessageIfDeleteSucceededOtherwiseThrowFileSystemException_UnlinkReturnValueIs0_QuietIsFalse_PrintsDeletedFileMessage)
AFACT(PrintDeletedFileMessageIfDeleteSucceededOtherwiseThrowFileSystemException_UnlinkReturnValueIsNot0_CallsThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDenied)
EVIDENCE

RecursiveFileDeleter _recursiveFileDeleter;
// Function Pointers
#ifdef _WIN32
METALMOCK_NONVOID1_STATIC_OR_FREE(DWORD, GetFileAttributesA, const char*)
METALMOCK_NONVOID2_STATIC_OR_FREE(BOOL, SetFileAttributesA, const char*, DWORD)
#endif
// Constant Components
ConsoleMock* _consoleMock = nullptr;
FileSystemExceptionMakerMock* _fileSystemExceptionMakerMock = nullptr;

class RecursiveFileDeleter_SelfMocked : public Metal::Mock<RecursiveFileDeleter>
{
public:
   ConsoleMock* _consoleMock = nullptr;

   RecursiveFileDeleter_SelfMocked()
   {
      _console.reset(_consoleMock = new ConsoleMock);
   }

   METALMOCK_VOID2_CONST(ThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDenied, const char*, const FileRevisorArgs&)
} _recursiveFileDeleter_SelfMocked;

STARTUP
{
   // Function Pointers
#ifdef _WIN32
   _recursiveFileDeleter._call_GetFileAttributesA = BIND_1ARG_METALMOCK_OBJECT(GetFileAttributesAMock);
   _recursiveFileDeleter._call_SetFileAttributesA = BIND_2ARG_METALMOCK_OBJECT(SetFileAttributesAMock);
#endif
   // Constant Components
   _recursiveFileDeleter._console.reset(_consoleMock = new ConsoleMock);
   _recursiveFileDeleter._fileSystemExceptionMaker.reset(_fileSystemExceptionMakerMock = new FileSystemExceptionMakerMock);
}

TEST(Constructor_NewsComponents_SetsFunctionCallers)
{
   RecursiveFileDeleter recursiveFileDeleter;
   // Function Pointers
#ifdef _WIN32
   STD_FUNCTION_TARGETS(::GetFileAttributesA, recursiveFileDeleter._call_GetFileAttributesA);
   STD_FUNCTION_TARGETS(::SetFileAttributesA, recursiveFileDeleter._call_SetFileAttributesA);
#endif
   // Constant Components
   DELETE_TO_ASSERT_NEWED(recursiveFileDeleter._console);
   DELETE_TO_ASSERT_NEWED(recursiveFileDeleter._fileSystemExceptionMaker);
}

TEST(ThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDenied_SkipFilesInUseIsFalse_ThrowsFileSystemException)
{
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.skipFilesInUse = false;

   const FileSystemException fileSystemException =
      _fileSystemExceptionMakerMock->MakeFileSystemExceptionForFailedToDeleteFileMock.ReturnRandom();

   const char* const filePath = ZenUnit::Random<const char*>();
   //
   THROWS_EXCEPTION(_recursiveFileDeleter.ThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDenied(filePath, args),
      FileSystemException, fileSystemException.what());
   //
   METALMOCK(_fileSystemExceptionMakerMock->MakeFileSystemExceptionForFailedToDeleteFileMock.CalledOnceWith(filePath));
}

TEST(ThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDenied_SkipFilesInUseIsTrue_ErrnoIs13_WritesSkippingMessage_DoesNotThrowException)
{
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.skipFilesInUse = true;

   _fileSystemExceptionMakerMock->GetErrnoValueMock.Return(ErrnoValue::PermissionDenied);

   _consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   const char* const filePath = ZenUnit::Random<const char*>();
   //
   _recursiveFileDeleter.ThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDenied(filePath, args);
   //
   const string expectedSkippingFileMessage = String::ConcatStrings(
      "Skipped file: \"", filePath, "\" because of error 13 (permission denied) when attempting to delete it");
   METALMOCK(_fileSystemExceptionMakerMock->GetErrnoValueMock.CalledOnce());
   METALMOCK(_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(expectedSkippingFileMessage));
}

TEST(ThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDenied_SkipFilesInUseIsTrue_ErrnoIsNot13_ThrowsFileSystemException)
{
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.skipFilesInUse = true;

   const FileSystemException fileSystemException = _fileSystemExceptionMakerMock->MakeFileSystemExceptionForFailedToDeleteFileMock.ReturnRandom();

   const int errnoValueThatIsNotPermissionDenied =
      ZenUnit::RandomBetween<int>(0, static_cast<long long>(ErrnoValue::PermissionDenied) - 1);
   _fileSystemExceptionMakerMock->GetErrnoValueMock.Return(errnoValueThatIsNotPermissionDenied);

   const char* const filePath = ZenUnit::Random<const char*>();
   //
   THROWS_EXCEPTION(_recursiveFileDeleter.ThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDenied(filePath, args),
      FileSystemException, fileSystemException.what());
   //
   METALMOCK(_fileSystemExceptionMakerMock->GetErrnoValueMock.CalledOnce());
   METALMOCK(_fileSystemExceptionMakerMock->MakeFileSystemExceptionForFailedToDeleteFileMock.CalledOnceWith(filePath));
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
   const FileSystemException fileSystemException =
      _fileSystemExceptionMakerMock->MakeFileSystemExceptionForFindFirstFileExHavingReturnedInvalidHandleValueMock.ReturnRandom();
   const HANDLE findFirstFileExHandle = INVALID_HANDLE_VALUE;
   const char* const directoryPathSearchPatternChars = ZenUnit::Random<const char*>();
   //
   THROWS_EXCEPTION(_recursiveFileDeleter.ThrowFileSystemExceptionIfFindFirstFileExReturnedInvalidHandleValue(
      findFirstFileExHandle, directoryPathSearchPatternChars),
      FileSystemException, fileSystemException.what());
   //
   METALMOCK(_fileSystemExceptionMakerMock->
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
   METALMOCK(GetFileAttributesAMock.CalledOnceWith(filePath));
}

TEST1X1(RemoveReadonlyFlagFromConstCharPointerFilePath_GetsFileAttributesWhichIncludeReadonlyAttribute_RemovesReadonlyAttributeWhichSuceeds_Returns,
   DWORD fileAttributes,
   static_cast<DWORD>(FILE_ATTRIBUTE_READONLY),
   static_cast<DWORD>(FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN))
{
   GetFileAttributesAMock.Return(fileAttributes);
   SetFileAttributesAMock.Return(TRUE);
   const char* const filePath = ZenUnit::Random<const char*>();
   //
   _recursiveFileDeleter.RemoveReadonlyFlagFromConstCharPointerFilePath(filePath);
   //
   const DWORD fileAttributesMinusReadonlyAttribute = fileAttributes & ~FILE_ATTRIBUTE_READONLY;
   METALMOCK(GetFileAttributesAMock.CalledOnceWith(filePath));
   METALMOCK(SetFileAttributesAMock.CalledOnceWith(filePath, fileAttributesMinusReadonlyAttribute));
}

TEST1X1(RemoveReadonlyFlagFromConstCharPointerFilePath_GetsFileAttributesWhichIncludeReadonlyAttribute_RemovesReadonlyAttributeWhichFails_ThrowsFileSystemException,
   DWORD fileAttributes,
   static_cast<DWORD>(FILE_ATTRIBUTE_READONLY),
   static_cast<DWORD>(FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN))
{
   const FileSystemException fileSystemException = _fileSystemExceptionMakerMock->MakeFileSystemExceptionForFailedToSetFileAttributeMock.ReturnRandom();
   GetFileAttributesAMock.Return(fileAttributes);
   SetFileAttributesAMock.Return(FALSE);
   const char* const filePath = ZenUnit::Random<const char*>();
   //
   THROWS_EXCEPTION(_recursiveFileDeleter.RemoveReadonlyFlagFromConstCharPointerFilePath(filePath),
      FileSystemException, fileSystemException.what());
   //
   const DWORD expectedFileAttributesMinusReadonlyAttribute = fileAttributes & ~FILE_ATTRIBUTE_READONLY;
   METALMOCK(GetFileAttributesAMock.CalledOnceWith(filePath));
   METALMOCK(SetFileAttributesAMock.CalledOnceWith(filePath, expectedFileAttributesMinusReadonlyAttribute));
   METALMOCK(_fileSystemExceptionMakerMock->MakeFileSystemExceptionForFailedToSetFileAttributeMock.CalledOnceWith(
      filePath, expectedFileAttributesMinusReadonlyAttribute));
}
#endif

TEST(PrintDeletedFileMessageIfDeleteSucceededOtherwiseThrowFileSystemException_UnlinkReturnValueIs0_QuietIsTrue_DoesNothing)
{
   const char* const filePath = ZenUnit::Random<const char*>();
   const int unlinkReturnValue = 0;
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.quiet = true;
   //
   _recursiveFileDeleter_SelfMocked.PrintDeletedFileMessageIfDeleteSucceededOtherwiseThrowFileSystemException(filePath, unlinkReturnValue, args);
}

TEST(PrintDeletedFileMessageIfDeleteSucceededOtherwiseThrowFileSystemException_UnlinkReturnValueIs0_QuietIsFalse_PrintsDeletedFileMessage)
{
   _recursiveFileDeleter_SelfMocked._consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   const char* const filePath = ZenUnit::Random<const char*>();
   const int unlinkReturnValue = 0;
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.quiet = false;
   //
   _recursiveFileDeleter_SelfMocked.PrintDeletedFileMessageIfDeleteSucceededOtherwiseThrowFileSystemException(filePath, unlinkReturnValue, args);
   //
   const string expectedDeletedFilePathMessage = String::ConcatValues("Deleted ", filePath);
   METALMOCK(_recursiveFileDeleter_SelfMocked._consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(expectedDeletedFilePathMessage));
}

TEST(PrintDeletedFileMessageIfDeleteSucceededOtherwiseThrowFileSystemException_UnlinkReturnValueIsNot0_CallsThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDenied)
{
   _recursiveFileDeleter_SelfMocked.ThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDeniedMock.Expect();
   const char* const filePath = ZenUnit::Random<const char*>();
   const int unlinkReturnValue = ZenUnit::RandomNon0<int>();
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.quiet = false;
   //
   _recursiveFileDeleter_SelfMocked.PrintDeletedFileMessageIfDeleteSucceededOtherwiseThrowFileSystemException(filePath, unlinkReturnValue, args);
   //
   METALMOCK(_recursiveFileDeleter_SelfMocked.ThrowFileSystemExceptionExceptIfSkipFilesInUseIsTrueAndErrnoIsPermissionDeniedMock.CalledOnceWith(filePath, args));
}

RUN_TESTS(RecursiveFileDeleterTests)
