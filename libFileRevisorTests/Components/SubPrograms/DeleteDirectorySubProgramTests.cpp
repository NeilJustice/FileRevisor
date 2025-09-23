#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"
#include "libFileRevisorTests/Components/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/MetalMock/VoidOneArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/TryCatchCallers/MetalMock/VoidTwoArgTryCatchCallerMock.h"
#include "libFileRevisorTests/Components/Iteration/ForEach/MetalMock/ParallelTwoArgMemberFunctionForEacherMock.h"
#include "libFileRevisorTests/Components/Iteration/ForEach/MetalMock/TwoArgMemberFunctionForEacherMock.h"
#include "libFileRevisorTests/Components/Strings/MetalMock/PluralizerMock.h"

TESTS(DeleteDirectorySubProgramTests)
AFACT(Run_TargetDirectoryDoesNotExist_WritesDirectoryDoesNotExistInformationalMessage_Returns0)
AFACT(Run_TargetDirectoryExists_ParallelIsFalse_WritesDeletingSequentiallyMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectorySequentiallyThenDeletesTargetDirectory_Returns0)
AFACT(Run_TargetDirectoryExists_ParallelIsTrue_WritesDeletingInParallelMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectoryInParallelThenDeletesTargetDirectory_Returns0)
// Private Functions
AFACT(DeleteDirectory_CallsRecursivelyDeleteAllFilesInDirectoryOnFolderPath)
AFACT(DeleteTargetDirectoryIfNotCurrentDirectory_CurrentFolderPathIsTargetFolderPath_DoesNothing)
AFACT(DeleteTargetDirectoryIfNotCurrentDirectory_CurrentFolderPathIsNotTargetFolderPath_DeletesTargetDirectory)
AFACT(TryCatchCallDeleteDirectory_TryCatchCallsDeleteDirectoryWithParallelExceptionHandler)
AFACT(ParallelExceptionHandler_WritesExceptionMessageWithThreadIdAndRedText)
EVIDENCE

DeleteDirectorySubProgram _deleteDirectorySubProgram;
// Base Class Constant Components
ConsoleMock* p_consoleMock = nullptr;
FileSystemMock* p_fileSystemMock = nullptr;
PluralizerMock* p_pluralizerMock = nullptr;
// Function Callers
using _caller_DeleteTargetDirectoryIfNotCurrentDirectoryMockType =
   VoidOneArgMemberFunctionCallerMock<DeleteDirectorySubProgram, const FileRevisorArgs&>;
_caller_DeleteTargetDirectoryIfNotCurrentDirectoryMockType* _caller_DeleteTargetDirectoryIfNotCurrentDirectoryMock = nullptr;

using  _oneExtraArgMemberForEacher_DeleteDirectoryMockType =
   TwoArgMemberFunctionForEacherMock<DeleteDirectorySubProgram, string, const FileRevisorArgs&>;
_oneExtraArgMemberForEacher_DeleteDirectoryMockType* _oneExtraArgMemberForEacher_DeleteDirectoryMock = nullptr;

using _parallelTwoArgMemberFunctionForEacher_DeleteDirectoryMockType =
   ParallelTwoArgMemberFunctionForEacherMock<DeleteDirectorySubProgram, string, const FileRevisorArgs&>;
_parallelTwoArgMemberFunctionForEacher_DeleteDirectoryMockType* _parallelTwoArgMemberFunctionForEacher_DeleteDirectoryMock = nullptr;

using _tryCatchCaller_DeleteDirectoryMockType =
   VoidTwoArgTryCatchCallerMock<DeleteDirectorySubProgram, const string&, const FileRevisorArgs&>;
_tryCatchCaller_DeleteDirectoryMockType* _tryCatchCaller_DeleteDirectoryMock = nullptr;

STARTUP
{
   // Base Class Constant Components
   _deleteDirectorySubProgram.p_console.reset(p_consoleMock = new ConsoleMock);
   _deleteDirectorySubProgram.p_fileSystem.reset(p_fileSystemMock = new FileSystemMock);
   _deleteDirectorySubProgram.p_pluralizer.reset(p_pluralizerMock = new PluralizerMock);
   // Function Callers
   _deleteDirectorySubProgram._caller_DeleteTargetDirectoryIfNotCurrentDirectory.reset(_caller_DeleteTargetDirectoryIfNotCurrentDirectoryMock = new _caller_DeleteTargetDirectoryIfNotCurrentDirectoryMockType);
   _deleteDirectorySubProgram._oneExtraArgMemberForEacher_DeleteDirectory.reset(_oneExtraArgMemberForEacher_DeleteDirectoryMock = new _oneExtraArgMemberForEacher_DeleteDirectoryMockType);
   _deleteDirectorySubProgram._parallelTwoArgMemberFunctionForEacher_DeleteDirectory.reset(_parallelTwoArgMemberFunctionForEacher_DeleteDirectoryMock = new _parallelTwoArgMemberFunctionForEacher_DeleteDirectoryMockType);
   _deleteDirectorySubProgram._tryCatchCaller_DeleteDirectory.reset(_tryCatchCaller_DeleteDirectoryMock = new _tryCatchCaller_DeleteDirectoryMockType);
}

