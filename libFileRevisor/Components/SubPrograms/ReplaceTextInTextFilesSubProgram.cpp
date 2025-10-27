#include "pch.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/Iteration/Math/OneArgMemberFunctionAccumulator.h"
#include "libFileRevisor/Components/Strings/Pluralizer.h"
#include "libFileRevisor/Components/Strings/TextReplacer.h"
#include "libFileRevisor/Components/SubPrograms/ReplaceTextInTextFilesSubProgram.h"

ReplaceTextInTextFilesSubProgram::ReplaceTextInTextFilesSubProgram()
   // Function Pointers
   : _call_PrintReadingFileMessageIfVerboseMode(make_unique<_call_PrintReadingFileMessageIfVerboseModeType>())
   // Function Callers
   , _memberFunctionAccumulator_ReplaceTextInTextFile(make_unique<_memberFunctionAccumulator_ReplaceTextInTextFileType>())
   // Constant Components
   , _textReplacer(make_unique<TextReplacer>())
   // Mutable Components
   , _directoryIterator(make_unique<DirectoryIterator>())
{
}

ReplaceTextInTextFilesSubProgram::~ReplaceTextInTextFilesSubProgram()
{
}

int ReplaceTextInTextFilesSubProgram::Run() const
{
   _directoryIterator->SetDirectoryIterator(p_args.targetFolderPath, p_args.recurse);
   static const vector<string> fileAndFolderPathIgnoreSubstrings =
   {
      ".git",
      ".mypy_cache",
      ".p7s",
      ".vs\\",
      "CMakeFiles",
      "CoberturaCodeCoverageResults_",
      "ReportGenerator_"
      // Windows
      "__Instrumented",
      "AltCoverCodeCoverageResults_",
      "\\Debug\\",
      "\\Release\\",
      "\\RelWithDebInfo\\",
   };
   _directoryIterator->SetFileAndFolderPathIgnoreSubstrings(fileAndFolderPathIgnoreSubstrings);

   const vector<fs::path> nonEmptyNonIgnoredTextFilePathsInTargetDirectory = _directoryIterator->GetNonEmptyNonIgnoredTextFilePaths();

   const size_t numberOfFilesThatWereOrWouldBeModified = _memberFunctionAccumulator_ReplaceTextInTextFile->SumElementsWithFunction(
      nonEmptyNonIgnoredTextFilePathsInTargetDirectory, this, &ReplaceTextInTextFilesSubProgram::ReplaceTextInTextFile);

   const string fileOrFiles = p_pluralizer->PotentiallyPluralizeWord(numberOfFilesThatWereOrWouldBeModified, "file", "files");
   if (p_args.dryrun)
   {
      const string message = String::ConcatValues("DryRun: Would replace text in ", numberOfFilesThatWereOrWouldBeModified, " ", fileOrFiles);
      p_console->ProgramNameThreadIdWriteLine(message);
   }
   else
   {
      const string message = String::ConcatValues("Result: Replaced text in ", numberOfFilesThatWereOrWouldBeModified, " ", fileOrFiles);
      p_console->ProgramNameThreadIdWriteLine(message);
   }
   return 0;
}

size_t ReplaceTextInTextFilesSubProgram::ReplaceTextInTextFile(const fs::path& textFilePath) const
{
   _call_PrintReadingFileMessageIfVerboseMode->CallConstMemberFunction(
      this, &ReplaceTextInTextFilesSubProgram::PrintReadingFileMessageIfVerboseIsTrue, p_args.verbose, textFilePath);
   const string textFileText = p_fileSystem->ReadText(textFilePath);
   const string replacedTextFileText = _textReplacer->ReplaceText(textFileText, p_args.fromRegexPattern, p_args.toRegexPattern);
   if (replacedTextFileText != textFileText)
   {
      if (p_args.dryrun)
      {
         const string wouldReplaceTextInFileMessage = "DryRun: Would replace text in file " + textFilePath.string();
         p_console->ProgramNameThreadIdWriteLine(wouldReplaceTextInFileMessage);
         return 1;
      }
      p_fileSystem->CreateTextFile(textFilePath, replacedTextFileText);
      const string replacedTextInFileMessage = "Replaced text in file " + textFilePath.string();
      p_console->ProgramNameThreadIdWriteLine(replacedTextInFileMessage);
      return 1;
   }
   return 0;
}

void ReplaceTextInTextFilesSubProgram::PrintReadingFileMessageIfVerboseIsTrue(bool verbose, const fs::path& textFilePath) const
{
   if (verbose)
   {
      const string readingFileMessage = "Verbose: Reading file " + textFilePath.string();
      p_console->ProgramNameThreadIdWriteLine(readingFileMessage);
   }
}
