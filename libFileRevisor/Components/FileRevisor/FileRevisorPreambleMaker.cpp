#include "pch.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorPreambleMaker.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"

FileRevisorPreambleMaker::FileRevisorPreambleMaker()
   // Constant Components
   : p_console(make_unique<Console>())
   , p_fileSystem(make_unique<FileSystem>())
{
}

FileRevisorPreambleMaker::~FileRevisorPreambleMaker()
{
}

void FileRevisorPreambleMaker::PrintPreambleLines(const FileRevisorArgs& args) const
{
   const fs::path currentFolderPath = p_fileSystem->CurrentFolderPath();
   const string programModeString = ENUM_AS_STRING(ProgramMode, args.programMode);
   const string dryRunOrEmptyString = args.dryrun ? " DryRun" : "";
   const string verboseOrEmptyString = args.verbose ? " Verbose" : "";

   const string runningLine = Utils::String::ConcatStrings("Running: ", args.commandLine);
   const string programModeLine = Utils::String::ConcatValues("ProgramMode: ", programModeString, dryRunOrEmptyString, verboseOrEmptyString);
   const string workingDirectoryLine = Utils::String::ConcatStrings("WorkingDirectory: ", currentFolderPath.string());
   const string targetDirectoryLine = Utils::String::ConcatStrings(" TargetDirectory: ", args.targetFolderPath.string());

   p_console->ProgramNameThreadIdWriteLine(runningLine);
   p_console->ProgramNameThreadIdWriteLine(programModeLine);
   p_console->ProgramNameThreadIdWriteLine(workingDirectoryLine);
   p_console->ProgramNameThreadIdWriteLine(targetDirectoryLine);
}
