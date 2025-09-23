#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/SubPrograms/RenameFilesSubProgram.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/Iteration/Counter/PredicateCounter.h"
#include "libFileRevisor/Components/Iteration/Transform/OneExtraArgMemberFunctionTransformer.h"
#include "libFileRevisor/Components/Strings/Pluralizer.h"
#include "libFileRevisor/Components/Strings/TextReplacer.h"

RenameFilesSubProgram::RenameFilesSubProgram()
   // Function Callers
   : _caller_PrintDidNotMatchFileMessageIfVerboseMode(make_unique<VoidTwoArgMemberFunctionCaller<RenameFilesSubProgram, bool, const fs::path& >>())
   , _transformer_RenameFileIfFileNameMatchesFromPattern(make_unique<OneExtraArgMemberFunctionTransformerType>())
   // Constant Components
   , _predicateCounter(make_unique<PredicateCounter<RenameResult>>())
   , _textReplacer(make_unique<TextReplacer>())
{
}

RenameFilesSubProgram::~RenameFilesSubProgram()
{
}

int RenameFilesSubProgram::Run(const FileRevisorArgs& args) const
{
   const vector<fs::path> filePathsInAndPossiblyBelowDirectory =
      p_fileSystem->GetFilePathsInDirectory(args.targetFolderPath, args.recurse);
   const vector<RenameResult> fileRenameResults = _transformer_RenameFileIfFileNameMatchesFromPattern->Transform(
      filePathsInAndPossiblyBelowDirectory, this, &RenameFilesSubProgram::RenameFileIfFileNameMatchesFromPattern, args);
   const size_t numberOfRenamedFiles = _predicateCounter->CountWhere(fileRenameResults, DidRenameFileIsTrue);
   string renamedFilesMessage;
   const string fileOrFiles = p_pluralizer->PotentiallyPluralizeWord(numberOfRenamedFiles, "file", "files");
   if (args.dryrun)
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

RenameResult RenameFilesSubProgram::RenameFileIfFileNameMatchesFromPattern(const fs::path& filePath, const FileRevisorArgs& args) const
{
   const string fileName = filePath.filename().string();
   const string regexReplacedFileName = _textReplacer->ReplaceText(fileName, args.fromRegexPattern, args.toRegexPattern);
   if (regexReplacedFileName == fileName)
   {
      _caller_PrintDidNotMatchFileMessageIfVerboseMode->CallConstMemberFunction(
         this, &RenameFilesSubProgram::PrintDidNotMatchFileMessageIfVerboseMode, args.verbose, filePath);
      return RenameResult(false, filePath, filePath);
   }
   if (args.dryrun)
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
