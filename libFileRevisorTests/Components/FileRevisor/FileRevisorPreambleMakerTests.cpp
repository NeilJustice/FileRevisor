#include "pch.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorPreambleMaker.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"

TESTS(FileRevisorPreambleMakerTests)
AFACT(DefaultConstructor_NewsComponents)
FACTS(MakePreambleLines_ReturnsExpectedPreambleLinesAsAString)
EVIDENCE

FileRevisorPreambleMaker _fileRevisorPreambleMaker;
// Constant Components
FileSystemMock* _fileSystemMock = nullptr;

STARTUP
{
   // Constant Components
   _fileRevisorPreambleMaker._fileSystem.reset(_fileSystemMock = new FileSystemMock);
}

TEST(DefaultConstructor_NewsComponents)
{
   FileRevisorPreambleMaker fileRevisorPreambleMaker;
   // Constant Components
   DELETE_TO_ASSERT_NEWED(fileRevisorPreambleMaker._fileSystem);
}

TEST3X3(MakePreambleLines_ReturnsExpectedPreambleLinesAsAString,
   bool verbose, bool dryrun, string_view expectedActionSuffix,
   false, false, "",
   true, false, " Verbose",
   false, true, " DryRun",
   true, true, " Verbose DryRun")
{
   const fs::path currentDirectoryPath = _fileSystemMock->CurrentDirectoryPathMock.ReturnRandom();
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.verbose = verbose;
   args.dryrun = dryrun;
   //
   const string fileRevisorPreambleLines = _fileRevisorPreambleMaker.MakePreambleLines(args);
   //
   METALMOCK(_fileSystemMock->CurrentDirectoryPathMock.CalledOnce());
   const string expectedActionString = ENUM_AS_STRING(ProgramMode, args.programMode);
   const string expectedFileRevisorPreambleLines = String::ConcatStrings(
      "[FileRevisor] Running: ", args.commandLine, "\n",
      "[FileRevisor] ProgramMode: ", expectedActionString, expectedActionSuffix, "\n",
      "[FileRevisor] WorkingDirectory: ", currentDirectoryPath.string(), "\n",
      "[FileRevisor]  TargetDirectory: ", args.targetDirectoryPath.string());
   ARE_EQUAL(expectedFileRevisorPreambleLines, fileRevisorPreambleLines);
}

RUN_TESTS(FileRevisorPreambleMakerTests)
