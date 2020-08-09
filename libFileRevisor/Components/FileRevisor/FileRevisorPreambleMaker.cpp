#include "pch.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorPreambleMaker.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"

FileRevisorPreambleMaker::FileRevisorPreambleMaker()
   : _fileSystem(make_unique<FileSystem>())
{
}

string FileRevisorPreambleMaker::MakePreambleLines(const FileRevisorArgs& args) const
{
   const fs::path currentDirectoryPath = _fileSystem->CurrentDirectoryPath();
   const string programModeString = ENUM_TO_STRING(ProgramMode, args.programMode);
   const string verboseOrEmptyString = args.verbose ? " Verbose" : "";
   const string previewOrEmptyString = args.preview ? " Preview" : "";
   const string fileRevisorPreambleLines = String::Concat(
      "[FileRevisor] Running: ", args.commandLine, '\n',
      "[FileRevisor] ProgramMode: ", programModeString, verboseOrEmptyString, previewOrEmptyString, '\n',
      "[FileRevisor] WorkingDirectory: ", currentDirectoryPath.string(), '\n',
      "[FileRevisor]  TargetDirectory: ", args.targetDirectoryPath.string());
   return fileRevisorPreambleLines;
}
