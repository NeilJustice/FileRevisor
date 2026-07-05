#include "pch.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorPreambleMaker.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"

TESTS(FileRevisorPreambleMakerTests)
FACTS(PrintPreambleLines_PrintsPreambleLines)
EVIDENCE

FileRevisorPreambleMaker _fileRevisorPreambleMaker;
// Constant Components
ConsoleMock* p_consoleMock = nullptr;
FileSystemMock* p_fileSystemMock = nullptr;

STARTUP
{
   // Constant Components
   _fileRevisorPreambleMaker.p_console.reset(p_consoleMock = new ConsoleMock);
   _fileRevisorPreambleMaker.p_fileSystem.reset(p_fileSystemMock = new FileSystemMock);
}

TEST3X3(PrintPreambleLines_PrintsPreambleLines,
   bool dryrun, bool verbose, string_view expectedActionSuffix,
   false, false, "",
   true, false, " DryRun",
   false, true, " Verbose",
   true, true, " DryRun Verbose")
{
   const fs::path currentDirectoryPath = p_fileSystemMock->CurrentDirectoryPathMock.ReturnRandom();
   p_consoleMock->WriteProgramNameThreadIdLineMock.Expect();
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.verbose = verbose;
   args.dryrun = dryrun;
   //
   _fileRevisorPreambleMaker.PrintPreambleLines(args);
   //
   METALMOCK(p_fileSystemMock->CurrentDirectoryPathMock.CalledOnce());
   const string expectedRunningLine = Utils::String::ConcatStrings("Running: ", args.commandLine);
   const string expectedProgramModeString = ENUM_AS_STRING(ProgramMode, args.programMode);
   const string expectedProgramModeLine = Utils::String::ConcatValues("ProgramMode: ", expectedProgramModeString, expectedActionSuffix);
   const string expectedWorkingDirectoryLine = Utils::String::ConcatStrings("WorkingDirectory: ", currentDirectoryPath.string());
   const string expectedTargetDirectoryLine = Utils::String::ConcatStrings(" TargetDirectory: ", args.targetDirectoryPath.string());
   METALMOCK(p_consoleMock->WriteProgramNameThreadIdLineMock.CalledNTimes(4));
   METALMOCKTHEN(p_consoleMock->WriteProgramNameThreadIdLineMock.CalledWith(expectedRunningLine)).Then(
   METALMOCKTHEN(p_consoleMock->WriteProgramNameThreadIdLineMock.CalledWith(expectedProgramModeLine))).Then(
   METALMOCKTHEN(p_consoleMock->WriteProgramNameThreadIdLineMock.CalledWith(expectedWorkingDirectoryLine))).Then(
   METALMOCKTHEN(p_consoleMock->WriteProgramNameThreadIdLineMock.CalledWith(expectedTargetDirectoryLine)));
}

RUN_TESTS(FileRevisorPreambleMakerTests)
