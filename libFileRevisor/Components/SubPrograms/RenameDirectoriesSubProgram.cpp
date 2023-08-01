#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/SubPrograms/RenameDirectoriesSubProgram.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/UtilityComponents/Iteration/Counter/PredicateCounter.h"
#include "libFileRevisor/UtilityComponents/Iteration/Transform/OneExtraArgMemberFunctionTransformer.h"
#include "libFileRevisor/UtilityComponents/Strings/Pluralizer.h"
#include "libFileRevisor/UtilityComponents/Strings/Regexer.h"

RenameDirectoriesSubProgram::RenameDirectoriesSubProgram()
   // Function Callers
   : _call_PrintDidNotMatchDirectoryMessageIfVerboseMode(
      make_unique<VoidTwoArgMemberFunctionCaller<RenameDirectoriesSubProgram, bool, const fs::path& >>())
   , _directoryPathsTransformer_RenameDirectory(make_unique<OneExtraArgMemberFunctionTransformerType>())
   // Constant Components
   , _predicateCounter(make_unique<PredicateCounter<RenameResult>>())
   , _regexer(make_unique<Regexer>())
{
}

RenameDirectoriesSubProgram::~RenameDirectoriesSubProgram()
{
}

int RenameDirectoriesSubProgram::Run(const FileRevisorArgs& args) const
{
   const vector<fs::path> directoryPathsInDirectory = _fileSystem->GetDirectoryPathsInDirectory(args.targetDirectoryPath, args.recurse);
   const vector<RenameResult> directoryRenameResults = _directoryPathsTransformer_RenameDirectory->Transform(
      directoryPathsInDirectory, this, &RenameDirectoriesSubProgram::RenameDirectory, args);
   const size_t numberOfRenamedDirectories = _predicateCounter->CountWhere(directoryRenameResults, RenameResult::DidRenameFileOrDirectoryFieldIsTrue);
   const string directoryOrDirectories = _pluralizer->PotentiallyPluralizeWord(numberOfRenamedDirectories, "directory", "directories");
   string numberOfDirectoriesMessage;
   if (args.dryrun)
   {
      numberOfDirectoriesMessage = String::ConcatValues("Result: Would rename ", numberOfRenamedDirectories, ' ', directoryOrDirectories);
   }
   else
   {
      numberOfDirectoriesMessage = String::ConcatValues("Result: Renamed ", numberOfRenamedDirectories, ' ', directoryOrDirectories);
   }
   _console->ThreadIdWriteLine(numberOfDirectoriesMessage);
   return 0;
}

RenameResult RenameDirectoriesSubProgram::RenameDirectory(const fs::path& directoryPath, const FileRevisorArgs& args) const
{
   const string directoryName = directoryPath.filename().string();
   const string regexReplacedDirectoryName = _regexer->Replace(directoryName, args.fromRegexPattern, args.toRegexPattern);
   if (regexReplacedDirectoryName == directoryName)
   {
      _call_PrintDidNotMatchDirectoryMessageIfVerboseMode->CallConstMemberFunction(
         this, &RenameDirectoriesSubProgram::PrintDidNotMatchDirectoryMessageIfVerboseMode, args.verbose, directoryPath);
      return RenameResult(false, directoryPath, directoryPath);
   }
   if (args.dryrun)
   {
      const fs::path parentDirectoryPath = directoryPath.parent_path();
      const fs::path renamedDirectoryPath = parentDirectoryPath / regexReplacedDirectoryName;
      const string wouldRenameMessage = String::ConcatStrings("DryRun: Would rename directory ", directoryPath.string(), " to ", regexReplacedDirectoryName);
      _console->ThreadIdWriteLine(wouldRenameMessage);
      return RenameResult(true, directoryPath, renamedDirectoryPath);
   }
   const fs::path renamedDirectoryPath = _fileSystem->RenameDirectory(directoryPath, regexReplacedDirectoryName);
   const string renamedDirectoryMessage = String::ConcatStrings("Renamed directory ", directoryPath.string(), " to ", regexReplacedDirectoryName);
   _console->ThreadIdWriteLine(renamedDirectoryMessage);
   return RenameResult(true, directoryPath, renamedDirectoryPath);
}

void RenameDirectoriesSubProgram::PrintDidNotMatchDirectoryMessageIfVerboseMode(bool verbose, const fs::path& directoryPath) const
{
   if (verbose)
   {
      const string didNotMatchDirectoryMessage = String::ConcatStrings("Verbose: Did not match ", directoryPath.string());
      _console->ThreadIdWriteLine(didNotMatchDirectoryMessage);
   }
}
