#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"
#include "libFileRevisorTests/UtilityComponents/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/UtilityComponents/FunctionCallers/TryCatchCallers/MetalMock/VoidTwoArgTryCatchCallerMock.h"
#include "libFileRevisorTests/UtilityComponents/Iteration/ForEach/MetalMock/TwoArgMemberForEacherMock.h"
#include "libFileRevisorTests/UtilityComponents/Iteration/ForEach/MetalMock/ParallelTwoArgMemberForEacherMock.h"
#include "libFileRevisorTests/UtilityComponents/Strings/MetalMock/PluralizerMock.h"

TESTS(DeleteDirectorySubProgramTests)
AFACT(DefaultConstructor_NewsComponents)
AFACT(Run_TargetDirectoryDoesNotExist_WritesDirectoryDoesNotExistInformationalMessage_Returns0)
AFACT(Run_TargetDirectoryExists_ParallelIsFalse_WritesDeletingSequentiallyMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectorySequentiallyThenDeletesTargetDirectory_Returns0)
AFACT(Run_TargetDirectoryExists_ParallelIsTrue_WritesDeletingInParallelMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectoryInParallelThenDeletesTargetDirectory_Returns0)
// Private Functions
AFACT(TryCatchCallDeleteDirectory_TryCatchCallsDeleteDirectoryWithParallelExceptionHandler)
AFACT(DeleteDirectory_CallsRecursivelyDeleteAllFilesInDirectoryOnDirectoryPath)
AFACT(ParallelExceptionHandler_WritesExceptionMessageWithThreadIdAndRedText)
EVIDENCE

DeleteDirectorySubProgram _deleteDirectorySubProgram;
// Base Class Constant Components
ConsoleMock* _consoleMock = nullptr;
FileSystemMock* _fileSystemMock = nullptr;
PluralizerMock* _pluralizerMock = nullptr;
// Function Callers
using TwoArgMemberForEacherMockType = TwoArgMemberForEacherMock<DeleteDirectorySubProgram, string, const FileRevisorArgs&>;
TwoArgMemberForEacherMockType* _oneExtraArgMemberForEacher_DeleteDirectoryMock = nullptr;

using ParallelTwoArgMemberForEacherMockType = ParallelTwoArgMemberForEacherMock<DeleteDirectorySubProgram, string, const FileRevisorArgs&>;
ParallelTwoArgMemberForEacherMockType* _parallelTwoArgMemberForEacher_DeleteDirectoryMock = nullptr;

using _voidTwoArgTryCatchCallerMockType = VoidTwoArgTryCatchCallerMock<DeleteDirectorySubProgram, const string&, const FileRevisorArgs&>;
_voidTwoArgTryCatchCallerMockType* _voidTwoArgTryCatchCallerMock = nullptr;

STARTUP
{
   // Base Class Constant Components
   _deleteDirectorySubProgram._console.reset(_consoleMock = new ConsoleMock);
   _deleteDirectorySubProgram._fileSystem.reset(_fileSystemMock = new FileSystemMock);
   _deleteDirectorySubProgram._pluralizer.reset(_pluralizerMock = new PluralizerMock);
   // Function Callers
   _deleteDirectorySubProgram._oneExtraArgMemberForEacher_DeleteDirectory.reset(_oneExtraArgMemberForEacher_DeleteDirectoryMock = new TwoArgMemberForEacherMockType);
   _deleteDirectorySubProgram._parallelTwoArgMemberForEacher_DeleteDirectory.reset(_parallelTwoArgMemberForEacher_DeleteDirectoryMock = new ParallelTwoArgMemberForEacherMockType);
   _deleteDirectorySubProgram._voidTwoArgTryCatchCaller.reset(_voidTwoArgTryCatchCallerMock = new _voidTwoArgTryCatchCallerMockType);
}

TEST(DefaultConstructor_NewsComponents)
{
   DeleteDirectorySubProgram DeleteDirectorySubProgram;
   // Base Class Constant Components
   DELETE_TO_ASSERT_NEWED(DeleteDirectorySubProgram._console);
   DELETE_TO_ASSERT_NEWED(DeleteDirectorySubProgram._fileSystem);
   DELETE_TO_ASSERT_NEWED(DeleteDirectorySubProgram._pluralizer);
   // Function Callers
   DELETE_TO_ASSERT_NEWED(DeleteDirectorySubProgram._oneExtraArgMemberForEacher_DeleteDirectory);
   DELETE_TO_ASSERT_NEWED(DeleteDirectorySubProgram._parallelTwoArgMemberForEacher_DeleteDirectory);
   DELETE_TO_ASSERT_NEWED(DeleteDirectorySubProgram._voidTwoArgTryCatchCaller);
}

TEST(Run_TargetDirectoryDoesNotExist_WritesDirectoryDoesNotExistInformationalMessage_Returns0)
{
   _fileSystemMock->FileOrDirectoryExistsMock.Return(false);
   _consoleMock->ThreadIdWriteLineMock.Expect();
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   const int exitCode = _deleteDirectorySubProgram.Run(args);
   //
   METALMOCK(_fileSystemMock->FileOrDirectoryExistsMock.CalledOnceWith(args.targetDirectoryPath));
   const string expectedDirectoryDoesNotExistMessage = "Directory does not exist: " + args.targetDirectoryPath.string();
   METALMOCK(_consoleMock->ThreadIdWriteLineMock.CalledOnceWith(expectedDirectoryDoesNotExistMessage));
   IS_ZERO(exitCode);
}

