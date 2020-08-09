#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/RenameFilesSubProgram.h"
#include "libFileRevisor/Components/Strings/Regexer.h"
#include "libFileRevisor/Components/Iteration/Counter/PredicateCounter.h"
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"
#include "libFileRevisor/ValueTypes/RenameResult.h"

RenameFilesSubProgram::RenameFilesSubProgram()
   : _oneExtraArgMemberFunctionTransformer(make_unique<OneExtraArgMemberFunctionTransformerType>())
   , _regexer(make_unique<Regexer>())
   , _caller_PrintDidNotMatchFileMessageIfVerboseMode(make_unique<VoidTwoArgMemberFunctionCaller<
      RenameFilesSubProgram, bool, const fs::path& >>())
   , _predicateCounter(make_unique<PredicateCounter<vector<RenameResult>, RenameResult>>())
{
}

int RenameFilesSubProgram::Run(const FileRevisorArgs& args) const
{
   const vector<fs::path> filePathsInAndPossiblyBelowDirectory =
      _protected_fileSystem->GetFilePathsInDirectory(args.targetDirectoryPath, args.recursive);
   const vector<RenameResult> fileRenameResults = _oneExtraArgMemberFunctionTransformer->Transform(
      filePathsInAndPossiblyBelowDirectory, this, &RenameFilesSubProgram::RenameFileIfFileNameMatchesFromPattern, args);
   const size_t numberOfRenamedFiles = _predicateCounter->CountWhere(fileRenameResults, DidRenameFileIsTrue);
   string renamedFilesMessage;
   const string fileOrFiles = _protected_pluralizer->PotentiallyPluralizeWord(numberOfRenamedFiles, "file", "files");
   if (args.preview)
   {
      renamedFilesMessage = String::Concat(
         "[FileRevisor] Result: Would rename ", numberOfRenamedFiles, ' ', fileOrFiles);
   }
   else
   {
      renamedFilesMessage = String::Concat(
         "[FileRevisor] Result: Renamed ", numberOfRenamedFiles, ' ', fileOrFiles);
   }
   _protected_console->WriteLine(renamedFilesMessage);
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
   if (args.preview)
   {
      const string wouldRenameMessage = String::Concat(
         "[FileRevisor]  Preview: Would rename file ", filePath.string(), " to ", regexReplacedFileName);
      _protected_console->WriteLine(wouldRenameMessage);
      const fs::path sourceDirectoryPath = filePath.parent_path();
      const fs::path renamedFilePath = sourceDirectoryPath / regexReplacedFileName;
      return RenameResult(true, filePath, renamedFilePath);
   }
   const fs::path renamedFilePath = _protected_fileSystem->RenameFile(filePath, regexReplacedFileName);
   const string renamedFileMessage = String::Concat(
      "[FileRevisor] Renamed file ", filePath.string(), " to ", regexReplacedFileName);
   _protected_console->WriteLine(renamedFileMessage);
   return RenameResult(true, filePath, renamedFilePath);
}

void RenameFilesSubProgram::
PrintDidNotMatchFileMessageIfVerboseMode(bool verbose, const fs::path& filePath) const
{
   if (verbose)
   {
      const string didNotMatchFileMessage = "[FileRevisor]  Verbose: Did not match " + filePath.string();
      _protected_console->WriteLine(didNotMatchFileMessage);
   }
}