TEST(Run_TargetDirectoryDoesNotExist_WritesDirectoryDoesNotExistInformationalMessage_Returns0)
{
   p_fileSystemMock->FileOrDirectoryExistsMock.Return(false);
   p_consoleMock->ProgramNameThreadIdWriteLineMock.Expect();
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   const int exitCode = _deleteDirectorySubProgram.Run(args);
   //
   const string expectedDirectoryDoesNotExistMessage = "Directory does not exist: " + args.targetFolderPath.string();
   METALMOCKTHEN(p_fileSystemMock->FileOrDirectoryExistsMock.CalledOnceWith(args.targetFolderPath)).Then(
   METALMOCKTHEN(p_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(expectedDirectoryDoesNotExistMessage)));
   IS_ZERO(exitCode);
}

TEST(Run_TargetDirectoryExists_ParallelIsFalse_WritesDeletingSequentiallyMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectorySequentiallyThenDeletesTargetDirectory_Returns0)
{
   p_fileSystemMock->FileOrDirectoryExistsMock.Return(true);

   _oneExtraArgMemberForEacher_DeleteDirectoryMock->CallConstMemberFunctionWithEachElementMock.Expect();

   const vector<string> topLevelFolderPathsInDirectory = p_fileSystemMock->GetStringFolderPathsInDirectoryMock.ReturnRandom();

   p_fileSystemMock->DeleteTopLevelFilesAndEmptyDirectoriesInDirectoryMock.Expect();

   _caller_DeleteTargetDirectoryIfNotCurrentDirectoryMock->CallConstMemberFunctionMock.Expect();

   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.parallel = false;
   //
   const int exitCode = _deleteDirectorySubProgram.Run(args);
   //
   METALMOCKTHEN(p_fileSystemMock->FileOrDirectoryExistsMock.CalledOnceWith(args.targetFolderPath)).Then(
   METALMOCKTHEN(p_fileSystemMock->GetStringFolderPathsInDirectoryMock.CalledOnceWith(args.targetFolderPath, false))).Then(
   METALMOCKTHEN(_oneExtraArgMemberForEacher_DeleteDirectoryMock->CallConstMemberFunctionWithEachElementMock.CalledOnceWith(
      topLevelFolderPathsInDirectory, &_deleteDirectorySubProgram, &DeleteDirectorySubProgram::DeleteDirectory, args))).Then(
   METALMOCKTHEN(p_fileSystemMock->DeleteTopLevelFilesAndEmptyDirectoriesInDirectoryMock.CalledOnceWith(
      args.targetFolderPath, args.skipFilesInUse, args.dryrun, args.quiet))).Then(
   METALMOCKTHEN(_caller_DeleteTargetDirectoryIfNotCurrentDirectoryMock->CallConstMemberFunctionMock.CalledOnceWith(
      &_deleteDirectorySubProgram, &DeleteDirectorySubProgram::DeleteTargetDirectoryIfNotCurrentDirectory, args)));
   IS_ZERO(exitCode);
}

