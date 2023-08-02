#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"
#include "libFileRevisorTests/Components/Utility/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/Components/Utility/FunctionCallers/Member/MetalMock/VoidOneArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/Utility/FunctionCallers/TryCatchCallers/MetalMock/VoidTwoArgTryCatchCallerMock.h"
#include "libFileRevisorTests/Components/Utility/Iteration/ForEach/MetalMock/ParallelTwoArgMemberFunctionForEacherMock.h"
#include "libFileRevisorTests/Components/Utility/Iteration/ForEach/MetalMock/TwoArgMemberFunctionForEacherMock.h"
#include "libFileRevisorTests/Components/Utility/Strings/MetalMock/PluralizerMock.h"

TESTS(DeleteDirectorySubProgramTests)
AFACT(DefaultConstructor_NewsComponents)
AFACT(Run_TargetDirectoryDoesNotExist_WritesDirectoryDoesNotExistInformationalMessage_Returns0)
AFACT(Run_TargetDirectoryExists_ParallelIsFalse_WritesDeletingSequentiallyMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectorySequentiallyThenDeletesTargetDirectory_Returns0)
AFACT(Run_TargetDirectoryExists_ParallelIsTrue_WritesDeletingInParallelMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectoryInParallelThenDeletesTargetDirectory_Returns0)
// Private Functions
AFACT(DeleteDirectory_CallsRecursivelyDeleteAllFilesInDirectoryOnDirectoryPath)
AFACT(DeleteTargetDirectoryIfNotCurrentDirectory_CurrentDirectoryPathIsTargetDirectoryPath_DoesNothing)
AFACT(DeleteTargetDirectoryIfNotCurrentDirectory_CurrentDirectoryPathIsNotTargetDirectoryPath_DeletesTargetDirectory)
AFACT(TryCatchCallDeleteDirectory_TryCatchCallsDeleteDirectoryWithParallelExceptionHandler)
AFACT(ParallelExceptionHandler_WritesExceptionMessageWithThreadIdAndRedText)
EVIDENCE

DeleteDirectorySubProgram _deleteDirectorySubProgram;
// Base Class Constant Components
ConsoleMock* _consoleMock = nullptr;
FileSystemMock* _fileSystemMock = nullptr;
PluralizerMock* _pluralizerMock = nullptr;
// Function Callers
using _caller_DeleteTargetDirectoryIfNotCurrentDirectoryMockType = VoidOneArgMemberFunctionCallerMock<DeleteDirectorySubProgram, const FileRevisorArgs&>;
_caller_DeleteTargetDirectoryIfNotCurrentDirectoryMockType* _caller_DeleteTargetDirectoryIfNotCurrentDirectoryMock = nullptr;

using TwoArgMemberFunctionForEacherMockType = TwoArgMemberFunctionForEacherMock<DeleteDirectorySubProgram, string, const FileRevisorArgs&>;
TwoArgMemberFunctionForEacherMockType* _oneExtraArgMemberForEacher_DeleteDirectoryMock = nullptr;

using ParallelTwoArgMemberFunctionForEacherMockType = ParallelTwoArgMemberFunctionForEacherMock<DeleteDirectorySubProgram, string, const FileRevisorArgs&>;
ParallelTwoArgMemberFunctionForEacherMockType* _parallelTwoArgMemberFunctionForEacher_DeleteDirectoryMock = nullptr;

using _voidTwoArgTryCatchCallerMockType = VoidTwoArgTryCatchCallerMock<DeleteDirectorySubProgram, const string&, const FileRevisorArgs&>;
_voidTwoArgTryCatchCallerMockType* _voidTwoArgTryCatchCallerMock = nullptr;

STARTUP
{
   // Base Class Constant Components
   _deleteDirectorySubProgram._console.reset(_consoleMock = new ConsoleMock);
   _deleteDirectorySubProgram._fileSystem.reset(_fileSystemMock = new FileSystemMock);
   _deleteDirectorySubProgram._pluralizer.reset(_pluralizerMock = new PluralizerMock);
   // Function Callers
   _deleteDirectorySubProgram._caller_DeleteTargetDirectoryIfNotCurrentDirectory.reset(_caller_DeleteTargetDirectoryIfNotCurrentDirectoryMock = new _caller_DeleteTargetDirectoryIfNotCurrentDirectoryMockType);
   _deleteDirectorySubProgram._oneExtraArgMemberForEacher_DeleteDirectory.reset(_oneExtraArgMemberForEacher_DeleteDirectoryMock = new TwoArgMemberFunctionForEacherMockType);
   _deleteDirectorySubProgram._parallelTwoArgMemberFunctionForEacher_DeleteDirectory.reset(_parallelTwoArgMemberFunctionForEacher_DeleteDirectoryMock = new ParallelTwoArgMemberFunctionForEacherMockType);
   _deleteDirectorySubProgram._voidTwoArgTryCatchCaller.reset(_voidTwoArgTryCatchCallerMock = new _voidTwoArgTryCatchCallerMockType);
}

