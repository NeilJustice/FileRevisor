#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/ReplaceTextInTextFilesSubProgram.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/DirectoryIteratorMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"
#include "libFileRevisorTests/Components/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/MetalMock/VoidTwoArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/Iteration/Math/MetalMock/OneArgMemberFunctionAccumulatorMock.h"
#include "libFileRevisorTests/Components/Strings/MetalMock/PluralizerMock.h"
#include "libFileRevisorTests/Components/Strings/MetalMock/TextReplacerMock.h"

TESTS(ReplaceTextInTextFilesSubProgramTests)
FACTS(Run_ReadsTextFilesInWorkingDirectory_CallsReplaceTextInTextFileOnEachTextFilePath_Returns0)
AFACT(ReplaceTextInTextFile_DryRunIsTrueOrFalse_ReplacedFileTextEqualsOriginalFileText_DoesNothing_Returns0)
AFACT(ReplaceTextInTextFile_DryRunIsTrue_ReplacedFileTextDiffersFromOriginalFileText_DoesNotModifyFile_PrintsWouldReplaceTextInFileMessage_Returns1)
AFACT(ReplaceTextInTextFile_DryRunIsFalse_ReplacedFileTextDiffersFromOriginalFileText_WritesReplacedTextToFile_PrintsReplacedTextInFileMessage_Returns1)
AFACT(PrintReadingFileMessageIfVerboseIsTrue_VerboseIsFalse_DoesNothing)
AFACT(PrintReadingFileMessageIfVerboseIsTrue_VerboseIsTrue_PrintsReadingFileMessage)
EVIDENCE

ReplaceTextInTextFilesSubProgram _replaceTextInTextFilesSubProgram;
// Function Pointers
using _call_PrintReadingFileMessageIfVerboseModeMockType = VoidTwoArgMemberFunctionCallerMock<ReplaceTextInTextFilesSubProgram, bool, const fs::path&>;
_call_PrintReadingFileMessageIfVerboseModeMockType* _call_PrintReadingFileMessageIfVerboseModeMock = nullptr;
// Function Callers
using _memberFunctionAccumulator_ReplaceTextInTextFileMockType = OneArgMemberFunctionAccumulatorMock<
   ReplaceTextInTextFilesSubProgram, size_t, fs::path>;
_memberFunctionAccumulator_ReplaceTextInTextFileMockType* _memberFunctionAccumulator_ReplaceTextInTextFileMock = nullptr;
// Base Class Constant Components
ConsoleMock* p_consoleMock = nullptr;
FileSystemMock* p_fileSystemMock = nullptr;
PluralizerMock* p_pluralizerMock = nullptr;
// Constant Components
TextReplacerMock* _textReplacerMock = nullptr;
// Mutable Components
DirectoryIteratorMock* _directoryIteratorMock = nullptr;
// Mutable Fields
FileRevisorArgs p_args;

STARTUP
{
   // Function Pointers
   _replaceTextInTextFilesSubProgram._call_PrintReadingFileMessageIfVerboseMode.reset(_call_PrintReadingFileMessageIfVerboseModeMock = new _call_PrintReadingFileMessageIfVerboseModeMockType);
   // Function Callers
   _replaceTextInTextFilesSubProgram._memberFunctionAccumulator_ReplaceTextInTextFile.reset(_memberFunctionAccumulator_ReplaceTextInTextFileMock = new _memberFunctionAccumulator_ReplaceTextInTextFileMockType);
   // Base Class Constant Components
   _replaceTextInTextFilesSubProgram.p_console.reset(p_consoleMock = new ConsoleMock);
   _replaceTextInTextFilesSubProgram.p_fileSystem.reset(p_fileSystemMock = new FileSystemMock);
   _replaceTextInTextFilesSubProgram.p_pluralizer.reset(p_pluralizerMock = new PluralizerMock);
   // Constant Components
   _replaceTextInTextFilesSubProgram._textReplacer.reset(_textReplacerMock = new TextReplacerMock);
   // Mutable Components
   _replaceTextInTextFilesSubProgram._directoryIterator.reset(_directoryIteratorMock = new DirectoryIteratorMock);
   // Mutable Fields
   _replaceTextInTextFilesSubProgram.p_args = p_args = ZenUnit::Random<FileRevisorArgs>();
}

