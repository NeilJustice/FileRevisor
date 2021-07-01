#include "pch.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorPreambleMaker.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"

FileRevisorPreambleMaker::FileRevisorPreambleMaker()
   // Constant Components
   : _fileSystem(make_unique<FileSystem>())
{
}

FileRevisorPreambleMaker::~FileRevisorPreambleMaker()
{
}

string FileRevisorPreambleMaker::MakePreambleLines(const FileRevisorArgs& args) const
{
   const fs::path currentDirectoryPath = _fileSystem->CurrentDirectoryPath();
   const string programModeString = ENUM_AS_STRING(ProgramMode, args.programMode);
   const string verboseOrEmptyString = args.verbose ? " Verbose" : "";
   const string previewOrEmptyString = args.preview ? " Preview" : "";
   string fileRevisorPreambleLines = String::ConcatStrings(
      "[FileRevisor] Running: ", args.commandLine, "\n",
      "[FileRevisor] ProgramMode: ", programModeString, verboseOrEmptyString, previewOrEmptyString, "\n",
      "[FileRevisor] WorkingDirectory: ", currentDirectoryPath.string(), "\n",
      "[FileRevisor]  TargetDirectory: ", args.targetDirectoryPath.string());
   return fileRevisorPreambleLines;
}