TEST(DefaultConstructor_NewsComponents)
{
   DeleteDirectorySubProgram deleteDirectorySubProgram;
   // Base Class Constant Components
   DELETE_TO_ASSERT_NEWED(deleteDirectorySubProgram._console);
   DELETE_TO_ASSERT_NEWED(deleteDirectorySubProgram._fileSystem);
   DELETE_TO_ASSERT_NEWED(deleteDirectorySubProgram._pluralizer);
   // Function Callers
   DELETE_TO_ASSERT_NEWED(deleteDirectorySubProgram._caller_DeleteTargetDirectoryIfNotCurrentDirectory);
   DELETE_TO_ASSERT_NEWED(deleteDirectorySubProgram._oneExtraArgMemberForEacher_DeleteDirectory);
   DELETE_TO_ASSERT_NEWED(deleteDirectorySubProgram._parallelTwoArgMemberFunctionForEacher_DeleteDirectory);
   DELETE_TO_ASSERT_NEWED(deleteDirectorySubProgram._voidTwoArgTryCatchCaller);
}

TEST(Run_TargetDirectoryDoesNotExist_WritesDirectoryDoesNotExistInformationalMessage_Returns0)
{
   _fileSystemMock->FileOrDirectoryExistsMock.Return(false);
   _consoleMock->ThreadIdWriteLineMock.Expect();
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   const int exitCode = _deleteDirectorySubProgram.Run(args);
   //
   const string expectedDirectoryDoesNotExistMessage = "Directory does not exist: " + args.targetDirectoryPath.string();
   METALMOCKTHEN(_fileSystemMock->FileOrDirectoryExistsMock.CalledOnceWith(args.targetDirectoryPath)).Then(
   METALMOCKTHEN(_consoleMock->ThreadIdWriteLineMock.CalledOnceWith(expectedDirectoryDoesNotExistMessage)));
   IS_ZERO(exitCode);
}

TEST(Run_TargetDirectoryExists_ParallelIsFalse_WritesDeletingSequentiallyMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectorySequentiallyThenDeletesTargetDirectory_Returns0)
{
   _fileSystemMock->FileOrDirectoryExistsMock.Return(true);

   _oneExtraArgMemberForEacher_DeleteDirectoryMock->CallConstMemberFunctionWithEachElementMock.Expect();

   const vector<string> topLevelDirectoryPathsInDirectory = _fileSystemMock->GetStringDirectoryPathsInDirectoryMock.ReturnRandom();

   _fileSystemMock->DeleteTopLevelFilesAndEmptyDirectoriesInDirectoryMock.Expect();

   _caller_DeleteTargetDirectoryIfNotCurrentDirectoryMock->CallConstMemberFunctionMock.Expect();

   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.parallel = false;
   //
   const int exitCode = _deleteDirectorySubProgram.Run(args);
   //
   METALMOCKTHEN(_fileSystemMock->FileOrDirectoryExistsMock.CalledOnceWith(args.targetDirectoryPath)).Then(
   METALMOCKTHEN(_fileSystemMock->GetStringDirectoryPathsInDirectoryMock.CalledOnceWith(args.targetDirectoryPath, false))).Then(
   METALMOCKTHEN(_oneExtraArgMemberForEacher_DeleteDirectoryMock->CallConstMemberFunctionWithEachElementMock.CalledOnceWith(
      topLevelDirectoryPathsInDirectory, &_deleteDirectorySubProgram, &DeleteDirectorySubProgram::DeleteDirectory, args))).Then(
   METALMOCKTHEN(_fileSystemMock->DeleteTopLevelFilesAndEmptyDirectoriesInDirectoryMock.CalledOnceWith(
      args.targetDirectoryPath, args.skipFilesInUse, args.dryrun, args.quiet))).Then(
   METALMOCKTHEN(_caller_DeleteTargetDirectoryIfNotCurrentDirectoryMock->CallConstMemberFunctionMock.CalledOnceWith(
      &_deleteDirectorySubProgram, &DeleteDirectorySubProgram::DeleteTargetDirectoryIfNotCurrentDirectory, args)));
   IS_ZERO(exitCode);
}