TEST2X2(Run_ReadsTextFilesInWorkingDirectory_CallsReplaceTextInTextFileOnEachTextFilePath_Returns0,
   bool dryRun, const string& expectedMessagePrefix,
   true,  "DryRun: Would replace text in ",
   false, "Result: Replaced text in ")
{
   _directoryIteratorMock->SetDirectoryIteratorMock.Expect();

   _directoryIteratorMock->SetFileAndFolderPathIgnoreSubstringsMock.Expect();

   const vector<fs::path> nonEmptyNonIgnoredTextFilePathsInTargetDirectory =
      _directoryIteratorMock->GetNonEmptyNonIgnoredTextFilePathsMock.ReturnRandom();

   const size_t numberOfFilesThatWereOrWouldBeModified =
      _memberFunctionAccumulator_ReplaceTextInTextFileMock->SumElementsWithFunctionMock.ReturnRandom();

   const string fileOrFiles = p_pluralizerMock->PotentiallyPluralizeWordMock.ReturnRandom();

   p_consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   _replaceTextInTextFilesSubProgram.p_args.dryrun = dryRun;
   //
   const int exitCode = _replaceTextInTextFilesSubProgram.Run();
   //
   static const vector<string> expectedFileAndFolderPathIgnoreSubstrings =
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
      "\\RelWithDebInfo\\"
   };
   const string expectedMessage = expectedMessagePrefix + to_string(numberOfFilesThatWereOrWouldBeModified) + " " + fileOrFiles;

   METALMOCKTHEN(_directoryIteratorMock->SetDirectoryIteratorMock.CalledOnceWith(p_args.targetFolderPath, p_args.recurse)).Then(

   METALMOCKTHEN(_directoryIteratorMock->SetFileAndFolderPathIgnoreSubstringsMock.CalledOnceWith(
      expectedFileAndFolderPathIgnoreSubstrings))).Then(

   METALMOCKTHEN(_directoryIteratorMock->GetNonEmptyNonIgnoredTextFilePathsMock.CalledOnce())).Then(

   METALMOCKTHEN(_memberFunctionAccumulator_ReplaceTextInTextFileMock->SumElementsWithFunctionMock.CalledOnceWith(
      nonEmptyNonIgnoredTextFilePathsInTargetDirectory,
      &_replaceTextInTextFilesSubProgram, &ReplaceTextInTextFilesSubProgram::ReplaceTextInTextFile))).Then(

   METALMOCKTHEN(p_pluralizerMock->PotentiallyPluralizeWordMock.CalledOnceWith(
      numberOfFilesThatWereOrWouldBeModified, "file", "files"))).Then(

   METALMOCKTHEN(p_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(
      expectedMessage)));

   IS_ZERO(exitCode);
}

TEST(ReplaceTextInTextFile_DryRunIsTrueOrFalse_ReplacedFileTextEqualsOriginalFileText_DoesNothing_Returns0)
{
   _call_PrintReadingFileMessageIfVerboseModeMock->CallConstMemberFunctionMock.Expect();

   const string textFileText = p_fileSystemMock->ReadTextMock.ReturnRandom();

   _textReplacerMock->ReplaceTextMock.Return(textFileText);

   const fs::path textFilePath = ZenUnit::Random<string>();
   //
   const size_t numberOfFilesThatWereOrWouldBeModified = _replaceTextInTextFilesSubProgram.ReplaceTextInTextFile(textFilePath);
   //
   METALMOCKTHEN(_call_PrintReadingFileMessageIfVerboseModeMock->CallConstMemberFunctionMock.CalledOnceWith(
      &_replaceTextInTextFilesSubProgram,
      &ReplaceTextInTextFilesSubProgram::PrintReadingFileMessageIfVerboseIsTrue, p_args.verbose, textFilePath)).Then(

   METALMOCKTHEN(p_fileSystemMock->ReadTextMock.CalledOnceWith(textFilePath))).Then(

   METALMOCKTHEN(_textReplacerMock->ReplaceTextMock.CalledOnceWith(
      textFileText,
      p_args.fromRegexPattern,
      p_args.toRegexPattern)));

   IS_ZERO(numberOfFilesThatWereOrWouldBeModified);
}

