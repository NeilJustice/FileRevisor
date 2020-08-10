#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"
#include "libFileRevisorTests/Components/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"
#include "libFileRevisorTests/Components/Iteration/ForEach/MetalMock/OneExtraArgMemberForEacherMock.h"
#include "libFileRevisorTests/Components/Iteration/ForEach/MetalMock/ParallelOneExtraArgMemberForEacherMock.h"
#include "libFileRevisorTests/Components/Strings/MetalMock/PluralizerMock.h"

TESTS(DeleteDirectorySubProgramTests)
AFACT(DefaultConstructor_NewsComponents)
AFACT(Run_TargetDirectoryDoesNotExist_WritesDirectoryDoesNotExistInformationalMessage_Returns0)
AFACT(Run_TargetDirectoryExists_ParallelIsFalse_WritesDeletingSequentiallyMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectorySequentiallyThenDeletesTargetDirectory_Returns0)
AFACT(Run_TargetDirectoryExists_ParallelIsTrue_WritesDeletingInParallelMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectoryInParallelThenDeletesTargetDirectory_Returns0)
AFACT(DeleteDirectory_CallsRecursivelyDeleteAllFilesInDirectoryOnDirectoryPath)
EVIDENCE

DeleteDirectorySubProgram _deleteDirectorySubProgram;
ConsoleMock* _protected_consoleMock = nullptr;
FileSystemMock* _protected_fileSystemMock = nullptr;
PluralizerMock* _protected_pluralizerMock = nullptr;

using OneExtraArgMemberForEacherMockType = OneExtraArgMemberForEacherMock<
   string,
   DeleteDirectorySubProgram,
   void (DeleteDirectorySubProgram::*)(const string&, const FileRevisorArgs&) const,
   const FileRevisorArgs&>;
OneExtraArgMemberForEacherMockType* _oneExtraArgMemberForEacher_DeleteDirectoryMock = nullptr;

using ParallelOneExtraArgMemberForEacherMockType = ParallelOneExtraArgMemberForEacherMock<
   string,
   DeleteDirectorySubProgram,
   void (DeleteDirectorySubProgram::*)(const string&, const FileRevisorArgs&) const,
   const FileRevisorArgs&>;
ParallelOneExtraArgMemberForEacherMockType* _parallelOneExtraArgMemberForEacher_DeleteDirectoryMock = nullptr;

STARTUP
{
   _deleteDirectorySubProgram._protected_console.reset(
      _protected_consoleMock = new ConsoleMock);

   _deleteDirectorySubProgram._protected_fileSystem.reset(
      _protected_fileSystemMock = new FileSystemMock);

   _deleteDirectorySubProgram._protected_pluralizer.reset(
      _protected_pluralizerMock = new PluralizerMock);

   _deleteDirectorySubProgram._oneExtraArgMemberForEacher_DeleteDirectory.reset(
      _oneExtraArgMemberForEacher_DeleteDirectoryMock = new OneExtraArgMemberForEacherMockType);

   _deleteDirectorySubProgram._parallelOneExtraArgMemberForEacher_DeleteDirectory.reset(
      _parallelOneExtraArgMemberForEacher_DeleteDirectoryMock = new ParallelOneExtraArgMemberForEacherMockType);
}

TEST(DefaultConstructor_NewsComponents)
{
   DeleteDirectorySubProgram DeleteDirectorySubProgram;
   DELETE_TO_ASSERT_NEWED(DeleteDirectorySubProgram._protected_console);
   DELETE_TO_ASSERT_NEWED(DeleteDirectorySubProgram._protected_fileSystem);
   DELETE_TO_ASSERT_NEWED(DeleteDirectorySubProgram._protected_pluralizer);
   DELETE_TO_ASSERT_NEWED(DeleteDirectorySubProgram._oneExtraArgMemberForEacher_DeleteDirectory);
   DELETE_TO_ASSERT_NEWED(DeleteDirectorySubProgram._parallelOneExtraArgMemberForEacher_DeleteDirectory);
}

TEST(Run_TargetDirectoryDoesNotExist_WritesDirectoryDoesNotExistInformationalMessage_Returns0)
{
   _protected_fileSystemMock->FileOrDirectoryExistsMock.Return(false);
   _protected_consoleMock->WriteLineMock.Expect();
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   const int exitCode = _deleteDirectorySubProgram.Run(args);
   //
   METALMOCK(_protected_fileSystemMock->FileOrDirectoryExistsMock.CalledOnceWith(args.targetDirectoryPath));
   const string expectedDirectoryDoesNotExistMessage =
      "[FileRevisor] Directory does not exist: " + args.targetDirectoryPath.string();
   METALMOCK(_protected_consoleMock->WriteLineMock.CalledOnceWith(expectedDirectoryDoesNotExistMessage));
   IS_ZERO(exitCode);
}