TEST(Run_TargetDirectoryExists_ParallelIsTrue_WritesDeletingInParallelMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectoryInParallelThenDeletesTargetDirectory_Returns0)
{
   _fileSystemMock->FileOrDirectoryExistsMock.Return(true);

   _parallelTwoArgMemberFunctionForEacher_DeleteDirectoryMock->ParallelCallConstMemberFunctionWithEachElementMock.Expect();

   const vector<string> topLevelDirectoryPathsInDirectory = _fileSystemMock->GetStringDirectoryPathsInDirectoryMock.ReturnRandom();

   _fileSystemMock->DeleteTopLevelFilesAndEmptyDirectoriesInDirectoryMock.Expect();

   _caller_DeleteTargetDirectoryIfNotCurrentDirectoryMock->CallConstMemberFunctionMock.Expect();

   _consoleMock->ThreadIdWriteLineMock.Expect();

   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.parallel = true;
   //
   const int exitCode = _deleteDirectorySubProgram.Run(args);
   //
   const string expectedDeletingInParallelMessage = "Deleting in parallel all files in directory: " + args.targetDirectoryPath.string();
   METALMOCKTHEN(_fileSystemMock->FileOrDirectoryExistsMock.CalledOnceWith(args.targetDirectoryPath)).Then(
   METALMOCKTHEN(_fileSystemMock->GetStringDirectoryPathsInDirectoryMock.CalledOnceWith(args.targetDirectoryPath, false))).Then(
   METALMOCKTHEN(_consoleMock->ThreadIdWriteLineMock.CalledOnceWith(expectedDeletingInParallelMessage))).Then(
   METALMOCKTHEN(_parallelTwoArgMemberFunctionForEacher_DeleteDirectoryMock->ParallelCallConstMemberFunctionWithEachElementMock.CalledOnceWith(
      topLevelDirectoryPathsInDirectory, &_deleteDirectorySubProgram, &DeleteDirectorySubProgram::TryCatchCallDeleteDirectory, args))).Then(
   METALMOCKTHEN(_fileSystemMock->DeleteTopLevelFilesAndEmptyDirectoriesInDirectoryMock.CalledOnceWith(
      args.targetDirectoryPath, args.skipFilesInUse, args.dryrun, args.quiet))).Then(
   METALMOCKTHEN(_caller_DeleteTargetDirectoryIfNotCurrentDirectoryMock->CallConstMemberFunctionMock.CalledOnceWith(
      &_deleteDirectorySubProgram, &DeleteDirectorySubProgram::DeleteTargetDirectoryIfNotCurrentDirectory, args)));
   IS_ZERO(exitCode);
}

// Private Functions

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

TEST(DeleteTargetDirectoryIfNotCurrentDirectory_CurrentDirectoryPathIsTargetDirectoryPath_DoesNothing)
{
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   _fileSystemMock->CurrentDirectoryPathMock.Return(args.targetDirectoryPath);
   //
   _deleteDirectorySubProgram.DeleteTargetDirectoryIfNotCurrentDirectory(args);
   //
   METALMOCK(_fileSystemMock->CurrentDirectoryPathMock.CalledOnce());
}

TEST(DeleteTargetDirectoryIfNotCurrentDirectory_CurrentDirectoryPathIsNotTargetDirectoryPath_DeletesTargetDirectory)
{
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   const fs::path currentDirectoryPath = ZenUnit::RandomNotEqualTo<fs::path>(args.targetDirectoryPath);
   _fileSystemMock->CurrentDirectoryPathMock.Return(currentDirectoryPath);
   _fileSystemMock->DeleteFileOrDirectoryMock.Expect();
   //
   _deleteDirectorySubProgram.DeleteTargetDirectoryIfNotCurrentDirectory(args);
   //
   METALMOCK(_fileSystemMock->CurrentDirectoryPathMock.CalledOnce());
   METALMOCK(_fileSystemMock->DeleteFileOrDirectoryMock.CalledOnceWith(args.targetDirectoryPath, args.skipFilesInUse, args.dryrun, args.quiet));
}

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
