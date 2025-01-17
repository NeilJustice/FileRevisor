#include "pch.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorArgsParser.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorPreambleMaker.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/Utility/Docopt/DocoptParser.h"
#include "libFileRevisor/Components/Utility/Docopt/docopt.h"
#include "libFileRevisor/Components/Utility/FunctionCallers/Member/NonVoidTwoArgMemberFunctionCaller.h"

FileRevisorArgsParser::FileRevisorArgsParser()
   // Function Pointers
   : _call_DetermineProgramMode(FileRevisorArgsParser::DetermineProgramMode)
   // Function Callers
   , _caller_ParseDirAndFromAndToArguments(make_unique<NonVoidTwoArgMemberFunctionCallerType>())
   // Constant Components
   , _console(make_unique<Console>())
   , _fileSystem(make_unique<FileSystem>())
   , _docoptParser(make_unique<DocoptParser>())
   , _fileRevisorPreambleMaker(make_unique<FileRevisorPreambleMaker>())
{
}

FileRevisorArgsParser::~FileRevisorArgsParser()
{
}

FileRevisorArgs FileRevisorArgsParser::ParseArgs(const vector<string>& stringArgs) const
{
   const map<string, docopt::Value> docoptValues = _docoptParser->ParseArgs(FileRevisorArgs::CommandLineUsage, stringArgs);
   const bool isRenameFilesMode = _docoptParser->GetRequiredBool(docoptValues, "rename-files");
   const bool isRenameDirectoriesMode = _docoptParser->GetRequiredBool(docoptValues, "rename-directories");
   const bool isReplaceTextInTextFilesMode = _docoptParser->GetRequiredBool(docoptValues, "replace-text");
   const bool isDeleteDirectoryMode = _docoptParser->GetRequiredBool(docoptValues, "delete-directory");
   FileRevisorArgs args{};
   args.commandLine = Vector::Join(stringArgs, ' ');
   args.programMode = _call_DetermineProgramMode(isRenameFilesMode, isRenameDirectoriesMode, isReplaceTextInTextFilesMode, isDeleteDirectoryMode);
   tuple<fs::path, string, string> targetDirectory_fromRegexPattern_toRegexPattern =
      _caller_ParseDirAndFromAndToArguments->CallConstMemberFunction(
         this, &FileRevisorArgsParser::ParseTargetAndFromAndToArguments, docoptValues, isDeleteDirectoryMode);
   args.targetFolderPath = get<0>(targetDirectory_fromRegexPattern_toRegexPattern);
   args.fromRegexPattern = get<1>(targetDirectory_fromRegexPattern_toRegexPattern);
   args.toRegexPattern = get<2>(targetDirectory_fromRegexPattern_toRegexPattern);
   args.recurse = _docoptParser->GetOptionalBool(docoptValues, "--recurse");
   args.parallel = _docoptParser->GetOptionalBool(docoptValues, "--parallel");
   args.skipFilesInUse = _docoptParser->GetOptionalBool(docoptValues, "--skip-files-in-use");
   args.dryrun = _docoptParser->GetOptionalBool(docoptValues, "--dryrun");
   args.quiet = _docoptParser->GetOptionalBool(docoptValues, "--quiet");
   args.verbose = _docoptParser->GetOptionalBool(docoptValues, "--verbose");
   return args;
}

void FileRevisorArgsParser::PrintPreambleLines(const FileRevisorArgs& args) const
{
   _fileRevisorPreambleMaker->PrintPreambleLines(args);
}

tuple<fs::path, string, string> FileRevisorArgsParser::ParseTargetAndFromAndToArguments(
   const map<string, docopt::Value>& docoptValues, bool isDeleteDirectoryMode) const
{
   string targetFolderPathString;
   string fromRegexPattern;
   string toRegexPattern;
   if (isDeleteDirectoryMode)
   {
      targetFolderPathString = _docoptParser->GetRequiredString(docoptValues, "--target");
   }
   else
   {
      targetFolderPathString = _docoptParser->GetOptionalStringWithDefaultValue(docoptValues, "--target", ".");
      fromRegexPattern = _docoptParser->GetRequiredString(docoptValues, "--from");
      if (fromRegexPattern.empty())
      {
         throw invalid_argument("--from value cannot be empty");
      }
      toRegexPattern = _docoptParser->GetRequiredString(docoptValues, "--to");
   }
   fs::path targetFolderPath = _fileSystem->GetAbsolutePath(targetFolderPathString);
   tuple<fs::path, string, string> targetFolderPath_fromRegexPattern_toRegexPattern(targetFolderPath, fromRegexPattern, toRegexPattern);
   return targetFolderPath_fromRegexPattern_toRegexPattern;
}

ProgramMode FileRevisorArgsParser::DetermineProgramMode(
   bool isRenameFilesMode,
   bool isRenameDirectoriesMode,
   bool isReplaceTextInTextFilesMode,
   bool isDeleteDirectoryMode)
{
   if (isRenameFilesMode)
   {
      return ProgramMode::RenameFiles;
   }
   if (isRenameDirectoriesMode)
   {
      return ProgramMode::RenameDirectories;
   }
   if (isReplaceTextInTextFilesMode)
   {
      return ProgramMode::ReplaceTextInTextFiles;
   }
   if (isDeleteDirectoryMode)
   {
      return ProgramMode::DeleteDirectory;
   }
   throw invalid_argument("All four program mode bools (isRenameFilesMode, isRenameDirectoriesMode, isReplaceTextInTextFilesMode, and isDeleteDirectoryMode) are unexpectedly false");
}