TEST(Run_TargetDirectoryExists_ParallelIsFalse_WritesDeletingSequentiallyMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectorySequentiallyThenDeletesTargetDirectory_Returns0)
{
   _fileSystemMock->FileOrDirectoryExistsMock.Return(true);

   _oneExtraArgMemberForEacher_DeleteDirectoryMock->CallConstMemberFunctionWithEachElementMock.Expect();

   const vector<string> topLevelDirectoryPathsInDirectory = _fileSystemMock->GetStringDirectoryPathsInDirectoryMock.ReturnRandom();

   _fileSystemMock->DeleteTopLevelFilesAndEmptyDirectoriesInDirectoryMock.Expect();

   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.parallel = false;
   //
   const int exitCode = _deleteDirectorySubProgram.Run(args);
   //
   METALMOCK(_fileSystemMock->FileOrDirectoryExistsMock.CalledOnceWith(args.targetDirectoryPath));
   METALMOCK(_fileSystemMock->GetStringDirectoryPathsInDirectoryMock.CalledOnceWith(args.targetDirectoryPath, false));
   METALMOCK(_oneExtraArgMemberForEacher_DeleteDirectoryMock->CallConstMemberFunctionWithEachElementMock.CalledOnceWith(
      topLevelDirectoryPathsInDirectory, &_deleteDirectorySubProgram, &DeleteDirectorySubProgram::DeleteDirectory, args));
   METALMOCK(_fileSystemMock->DeleteTopLevelFilesAndEmptyDirectoriesInDirectoryMock.CalledOnceWith(
      args.targetDirectoryPath, args.skipFilesInUse, args.dryrun));
   IS_ZERO(exitCode);
}

TEST(Run_TargetDirectoryExists_ParallelIsTrue_WritesDeletingInParallelMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectoryInParallelThenDeletesTargetDirectory_Returns0)
{
   _fileSystemMock->FileOrDirectoryExistsMock.Return(true);

   _parallelTwoArgMemberForEacher_DeleteDirectoryMock->ParallelCallConstMemberFunctionWithEachElementMock.Expect();

   const vector<string> topLevelDirectoryPathsInDirectory = _fileSystemMock->GetStringDirectoryPathsInDirectoryMock.ReturnRandom();

   _fileSystemMock->DeleteTopLevelFilesAndEmptyDirectoriesInDirectoryMock.Expect();

   _consoleMock->ThreadIdWriteLineMock.Expect();

   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.parallel = true;
   //
   const int exitCode = _deleteDirectorySubProgram.Run(args);
   //
   METALMOCK(_fileSystemMock->FileOrDirectoryExistsMock.CalledOnceWith(args.targetDirectoryPath));
   METALMOCK(_fileSystemMock->GetStringDirectoryPathsInDirectoryMock.CalledOnceWith(args.targetDirectoryPath, false));
   METALMOCK(_parallelTwoArgMemberForEacher_DeleteDirectoryMock->ParallelCallConstMemberFunctionWithEachElementMock.CalledOnceWith(
      topLevelDirectoryPathsInDirectory, &_deleteDirectorySubProgram, &DeleteDirectorySubProgram::TryCatchCallDeleteDirectory, args));
   METALMOCK(_fileSystemMock->DeleteTopLevelFilesAndEmptyDirectoriesInDirectoryMock.CalledOnceWith(args.targetDirectoryPath, args.skipFilesInUse, args.dryrun));
   const string expectedDeletingInParallelMessage = "Deleting in parallel all files in directory: " + args.targetDirectoryPath.string();
   METALMOCK(_consoleMock->ThreadIdWriteLineMock.CalledOnceWith(expectedDeletingInParallelMessage));
   IS_ZERO(exitCode);
}

// Private Functions

TEST(TryCatchCallDeleteDirectory_TryCatchCallsDeleteDirectoryWithParallelExceptionHandler)
{
   _voidTwoArgTryCatchCallerMock->TryCatchCallConstMemberFunctionMock.Expect();
   const string directoryPath = ZenUnit::Random<string>();
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   _deleteDirectorySubProgram.TryCatchCallDeleteDirectory(directoryPath, args);
   //
   METALMOCK(_voidTwoArgTryCatchCallerMock->TryCatchCallConstMemberFunctionMock.CalledOnceWith(
      &_deleteDirectorySubProgram, &DeleteDirectorySubProgram::DeleteDirectory, directoryPath, args, &DeleteDirectorySubProgram::ParallelExceptionHandler));
}

TEST(DeleteDirectory_CallsRecursivelyDeleteAllFilesInDirectoryOnDirectoryPath)
{
   _fileSystemMock->RecursivelyDeleteAllFilesInDirectoryMock.Expect();
   const string directoryPath = ZenUnit::Random<string>();
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   _deleteDirectorySubProgram.DeleteDirectory(directoryPath, args);
   //
   METALMOCK(_fileSystemMock->RecursivelyDeleteAllFilesInDirectoryMock.CalledOnceWith(directoryPath, args));
}

TEST(ParallelExceptionHandler_WritesExceptionMessageWithThreadIdAndRedText)
{
   _consoleMock->ThreadIdWriteLineColorMock.Expect();
   const string exceptionClassNameAndMessage = ZenUnit::Random<string>();
   //
   _deleteDirectorySubProgram.ParallelExceptionHandler(exceptionClassNameAndMessage);
   //
   const string expectedErrorMessage = "Error: " + string(exceptionClassNameAndMessage);
   METALMOCK(_consoleMock->ThreadIdWriteLineColorMock.CalledOnceWith(expectedErrorMessage, Color::Red));
}

RUN_TESTS(DeleteDirectorySubProgramTests)
