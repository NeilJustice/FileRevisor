#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/SubPrograms/RenameDirectoriesSubProgram.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/Iteration/Counter/PredicateCounter.h"
#include "libFileRevisor/Components/Iteration/Transform/OneArgMemberFunctionTransformer.h"
#include "libFileRevisor/Components/Strings/Pluralizer.h"
#include "libFileRevisor/Components/Strings/TextReplacer.h"

RenameDirectoriesSubProgram::RenameDirectoriesSubProgram()
   // Function Callers
   : _call_PrintDidNotMatchDirectoryMessageIfVerboseMode(make_unique<_call_PrintDidNotMatchDirectoryMessageIfVerboseModeType>())
   , _directoryPathsTransformer_RenameDirectory(make_unique<_directoryPathsTransformer_RenameDirectoryType>())
   // Constant Components
   , _predicateCounter(make_unique<PredicateCounter<RenameResult>>())
   , _textReplacer(make_unique<TextReplacer>())
{
}

RenameDirectoriesSubProgram::~RenameDirectoriesSubProgram()
{
}

int RenameDirectoriesSubProgram::Run() const
{
   const vector<fs::path> directoryPathsInDirectory = p_fileSystem->GetFolderPathsInDirectory(p_args.targetFolderPath, p_args.recurse);

   const vector<RenameResult> directoryRenameResults = _directoryPathsTransformer_RenameDirectory->Transform(
      directoryPathsInDirectory, this, &RenameDirectoriesSubProgram::RenameDirectory);

   const size_t numberOfRenamedDirectories = _predicateCounter->CountWhere(directoryRenameResults, RenameResult::DidRenameFileOrDirectoryFieldIsTrue);

   const string directoryOrDirectories = p_pluralizer->PotentiallyPluralizeWord(numberOfRenamedDirectories, "directory", "directories");
   string numberOfDirectoriesMessage;
   if (p_args.dryrun)
   {
      numberOfDirectoriesMessage = String::ConcatValues("Result: Would rename ", numberOfRenamedDirectories, ' ', directoryOrDirectories);
   }
   else
   {
      numberOfDirectoriesMessage = String::ConcatValues("Result: Renamed ", numberOfRenamedDirectories, ' ', directoryOrDirectories);
   }
   p_console->ProgramNameThreadIdWriteLine(numberOfDirectoriesMessage);
   return 0;
}

RenameResult RenameDirectoriesSubProgram::RenameDirectory(const fs::path& directoryPath) const
{
   const string directoryName = directoryPath.filename().string();
   const string regexReplacedDirectoryName = _textReplacer->ReplaceText(directoryName, p_args.fromRegexPattern, p_args.toRegexPattern);
   if (regexReplacedDirectoryName == directoryName)
   {
      _call_PrintDidNotMatchDirectoryMessageIfVerboseMode->CallConstMemberFunction(
         this, &RenameDirectoriesSubProgram::PrintDidNotMatchDirectoryMessageIfVerboseMode, p_args.verbose, directoryPath);
      return RenameResult(false, directoryPath, directoryPath);
   }
   if (p_args.dryrun)
   {
      const fs::path parentFolderPath = directoryPath.parent_path();
      const fs::path renamedFolderPath = parentFolderPath / regexReplacedDirectoryName;
      const string wouldRenameMessage = String::ConcatStrings("DryRun: Would rename directory ", directoryPath.string(), " to ", regexReplacedDirectoryName);
      p_console->ProgramNameThreadIdWriteLine(wouldRenameMessage);
      return RenameResult(true, directoryPath, renamedFolderPath);
   }
   const fs::path renamedFolderPath = p_fileSystem->RenameDirectory(directoryPath, regexReplacedDirectoryName);
   const string renamedDirectoryMessage = String::ConcatStrings("Renamed directory ", directoryPath.string(), " to ", regexReplacedDirectoryName);
   p_console->ProgramNameThreadIdWriteLine(renamedDirectoryMessage);
   return RenameResult(true, directoryPath, renamedFolderPath);
}

void RenameDirectoriesSubProgram::PrintDidNotMatchDirectoryMessageIfVerboseMode(bool verbose, const fs::path& directoryPath) const
{
   if (verbose)
   {
      const string didNotMatchDirectoryMessage = String::ConcatStrings("Verbose: Did not match ", directoryPath.string());
      p_console->ProgramNameThreadIdWriteLine(didNotMatchDirectoryMessage);
   }
}
