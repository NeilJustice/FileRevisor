#include "pch.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorPreambleMaker.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"

FileRevisorPreambleMaker::FileRevisorPreambleMaker()
   // Constant Components
   : _console(make_unique<Console>())
   , _fileSystem(make_unique<FileSystem>())
{
}

FileRevisorPreambleMaker::~FileRevisorPreambleMaker()
{
}

void FileRevisorPreambleMaker::PrintPreambleLines(const FileRevisorArgs& args) const
{
   const fs::path currentDirectoryPath = _fileSystem->CurrentDirectoryPath();
   const string programModeString = ENUM_AS_STRING(ProgramMode, args.programMode);
   const string dryRunOrEmptyString = args.dryrun ? " DryRun" : "";
   const string verboseOrEmptyString = args.verbose ? " Verbose" : "";

   const string runningLine = String::ConcatStrings("Running: ", args.commandLine);
   const string programModeLine = String::ConcatValues("ProgramMode: ", programModeString, dryRunOrEmptyString, verboseOrEmptyString);
   const string workingDirectoryLine = String::ConcatStrings(" WorkingDirectory: ", currentDirectoryPath.string());
   const string targetDirectoryLine = String::ConcatStrings(" TargetDirectory: ", args.targetDirectoryPath.string());

   _console->ThreadIdWriteLine(runningLine);
   _console->ThreadIdWriteLine(programModeLine);
   _console->ThreadIdWriteLine(workingDirectoryLine);
   _console->ThreadIdWriteLine(targetDirectoryLine);
}
