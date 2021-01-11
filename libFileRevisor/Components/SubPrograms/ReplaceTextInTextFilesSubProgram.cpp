#include "pch.h"
#include "libFileRevisor/Components/Console/Console.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/Iteration/Math/OneExtraArgMemberFunctionSumator.h"
#include "libFileRevisor/Components/Strings/Pluralizer.h"
#include "libFileRevisor/Components/Strings/Regexer.h"
#include "libFileRevisor/Components/SubPrograms/ReplaceTextInTextFilesSubProgram.h"

ReplaceTextInTextFilesSubProgram::ReplaceTextInTextFilesSubProgram()
   // Constant Components
   : _regexer(make_unique<Regexer>())
   // Function Callers
   , _call_PrintReadingFileMessageIfVerboseMode(make_unique<VoidTwoArgMemberFunctionCaller<ReplaceTextInTextFilesSubProgram, bool, const fs::path&>>())
   , _memberFunctionSumator_RegexReplaceTextInTextFile(make_unique<OneExtraArgMemberFunctionSumatorType>())
{
}

int ReplaceTextInTextFilesSubProgram::Run(const FileRevisorArgs& args) const
{
   const vector<fs::path> nonEmptyTextFilePathsInTargetDirectory =
      _protected_fileSystem->GetNonEmptyTextFilePathsInDirectory(args.targetDirectoryPath, args.recurse);
   const size_t numberOfFilesThatWereOrWouldBeModified = _memberFunctionSumator_RegexReplaceTextInTextFile->SumElementsWithFunction(
      this, nonEmptyTextFilePathsInTargetDirectory, &ReplaceTextInTextFilesSubProgram::RegexReplaceTextInTextFile, args);
   const string fileOrFiles = _protected_pluralizer->PotentiallyPluralizeWord(numberOfFilesThatWereOrWouldBeModified, "file", "files");
   if (args.preview)
   {
      const string message = String::Concat(
         "[FileRevisor] PreviewResult: Would replace text in ", numberOfFilesThatWereOrWouldBeModified, " ", fileOrFiles);
      _protected_console->WriteLine(message);
   }
   else
   {
      const string message = String::Concat(
         "[FileRevisor] Result: Replaced text in ", numberOfFilesThatWereOrWouldBeModified, " ", fileOrFiles);
      _protected_console->WriteLine(message);
   }
   return 0;
}

size_t ReplaceTextInTextFilesSubProgram::
RegexReplaceTextInTextFile(const fs::path& textFilePath, const FileRevisorArgs& args) const
{
   _call_PrintReadingFileMessageIfVerboseMode->ConstCall(
      this, &ReplaceTextInTextFilesSubProgram::PrintReadingFileMessageIfVerboseIsTrue, args.verbose, textFilePath);
   const string textFileText = _protected_fileSystem->ReadText(textFilePath);
   const string regexReplacedTextFileText = _regexer->Replace(textFileText, args.fromRegexPattern, args.toRegexPattern);
   if (regexReplacedTextFileText != textFileText)
   {
      if (args.preview)
      {
         const string wouldReplaceTextInFileMessage = "[FileRevisor] Preview: Would replace text in file " + textFilePath.string();
         _protected_console->WriteLine(wouldReplaceTextInFileMessage);
         return 1;
      }
      _protected_fileSystem->CreateTextFile(textFilePath, regexReplacedTextFileText);
      const string replacedTextInFileMessage = "[FileRevisor] Replaced: Text in file " + textFilePath.string();
      _protected_console->WriteLine(replacedTextInFileMessage);
      return 1;
   }
   return 0;
}

void ReplaceTextInTextFilesSubProgram::
PrintReadingFileMessageIfVerboseIsTrue(bool verbose, const fs::path& textFilePath) const
{
   if (verbose)
   {
      const string readingFileMessage = "[FileRevisor]  Verbose: Reading file " + textFilePath.string();
      _protected_console->WriteLine(readingFileMessage);
   }
}