TEST(Run_TargetDirectoryExists_ParallelIsTrue_WritesDeletingInParallelMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectoryInParallelThenDeletesTargetDirectory_Returns0)
{
   p_fileSystemMock->FileOrDirectoryExistsMock.Return(true);

   _parallelTwoArgMemberFunctionForEacher_DeleteDirectoryMock->ParallelCallConstMemberFunctionWithEachElementMock.Expect();

   const vector<string> topLevelFolderPathsInDirectory = p_fileSystemMock->GetStringFolderPathsInDirectoryMock.ReturnRandom();

   p_fileSystemMock->DeleteTopLevelFilesAndEmptyDirectoriesInDirectoryMock.Expect();

   _caller_DeleteTargetDirectoryIfNotCurrentDirectoryMock->CallConstMemberFunctionMock.Expect();

   p_consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.parallel = true;
   //
   const int exitCode = _deleteDirectorySubProgram.Run(args);
   //
   const string expectedDeletingInParallelMessage = "Deleting in parallel all files in directory: " + args.targetFolderPath.string();
   METALMOCKTHEN(p_fileSystemMock->FileOrDirectoryExistsMock.CalledOnceWith(args.targetFolderPath)).Then(
   METALMOCKTHEN(p_fileSystemMock->GetStringFolderPathsInDirectoryMock.CalledOnceWith(args.targetFolderPath, false))).Then(
   METALMOCKTHEN(p_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(expectedDeletingInParallelMessage))).Then(
   METALMOCKTHEN(_parallelTwoArgMemberFunctionForEacher_DeleteDirectoryMock->ParallelCallConstMemberFunctionWithEachElementMock.CalledOnceWith(
      topLevelFolderPathsInDirectory, &_deleteDirectorySubProgram, &DeleteDirectorySubProgram::TryCatchCallDeleteDirectory, args))).Then(
   METALMOCKTHEN(p_fileSystemMock->DeleteTopLevelFilesAndEmptyDirectoriesInDirectoryMock.CalledOnceWith(
      args.targetFolderPath, args.skipFilesInUse, args.dryrun, args.quiet))).Then(
   METALMOCKTHEN(_caller_DeleteTargetDirectoryIfNotCurrentDirectoryMock->CallConstMemberFunctionMock.CalledOnceWith(
      &_deleteDirectorySubProgram, &DeleteDirectorySubProgram::DeleteTargetDirectoryIfNotCurrentDirectory, args)));
   IS_ZERO(exitCode);
}

// Private Functions

TEST(DeleteDirectory_CallsRecursivelyDeleteAllFilesInDirectoryOnFolderPath)
{
   p_fileSystemMock->RecursivelyDeleteAllFilesInDirectoryMock.Expect();
   const string directoryPath = ZenUnit::Random<string>();
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   _deleteDirectorySubProgram.DeleteDirectory(directoryPath, args);
   //
   METALMOCK(p_fileSystemMock->RecursivelyDeleteAllFilesInDirectoryMock.CalledOnceWith(directoryPath, args));
}

TEST(DeleteTargetDirectoryIfNotCurrentDirectory_CurrentFolderPathIsTargetFolderPath_DoesNothing)
{
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   p_fileSystemMock->CurrentFolderPathMock.Return(args.targetFolderPath);
   //
   _deleteDirectorySubProgram.DeleteTargetDirectoryIfNotCurrentDirectory(args);
   //
   METALMOCK(p_fileSystemMock->CurrentFolderPathMock.CalledOnce());
}

TEST(DeleteTargetDirectoryIfNotCurrentDirectory_CurrentFolderPathIsNotTargetFolderPath_DeletesTargetDirectory)
{
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   const fs::path currentFolderPath = ZenUnit::RandomNotEqualTo<fs::path>(args.targetFolderPath);
   p_fileSystemMock->CurrentFolderPathMock.Return(currentFolderPath);
   p_fileSystemMock->DeleteFileOrDirectoryMock.Expect();
   //
   _deleteDirectorySubProgram.DeleteTargetDirectoryIfNotCurrentDirectory(args);
   //
   METALMOCK(p_fileSystemMock->CurrentFolderPathMock.CalledOnce());
   METALMOCK(p_fileSystemMock->DeleteFileOrDirectoryMock.CalledOnceWith(args.targetFolderPath, args.skipFilesInUse, args.dryrun, args.quiet));
}

TEST(TryCatchCallDeleteDirectory_TryCatchCallsDeleteDirectoryWithParallelExceptionHandler)
{
   _tryCatchCaller_DeleteDirectoryMock->TryCatchCallConstMemberFunctionMock.Expect();
   const string directoryPath = ZenUnit::Random<string>();
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   _deleteDirectorySubProgram.TryCatchCallDeleteDirectory(directoryPath, args);
   //
   METALMOCK(_tryCatchCaller_DeleteDirectoryMock->TryCatchCallConstMemberFunctionMock.CalledOnceWith(
      &_deleteDirectorySubProgram, &DeleteDirectorySubProgram::DeleteDirectory, directoryPath, args,
      &DeleteDirectorySubProgram::ParallelExceptionHandler));
}

TEST(ParallelExceptionHandler_WritesExceptionMessageWithThreadIdAndRedText)
{
   p_consoleMock->ProgramNameThreadIdWriteLineColorMock.Expect();
   const string exceptionClassNameAndMessage = ZenUnit::Random<string>();
   //
   _deleteDirectorySubProgram.ParallelExceptionHandler(exceptionClassNameAndMessage);
   //
   const string expectedErrorMessage = "Error: " + string(exceptionClassNameAndMessage);
   METALMOCK(p_consoleMock->ProgramNameThreadIdWriteLineColorMock.CalledOnceWith(expectedErrorMessage, Color::Red));
}

RUN_TESTS(DeleteDirectorySubProgramTests)
