#include "pch.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorPreambleMaker.h"

TESTS(FileRevisorPreambleMakerTests)
AFACT(DefaultConstructor_NewsComponents)
FACTS(MakePreambleLines_ReturnsExpectedPreambleLinesAsAString)
EVIDENCE

FileRevisorPreambleMaker _fileRevisorPreambleMaker;
FileSystemMock* _fileSystemMock = nullptr;

STARTUP
{
   _fileRevisorPreambleMaker._fileSystem.reset(_fileSystemMock = new FileSystemMock);
}

TEST(DefaultConstructor_NewsComponents)
{
   FileRevisorPreambleMaker fileRevisorPreambleMaker;
   DELETE_TO_ASSERT_NEWED(fileRevisorPreambleMaker._fileSystem);
}

TEST3X3(MakePreambleLines_ReturnsExpectedPreambleLinesAsAString,
   bool verbose, bool preview, string_view expectedActionSuffix,
   false, false, "",
   true, false, " Verbose",
   false, true, " Preview",
   true, true, " Verbose Preview")
{
   const fs::path currentDirectoryPath = _fileSystemMock->CurrentDirectoryPathMock.ReturnRandom();
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.verbose = verbose;
   args.preview = preview;
   //
   const string fileRevisorPreambleLines = _fileRevisorPreambleMaker.MakePreambleLines(args);
   //
   METALMOCK(_fileSystemMock->CurrentDirectoryPathMock.CalledOnce());
   const string expectedActionString = ENUM_TO_STRING(ProgramMode, args.programMode);
   const string expectedFileRevisorPreambleLines = String::Concat(
      "[FileRevisor] Running: ", args.commandLine, '\n',
      "[FileRevisor] ProgramMode: ", expectedActionString, expectedActionSuffix, '\n',
      "[FileRevisor] WorkingDirectory: ", currentDirectoryPath.string(), '\n',
      "[FileRevisor]  TargetDirectory: ", args.targetDirectoryPath.string());
   ARE_EQUAL(expectedFileRevisorPreambleLines, fileRevisorPreambleLines);
}

RUN_TESTS(FileRevisorPreambleMakerTests)
