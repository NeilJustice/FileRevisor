#include "pch.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"
#include "libFileRevisor/Components/FileSystem/FileOpenerCloser.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/SubPrograms/ReplaceTextInTextFilesSubProgram.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/UtilityComponents/Iteration/Math/OneExtraArgMemberFunctionAccumulator.h"
#include "libFileRevisor/UtilityComponents/Strings/Pluralizer.h"
#include "libFileRevisor/UtilityComponents/Strings/Regexer.h"

ReplaceTextInTextFilesSubProgram::ReplaceTextInTextFilesSubProgram()
   // Function Pointers
   : _call_PrintReadingFileMessageIfVerboseMode(make_unique<VoidTwoArgMemberFunctionCaller<ReplaceTextInTextFilesSubProgram, bool, const fs::path&>>())
   // Function Callers
   , _memberFunctionAccumulator_RegexReplaceTextInTextFile(make_unique<OneExtraArgMemberFunctionAccumulatorType>())
   // Constant Components
   , _regexer(make_unique<Regexer>())
   // Mutable Components
   , _directoryIterator(make_unique<DirectoryIterator>())
{
}

ReplaceTextInTextFilesSubProgram::~ReplaceTextInTextFilesSubProgram()
{
}

int ReplaceTextInTextFilesSubProgram::Run(const FileRevisorArgs& args) const
{
   _directoryIterator->SetDirectoryIterator(args.targetDirectoryPath, args.recurse);
#if defined __linux__ || defined __APPLE__
   static const vector<string> fileAndDirectoryPathIgnoreSubstrings = { "/.git/", ".p7s" };
#elif defined _WIN32
   static const vector<string> fileAndDirectoryPathIgnoreSubstrings = { "\\.git\\", ".p7s" };
#endif
   _directoryIterator->SetFileAndDirectoryPathIgnoreSubstrings(fileAndDirectoryPathIgnoreSubstrings);
   const vector<fs::path> nonEmptyNonIgnoredTextFilePathsInTargetDirectory = _directoryIterator->GetNonEmptyNonIgnoredTextFilePaths();
   const size_t numberOfFilesThatWereOrWouldBeModified = _memberFunctionAccumulator_RegexReplaceTextInTextFile->SumElementsWithFunction(
      this, nonEmptyNonIgnoredTextFilePathsInTargetDirectory, &ReplaceTextInTextFilesSubProgram::RegexReplaceTextInTextFile, args);
   const string fileOrFiles = _pluralizer->PotentiallyPluralizeWord(numberOfFilesThatWereOrWouldBeModified, "file", "files");
   if (args.dryrun)
   {
      const string message = String::ConcatValues("DryRun: Would replace text in ", numberOfFilesThatWereOrWouldBeModified, " ", fileOrFiles);
      _console->ThreadIdWriteLine(message);
   }
   else
   {
      const string message = String::ConcatValues("Result: Replaced text in ", numberOfFilesThatWereOrWouldBeModified, " ", fileOrFiles);
      _console->ThreadIdWriteLine(message);
   }
   return 0;
}

size_t ReplaceTextInTextFilesSubProgram::RegexReplaceTextInTextFile(const fs::path& textFilePath, const FileRevisorArgs& args) const
{
   _call_PrintReadingFileMessageIfVerboseMode->CallConstMemberFunction(
      this, &ReplaceTextInTextFilesSubProgram::PrintReadingFileMessageIfVerboseIsTrue, args.verbose, textFilePath);
   const string textFileText = _fileSystem->ReadText(textFilePath);
   const string regexReplacedTextFileText = _regexer->Replace(textFileText, args.fromRegexPattern, args.toRegexPattern);
   if (regexReplacedTextFileText != textFileText)
   {
      if (args.dryrun)
      {
         const string wouldReplaceTextInFileMessage = "DryRun: Would replace text in file " + textFilePath.string();
         _console->ThreadIdWriteLine(wouldReplaceTextInFileMessage);
         return 1;
      }
      _fileSystem->CreateTextFile(textFilePath, regexReplacedTextFileText);
      const string replacedTextInFileMessage = "Replaced: Text in file " + textFilePath.string();
      _console->ThreadIdWriteLine(replacedTextInFileMessage);
      return 1;
   }
   return 0;
}

void ReplaceTextInTextFilesSubProgram::PrintReadingFileMessageIfVerboseIsTrue(bool verbose, const fs::path& textFilePath) const
{
   if (verbose)
   {
      const string readingFileMessage = "Verbose: Reading file " + textFilePath.string();
      _console->ThreadIdWriteLine(readingFileMessage);
   }
}