TEST(ReplaceTextInTextFile_DryRunIsTrue_ReplacedFileTextDiffersFromOriginalFileText_DoesNotModifyFile_PrintsWouldReplaceTextInFileMessage_Returns1)
{
   _call_PrintReadingFileMessageIfVerboseModeMock->CallConstMemberFunctionMock.Expect();

   const string textFileText = p_fileSystemMock->ReadTextMock.ReturnRandom();

   const string replacedTextFileText = ZenUnit::Random<string>() + ZenUnit::Random<string>();
   _textReplacerMock->ReplaceTextMock.Return(replacedTextFileText);

   p_consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   _replaceTextInTextFilesSubProgram.p_args.dryrun = true;
   const fs::path textFilePath = ZenUnit::Random<string>();
   //
   const size_t numberOfFilesThatWereOrWouldBeModified = _replaceTextInTextFilesSubProgram.ReplaceTextInTextFile(textFilePath);
   //
   const string expectedReplacedTextMessage = "DryRun: Would replace text in file " + textFilePath.string();;
   METALMOCKTHEN(_call_PrintReadingFileMessageIfVerboseModeMock->CallConstMemberFunctionMock.CalledOnceWith(&_replaceTextInTextFilesSubProgram, &ReplaceTextInTextFilesSubProgram::PrintReadingFileMessageIfVerboseIsTrue, p_args.verbose, textFilePath)).Then(
   METALMOCKTHEN(p_fileSystemMock->ReadTextMock.CalledOnceWith(textFilePath))).Then(
   METALMOCKTHEN(_textReplacerMock->ReplaceTextMock.CalledOnceWith(textFileText, p_args.fromRegexPattern, p_args.toRegexPattern))).Then(
   METALMOCKTHEN(p_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(expectedReplacedTextMessage)));
   ARE_EQUAL(1, numberOfFilesThatWereOrWouldBeModified);
}

TEST(ReplaceTextInTextFile_DryRunIsFalse_ReplacedFileTextDiffersFromOriginalFileText_WritesReplacedTextToFile_PrintsReplacedTextInFileMessage_Returns1)
{
   _call_PrintReadingFileMessageIfVerboseModeMock->CallConstMemberFunctionMock.Expect();

   const string textFileText = p_fileSystemMock->ReadTextMock.ReturnRandom();

   const string replacedTextFileText = ZenUnit::Random<string>() + ZenUnit::Random<string>();
   _textReplacerMock->ReplaceTextMock.Return(replacedTextFileText);

   p_fileSystemMock->CreateTextFileMock.Expect();

   p_consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   const fs::path textFilePath = ZenUnit::Random<string>();
   _replaceTextInTextFilesSubProgram.p_args.dryrun = false;
   //
   const size_t numberOfFilesThatWereOrWouldBeModified = _replaceTextInTextFilesSubProgram.ReplaceTextInTextFile(textFilePath);
   //
   const string expectedReplacedTextMessage = "Replaced text in file " + textFilePath.string();;
   METALMOCKTHEN(_call_PrintReadingFileMessageIfVerboseModeMock->CallConstMemberFunctionMock.CalledOnceWith(
      &_replaceTextInTextFilesSubProgram,
      &ReplaceTextInTextFilesSubProgram::PrintReadingFileMessageIfVerboseIsTrue, p_args.verbose, textFilePath)).Then(

   METALMOCKTHEN(p_fileSystemMock->ReadTextMock.CalledOnceWith(textFilePath))).Then(

   METALMOCKTHEN(_textReplacerMock->ReplaceTextMock.CalledOnceWith(
      textFileText, p_args.fromRegexPattern, p_args.toRegexPattern))).Then(

   METALMOCKTHEN(p_fileSystemMock->CreateTextFileMock.CalledOnceWith(
      textFilePath, replacedTextFileText))).Then(

   METALMOCKTHEN(p_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(
      expectedReplacedTextMessage)));

   ARE_EQUAL(1, numberOfFilesThatWereOrWouldBeModified);
}

TEST(PrintReadingFileMessageIfVerboseIsTrue_VerboseIsFalse_DoesNothing)
{
   const fs::path textFilePath = ZenUnit::Random<fs::path>();
   _replaceTextInTextFilesSubProgram.PrintReadingFileMessageIfVerboseIsTrue(false, textFilePath);
}

TEST(PrintReadingFileMessageIfVerboseIsTrue_VerboseIsTrue_PrintsReadingFileMessage)
{
   p_consoleMock->ProgramNameThreadIdWriteLineMock.Expect();
   const fs::path textFilePath = ZenUnit::Random<fs::path>();
   //
   _replaceTextInTextFilesSubProgram.PrintReadingFileMessageIfVerboseIsTrue(true, textFilePath);
   //
   const string expectedReadingFileMessage = "Verbose: Reading file " + textFilePath.string();
   METALMOCK(p_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(expectedReadingFileMessage));
}

RUN_TESTS(ReplaceTextInTextFilesSubProgramTests)
