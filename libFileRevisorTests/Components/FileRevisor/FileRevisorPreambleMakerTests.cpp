#include "pch.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorPreambleMaker.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"

TESTS(FileRevisorPreambleMakerTests)
AFACT(DefaultConstructor_NewsComponents)
FACTS(PrintPreambleLines_PrintsPreambleLines)
EVIDENCE

FileRevisorPreambleMaker _fileRevisorPreambleMaker;
// Constant Components
ConsoleMock* _consoleMock = nullptr;
FileSystemMock* _fileSystemMock = nullptr;

STARTUP
{
   // Constant Components
   _fileRevisorPreambleMaker._console.reset(_consoleMock = new ConsoleMock);
   _fileRevisorPreambleMaker._fileSystem.reset(_fileSystemMock = new FileSystemMock);
}

TEST(DefaultConstructor_NewsComponents)
{
   FileRevisorPreambleMaker fileRevisorPreambleMaker;
   // Constant Components
   DELETE_TO_ASSERT_NEWED(fileRevisorPreambleMaker._console);
   DELETE_TO_ASSERT_NEWED(fileRevisorPreambleMaker._fileSystem);
}

TEST3X3(PrintPreambleLines_PrintsPreambleLines,
   bool dryrun, bool verbose, string_view expectedActionSuffix,
   false, false, "",
   true, false, " DryRun",
   false, true, " Verbose",
   true, true, " DryRun Verbose")
{
   const fs::path currentFolderPath = _fileSystemMock->CurrentFolderPathMock.ReturnRandom();
   _consoleMock->ProgramNameThreadIdWriteLineMock.Expect();
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.verbose = verbose;
   args.dryrun = dryrun;
   //
   _fileRevisorPreambleMaker.PrintPreambleLines(args);
   //
   METALMOCK(_fileSystemMock->CurrentFolderPathMock.CalledOnce());
   const string expectedRunningLine = String::ConcatStrings("Running: ", args.commandLine);
   const string expectedProgramModeString = ENUM_AS_STRING(ProgramMode, args.programMode);
   const string expectedProgramModeLine = String::ConcatValues("ProgramMode: ", expectedProgramModeString, expectedActionSuffix);
   const string expectedWorkingDirectoryLine = String::ConcatStrings("WorkingDirectory: ", currentFolderPath.string());
   const string expectedTargetDirectoryLine = String::ConcatStrings(" TargetDirectory: ", args.targetFolderPath.string());
   METALMOCK(_consoleMock->ProgramNameThreadIdWriteLineMock.CalledNTimes(4));
   METALMOCKTHEN(_consoleMock->ProgramNameThreadIdWriteLineMock.CalledWith(expectedRunningLine)).Then(
   METALMOCKTHEN(_consoleMock->ProgramNameThreadIdWriteLineMock.CalledWith(expectedProgramModeLine))).Then(
   METALMOCKTHEN(_consoleMock->ProgramNameThreadIdWriteLineMock.CalledWith(expectedWorkingDirectoryLine))).Then(
   METALMOCKTHEN(_consoleMock->ProgramNameThreadIdWriteLineMock.CalledWith(expectedTargetDirectoryLine)));
}

RUN_TESTS(FileRevisorPreambleMakerTests)
