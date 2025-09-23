#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/SubPrograms/RenameFilesSubProgram.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/Iteration/Counter/PredicateCounter.h"
#include "libFileRevisor/Components/Iteration/Transform/OneArgMemberFunctionTransformer.h"
#include "libFileRevisor/Components/Strings/Pluralizer.h"
#include "libFileRevisor/Components/Strings/TextReplacer.h"

RenameFilesSubProgram::RenameFilesSubProgram()
   // Function Callers
   : _caller_PrintDidNotMatchFileMessageIfVerboseMode(make_unique<_caller_PrintDidNotMatchFileMessageIfVerboseModeType>())
   , _transformer_RenameFileIfFileNameMatchesFromPattern(make_unique<_transformer_RenameFileIfFileNameMatchesFromPatternType>())
   // Constant Components
   , _predicateCounter(make_unique<PredicateCounter<RenameResult>>())
   , _textReplacer(make_unique<TextReplacer>())
{
}

RenameFilesSubProgram::~RenameFilesSubProgram()
{
}

int RenameFilesSubProgram::Run() const
{
   const vector<fs::path> filePathsInAndPossiblyBelowDirectory =
      p_fileSystem->GetFilePathsInDirectory(p_args.targetFolderPath, p_args.recurse);
   const vector<RenameResult> fileRenameResults = _transformer_RenameFileIfFileNameMatchesFromPattern->Transform(
      filePathsInAndPossiblyBelowDirectory, this, &RenameFilesSubProgram::RenameFileIfFileNameMatchesFromPattern);
   const size_t numberOfRenamedFiles = _predicateCounter->CountWhere(fileRenameResults, DidRenameFileIsTrue);
   string renamedFilesMessage;
   const string fileOrFiles = p_pluralizer->PotentiallyPluralizeWord(numberOfRenamedFiles, "file", "files");
   if (p_args.dryrun)
   {
      renamedFilesMessage = String::ConcatValues("Result: Would rename ", numberOfRenamedFiles, ' ', fileOrFiles);
   }
   else
   {
      renamedFilesMessage = String::ConcatValues("Result: Renamed ", numberOfRenamedFiles, ' ', fileOrFiles);
   }
   p_console->ProgramNameThreadIdWriteLine(renamedFilesMessage);
   return 0;
}

bool RenameFilesSubProgram::DidRenameFileIsTrue(const RenameResult& fileRenameResult)
{
   return fileRenameResult.didRenameFileOrDirectory;
}

RenameResult RenameFilesSubProgram::RenameFileIfFileNameMatchesFromPattern(const fs::path& filePath) const
{
   const string fileName = filePath.filename().string();
   const string regexReplacedFileName = _textReplacer->ReplaceText(fileName, p_args.fromRegexPattern, p_args.toRegexPattern);
   if (regexReplacedFileName == fileName)
   {
      _caller_PrintDidNotMatchFileMessageIfVerboseMode->CallConstMemberFunction(
         this, &RenameFilesSubProgram::PrintDidNotMatchFileMessageIfVerboseMode, p_args.verbose, filePath);
      return RenameResult(false, filePath, filePath);
   }
   if (p_args.dryrun)
   {
      const string wouldRenameMessage = String::ConcatStrings("DryRun: Would rename file ", filePath.string(), " to ", regexReplacedFileName);
      p_console->ProgramNameThreadIdWriteLine(wouldRenameMessage);
      const fs::path sourceFolderPath = filePath.parent_path();
      const fs::path renamedFilePath = sourceFolderPath / regexReplacedFileName;
      return RenameResult(true, filePath, renamedFilePath);
   }
   const fs::path renamedFilePath = p_fileSystem->RenameFile(filePath, regexReplacedFileName);
   const string renamedFileMessage = String::ConcatStrings("Renamed file ", filePath.string(), " to ", regexReplacedFileName);
   p_console->ProgramNameThreadIdWriteLine(renamedFileMessage);
   return RenameResult(true, filePath, renamedFilePath);
}

void RenameFilesSubProgram::PrintDidNotMatchFileMessageIfVerboseMode(bool verbose, const fs::path& filePath) const
{
   if (verbose)
   {
      const string didNotMatchFileMessage = "Verbose: Did not match " + filePath.string();
      p_console->ProgramNameThreadIdWriteLine(didNotMatchFileMessage);
   }
}