TEST(Run_TargetDirectoryExists_ParallelIsFalse_WritesDeletingSequentiallyMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectorySequentiallyThenDeletesTargetDirectory_Returns0)
{
   _protected_fileSystemMock->FileOrDirectoryExistsMock.Return(true);

   _oneExtraArgMemberForEacher_DeleteDirectoryMock->OneExtraArgMemberForEachMock.Expect();

   const vector<string> topLevelDirectoryPathsInDirectory = ZenUnit::RandomVector<string>();
   _protected_fileSystemMock->GetStringDirectoryPathsInDirectoryMock.Return(topLevelDirectoryPathsInDirectory);

   _protected_fileSystemMock->RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindowsMock.Expect();

   unsigned long long numberOfFilesAndDirectoriesRemoved = _protected_fileSystemMock->RemoveAllMock.ReturnRandom();
   ++numberOfFilesAndDirectoriesRemoved; // Varible usage to supress unused variable warning

   _protected_consoleMock->WriteLineMock.Expect();

   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.parallel = false;
   //
   const int exitCode = _deleteDirectorySubProgram.Run(args);
   //
   METALMOCK(_protected_fileSystemMock->FileOrDirectoryExistsMock.CalledOnceWith(args.targetDirectoryPath));
   METALMOCK(_protected_fileSystemMock->GetStringDirectoryPathsInDirectoryMock.CalledOnceWith(args.targetDirectoryPath, false));
   METALMOCK(_oneExtraArgMemberForEacher_DeleteDirectoryMock->OneExtraArgMemberForEachMock.CalledOnceWith(
      topLevelDirectoryPathsInDirectory, &_deleteDirectorySubProgram,
      &DeleteDirectorySubProgram::DeleteDirectory, args));
   METALMOCK(_protected_fileSystemMock->
      RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindowsMock.CalledOnceWith(args.targetDirectoryPath));
   METALMOCK(_protected_fileSystemMock->RemoveAllMock.CalledOnceWith(args.targetDirectoryPath));
   const string expectedDeletedDirectoryMessage =
      "[FileRevisor] Deleted directory " + args.targetDirectoryPath.string();
   METALMOCK(_protected_consoleMock->WriteLineMock.CalledOnceWith(expectedDeletedDirectoryMessage));
   IS_ZERO(exitCode);
}

TEST(Run_TargetDirectoryExists_ParallelIsTrue_WritesDeletingInParallelMessage_RecursivelyDeletesAllTopLevelDirectoriesInTargetDirectoryInParallelThenDeletesTargetDirectory_Returns0)
{
   _protected_fileSystemMock->FileOrDirectoryExistsMock.Return(true);

   _parallelOneExtraArgMemberForEacher_DeleteDirectoryMock->ParallelOneExtraArgMemberForEachMock.Expect();

   const vector<string> topLevelDirectoryPathsInDirectory = ZenUnit::RandomVector<string>();
   _protected_fileSystemMock->GetStringDirectoryPathsInDirectoryMock.Return(topLevelDirectoryPathsInDirectory);

   _protected_fileSystemMock->RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindowsMock.Expect();

   unsigned long long numberOfFilesAndDirectoriesRemoved = _protected_fileSystemMock->RemoveAllMock.ReturnRandom();
   ++numberOfFilesAndDirectoriesRemoved; // Varible usage to supress unused variable warning

   _protected_consoleMock->WriteLineMock.Expect();

   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.parallel = true;
   //
   const int exitCode = _deleteDirectorySubProgram.Run(args);
   //
   METALMOCK(_protected_fileSystemMock->FileOrDirectoryExistsMock.CalledOnceWith(args.targetDirectoryPath));
   METALMOCK(_protected_fileSystemMock->GetStringDirectoryPathsInDirectoryMock.CalledOnceWith(args.targetDirectoryPath, false));
   METALMOCK(_parallelOneExtraArgMemberForEacher_DeleteDirectoryMock->ParallelOneExtraArgMemberForEachMock.CalledOnceWith(
      topLevelDirectoryPathsInDirectory, &_deleteDirectorySubProgram,
      &DeleteDirectorySubProgram::DeleteDirectory, args));
   METALMOCK(_protected_fileSystemMock->
      RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindowsMock.CalledOnceWith(args.targetDirectoryPath));
   METALMOCK(_protected_fileSystemMock->RemoveAllMock.CalledOnceWith(args.targetDirectoryPath));
   const string expectedDeletedDirectoryMessage = "[FileRevisor] Deleted directory " + args.targetDirectoryPath.string();
   const string expectedDeletingInParallelMessage = "[FileRevisor] Deleting in parallel all files in directory: " + args.targetDirectoryPath.string();
   METALMOCK(_protected_consoleMock->WriteLineMock.CalledAsFollows(
   {
      { expectedDeletingInParallelMessage },
      { expectedDeletedDirectoryMessage }
   }));
   IS_ZERO(exitCode);
}

TEST(DeleteDirectory_CallsRecursivelyDeleteAllFilesInDirectoryOnDirectoryPath)
{
   _protected_fileSystemMock->RecursivelyDeleteAllFilesInDirectoryMock.Expect();
   const string directoryPath = ZenUnit::Random<string>();
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   _deleteDirectorySubProgram.DeleteDirectory(directoryPath, args);
   //
   METALMOCK(_protected_fileSystemMock->RecursivelyDeleteAllFilesInDirectoryMock.CalledOnceWith(directoryPath, args));
}

RUN_TESTS(DeleteDirectorySubProgramTests)
