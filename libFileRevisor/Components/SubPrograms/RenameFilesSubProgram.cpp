#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/SubPrograms/RenameFilesSubProgram.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/UtilityComponents/Iteration/Counter/PredicateCounter.h"
#include "libFileRevisor/UtilityComponents/Iteration/Transform/OneExtraArgMemberFunctionTransformer.h"
#include "libFileRevisor/UtilityComponents/Strings/Pluralizer.h"
#include "libFileRevisor/UtilityComponents/Strings/Regexer.h"

RenameFilesSubProgram::RenameFilesSubProgram()
   // Function Callers
   : _caller_PrintDidNotMatchFileMessageIfVerboseMode(make_unique<VoidTwoArgMemberFunctionCaller<
      RenameFilesSubProgram, bool, const fs::path& >>())
   , _transformer_RenameFileIfFileNameMatchesFromPattern(make_unique<OneExtraArgMemberFunctionTransformerType>())
   // Constant Components
   , _predicateCounter(make_unique<PredicateCounter<vector<RenameResult>, RenameResult>>())
   , _regexer(make_unique<Regexer>())
{
}

RenameFilesSubProgram::~RenameFilesSubProgram()
{
}

int RenameFilesSubProgram::Run(const FileRevisorArgs& args) const
{
   const vector<fs::path> filePathsInAndPossiblyBelowDirectory =
      _fileSystem->GetFilePathsInDirectory(args.targetDirectoryPath, args.recurse);
   const vector<RenameResult> fileRenameResults = _transformer_RenameFileIfFileNameMatchesFromPattern->Transform(
      filePathsInAndPossiblyBelowDirectory, this, &RenameFilesSubProgram::RenameFileIfFileNameMatchesFromPattern, args);
   const size_t numberOfRenamedFiles = _predicateCounter->CountWhere(fileRenameResults, DidRenameFileIsTrue);
   string renamedFilesMessage;
   const string fileOrFiles = _pluralizer->PotentiallyPluralizeWord(numberOfRenamedFiles, "file", "files");
   if (args.dryrun)
   {
      renamedFilesMessage = String::ConcatValues("[FileRevisor] Result: Would rename ", numberOfRenamedFiles, ' ', fileOrFiles);
   }
   else
   {
      renamedFilesMessage = String::ConcatValues("[FileRevisor] Result: Renamed ", numberOfRenamedFiles, ' ', fileOrFiles);
   }
   _console->WriteLine(renamedFilesMessage);
   return 0;
}

bool RenameFilesSubProgram::DidRenameFileIsTrue(const RenameResult& fileRenameResult)
{
   return fileRenameResult.didRenameFileOrDirectory;
}

RenameResult RenameFilesSubProgram::
RenameFileIfFileNameMatchesFromPattern(const fs::path& filePath, const FileRevisorArgs& args) const
{
   const string fileName = filePath.filename().string();
   const string regexReplacedFileName = _regexer->Replace(fileName, args.fromRegexPattern, args.toRegexPattern);
   if (regexReplacedFileName == fileName)
   {
      _caller_PrintDidNotMatchFileMessageIfVerboseMode->ConstCall(
         this, &RenameFilesSubProgram::PrintDidNotMatchFileMessageIfVerboseMode, args.verbose, filePath);
      return RenameResult(false, filePath, filePath);
   }
   if (args.dryrun)
   {
      const string wouldRenameMessage = String::ConcatStrings(
         "[FileRevisor]  DryRun: Would rename file ", filePath.string(), " to ", regexReplacedFileName);
      _console->WriteLine(wouldRenameMessage);
      const fs::path sourceDirectoryPath = filePath.parent_path();
      const fs::path renamedFilePath = sourceDirectoryPath / regexReplacedFileName;
      return RenameResult(true, filePath, renamedFilePath);
   }
   const fs::path renamedFilePath = _fileSystem->RenameFile(filePath, regexReplacedFileName);
   const string renamedFileMessage = String::ConcatStrings("[FileRevisor] Renamed file ", filePath.string(), " to ", regexReplacedFileName);
   _console->WriteLine(renamedFileMessage);
   return RenameResult(true, filePath, renamedFilePath);
}

void RenameFilesSubProgram::
PrintDidNotMatchFileMessageIfVerboseMode(bool verbose, const fs::path& filePath) const
{
   if (verbose)
   {
      const string didNotMatchFileMessage = "[FileRevisor]  Verbose: Did not match " + filePath.string();
      _console->WriteLine(didNotMatchFileMessage);
   }
}
