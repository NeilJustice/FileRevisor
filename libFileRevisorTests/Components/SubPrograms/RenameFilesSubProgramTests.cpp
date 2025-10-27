#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/RenameFilesSubProgram.h"
#include "libFileRevisorTests/Components/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/MetalMock/VoidTwoArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/Iteration/Counter/MetalMock/PredicateCounterMock.h"
#include "libFileRevisorTests/Components/Iteration/Transform/MetalMock/OneArgMemberFunctionTransformerMock.h"
#include "libFileRevisorTests/Components/Strings/MetalMock/PluralizerMock.h"
#include "libFileRevisorTests/Components/Strings/MetalMock/TextReplacerMock.h"

TESTS(RenameFilesSubProgramTests)
FACTS(Run_CallsRenameFileOnEachFilePathInArgsDirPath_PrintsNumberOfFilesThatWereRenamedOrWouldBeRenamedDependingOnDryRunTrueOrFalse_Returns0)
FACTS(DidRenameFileIsTrue_ReturnsTrueIfDidRenameFileIsTrue)
AFACT(RenameFileIfFileNameMatchesFromPattern_ReplacedFileNameEqualsSourceFileName_PrintsDidNotMatchFileMessageIfVerboseMode_ReturnsFalseRenameResult)
AFACT(RenameFileIfFileNameMatchesFromPattern_ReplacedFileNameDoesNotEqualSourceFileName_DryRunIsTrue_PrintsWouldRenameMessage_ReturnsTrueRenameResult)
AFACT(RenameFileIfFileNameMatchesFromPattern_ReplacedFileNameDoesNotEqualSourceFileName_DryRunIsFalse_RenamesTheFile_ReturnsTrueRenameResult)
AFACT(PrintDidNotMatchFileMessageIfVerboseMode_VerboseIsFalse_DoesNothing)
AFACT(PrintDidNotMatchFileMessageIfVerboseMode_VerboseIsTrue_WritesDidNotMatchFileMessage)
EVIDENCE

RenameFilesSubProgram _renameFilesSubProgram;
// Function Callers
using _caller_PrintDidNotMatchFileMessageIfVerboseModeMockType =
   VoidTwoArgMemberFunctionCallerMock<RenameFilesSubProgram, bool, const fs::path&>;
_caller_PrintDidNotMatchFileMessageIfVerboseModeMockType* _caller_PrintDidNotMatchFileMessageIfVerboseModeMock = nullptr;

using _transformer_RenameFileIfFileNameMatchesFromPatternMockType =
   OneArgMemberFunctionTransformerMock<RenameFilesSubProgram, fs::path, RenameResult>;
_transformer_RenameFileIfFileNameMatchesFromPatternMockType* _transformer_RenameFileIfFileNameMatchesFromPatternMock = nullptr;
// Base Class Constant Components
ConsoleMock* p_consoleMock = nullptr;
FileSystemMock* p_fileSystemMock = nullptr;
PluralizerMock* p_pluralizerMock = nullptr;
// Constant Components
PredicateCounterMock<RenameResult>* _predicateCounterMock = nullptr;
TextReplacerMock* _textReplacerMock = nullptr;
// Mutable Fields
FileRevisorArgs p_args;

STARTUP
{
   // Function Callers
   _renameFilesSubProgram._caller_PrintDidNotMatchFileMessageIfVerboseMode.reset(_caller_PrintDidNotMatchFileMessageIfVerboseModeMock = new _caller_PrintDidNotMatchFileMessageIfVerboseModeMockType);
   _renameFilesSubProgram._transformer_RenameFileIfFileNameMatchesFromPattern.reset(_transformer_RenameFileIfFileNameMatchesFromPatternMock = new _transformer_RenameFileIfFileNameMatchesFromPatternMockType);
   // Base Class Constant Components
   _renameFilesSubProgram.p_console.reset(p_consoleMock = new ConsoleMock);
   _renameFilesSubProgram.p_fileSystem.reset(p_fileSystemMock = new FileSystemMock);
   _renameFilesSubProgram.p_pluralizer.reset(p_pluralizerMock = new PluralizerMock);
   // Constant Components
   _renameFilesSubProgram._predicateCounter.reset(_predicateCounterMock = new PredicateCounterMock<RenameResult>);
   _renameFilesSubProgram._textReplacer.reset(_textReplacerMock = new TextReplacerMock);
   // Mutable Fields
   _renameFilesSubProgram.p_args = p_args = ZenUnit::Random<FileRevisorArgs>();
}

TEST2X2(Run_CallsRenameFileOnEachFilePathInArgsDirPath_PrintsNumberOfFilesThatWereRenamedOrWouldBeRenamedDependingOnDryRunTrueOrFalse_Returns0,
   bool dryrun, string_view expectedRenamedFilesMessagePrefix,
   false, "Result: Renamed ",
   true, "Result: Would rename ")
{
   const vector<fs::path> filePathsInAndPossiblyBelowDirectory =
      p_fileSystemMock->GetFilePathsInDirectoryMock.ReturnRandom();

   const vector<RenameResult> fileRenameResults =
      _transformer_RenameFileIfFileNameMatchesFromPatternMock->TransformMock.ReturnRandom();

   const size_t numberOfRenamedFiles = _predicateCounterMock->CountWhereMock.ReturnRandom();

   const string fileOrFiles = p_pluralizerMock->PotentiallyPluralizeWordMock.ReturnRandom();

   p_consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   _renameFilesSubProgram.p_args.dryrun = dryrun;
   //
   const int exitCode = _renameFilesSubProgram.Run();
   //
   const string expectedRenamedXNumberOfFilesMessage = String::ConcatValues(expectedRenamedFilesMessagePrefix, numberOfRenamedFiles, ' ', fileOrFiles);

   METALMOCKTHEN(p_fileSystemMock->GetFilePathsInDirectoryMock.CalledOnceWith(
      p_args.targetFolderPath, p_args.recurse)).Then(

   METALMOCKTHEN(_transformer_RenameFileIfFileNameMatchesFromPatternMock->TransformMock.CalledOnceWith(
      filePathsInAndPossiblyBelowDirectory,
      &_renameFilesSubProgram, &RenameFilesSubProgram::RenameFileIfFileNameMatchesFromPattern))).Then(

   METALMOCKTHEN(_predicateCounterMock->CountWhereMock.CalledOnceWith(
      fileRenameResults, RenameFilesSubProgram::DidRenameFileIsTrue))).Then(

   METALMOCKTHEN(p_pluralizerMock->PotentiallyPluralizeWordMock.CalledOnceWith(
      numberOfRenamedFiles, "file", "files"))).Then(

   METALMOCKTHEN(p_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(
      expectedRenamedXNumberOfFilesMessage)));

   IS_ZERO(exitCode);
}

TEST1X1(DidRenameFileIsTrue_ReturnsTrueIfDidRenameFileIsTrue,
   bool didRenameFileValue,
   false,
   true)
{
   RenameResult fileRenameResult;
   fileRenameResult.didRenameFileOrDirectory = didRenameFileValue;
   //
   const bool didRenameFileIsTrue = RenameFilesSubProgram::DidRenameFileIsTrue(fileRenameResult);
   //
   ARE_EQUAL(didRenameFileValue, didRenameFileIsTrue);
}

TEST(RenameFileIfFileNameMatchesFromPattern_ReplacedFileNameEqualsSourceFileName_PrintsDidNotMatchFileMessageIfVerboseMode_ReturnsFalseRenameResult)
{
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const string fileName = filePath.filename().string();
   _textReplacerMock->ReplaceTextMock.Return(fileName);

   _caller_PrintDidNotMatchFileMessageIfVerboseModeMock->CallConstMemberFunctionMock.Expect();
   //
   const RenameResult fileRenameResult = _renameFilesSubProgram.RenameFileIfFileNameMatchesFromPattern(filePath);
   //
   METALMOCKTHEN(_textReplacerMock->ReplaceTextMock.CalledOnceWith(
      fileName, p_args.fromRegexPattern, p_args.toRegexPattern)).Then(

   METALMOCKTHEN(_caller_PrintDidNotMatchFileMessageIfVerboseModeMock->CallConstMemberFunctionMock.CalledOnceWith(
      &_renameFilesSubProgram, &RenameFilesSubProgram::PrintDidNotMatchFileMessageIfVerboseMode,
      p_args.verbose,
      filePath)));

   const RenameResult expectedRenameResult(false, filePath, filePath);
   ARE_EQUAL(expectedRenameResult, fileRenameResult);
}

TEST(RenameFileIfFileNameMatchesFromPattern_ReplacedFileNameDoesNotEqualSourceFileName_DryRunIsTrue_PrintsWouldRenameMessage_ReturnsTrueRenameResult)
{
   const string regexReplacedFileName = _textReplacerMock->ReplaceTextMock.ReturnRandom();

   p_consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   const fs::path filePath = ZenUnit::RandomNotEqualTo<string>(regexReplacedFileName);
   _renameFilesSubProgram.p_args.dryrun = true;
   //
   const RenameResult fileRenameResult = _renameFilesSubProgram.RenameFileIfFileNameMatchesFromPattern(filePath);
   //
   const string originalFileName = filePath.filename().string();
   const fs::path expectedRenamedFilePath = filePath.parent_path() / regexReplacedFileName;
   const string expectedFileRenamedMessage = String::ConcatStrings("DryRun: Would rename file ", filePath.string(), " to ", regexReplacedFileName);

   METALMOCKTHEN(_textReplacerMock->ReplaceTextMock.CalledOnceWith(
      originalFileName, p_args.fromRegexPattern, p_args.toRegexPattern)).Then(

   METALMOCKTHEN(p_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(
      expectedFileRenamedMessage)));

   const RenameResult expectedRenameResult(true, filePath, expectedRenamedFilePath);
   ARE_EQUAL(expectedRenameResult, fileRenameResult);
}

TEST(RenameFileIfFileNameMatchesFromPattern_ReplacedFileNameDoesNotEqualSourceFileName_DryRunIsFalse_RenamesTheFile_ReturnsTrueRenameResult)
{
   const string regexReplacedFileName = _textReplacerMock->ReplaceTextMock.ReturnRandom();

   const fs::path renamedFilePath = p_fileSystemMock->RenameFileMock.ReturnRandom();

   p_consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   const fs::path filePath = ZenUnit::RandomNotEqualTo<string>(regexReplacedFileName);
   _renameFilesSubProgram.p_args.dryrun = false;
   //
   const RenameResult fileRenameResult = _renameFilesSubProgram.RenameFileIfFileNameMatchesFromPattern(filePath);
   //
   const string originalFileName = filePath.filename().string();
   const string expectedRenamedFileMessage = String::ConcatStrings("Renamed file ", filePath.string(), " to ", regexReplacedFileName);
   const RenameResult expectedRenameResult(true, filePath, renamedFilePath);
   METALMOCKTHEN(_textReplacerMock->ReplaceTextMock.CalledOnceWith(
      originalFileName, p_args.fromRegexPattern, p_args.toRegexPattern)).Then(

   METALMOCKTHEN(p_fileSystemMock->RenameFileMock.CalledOnceWith(
      filePath, regexReplacedFileName))).Then(

   METALMOCKTHEN(p_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(
      expectedRenamedFileMessage)));

   ARE_EQUAL(expectedRenameResult, fileRenameResult);
}

TEST(PrintDidNotMatchFileMessageIfVerboseMode_VerboseIsFalse_DoesNothing)
{
   const fs::path filePath = ZenUnit::Random<fs::path>();
   _renameFilesSubProgram.PrintDidNotMatchFileMessageIfVerboseMode(false, filePath);
}

TEST(PrintDidNotMatchFileMessageIfVerboseMode_VerboseIsTrue_WritesDidNotMatchFileMessage)
{
   p_consoleMock->ProgramNameThreadIdWriteLineMock.Expect();
   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   _renameFilesSubProgram.PrintDidNotMatchFileMessageIfVerboseMode(true, filePath);
   //
   const string expectedDidNotMatchFileMessage = "Verbose: Did not match " + filePath.string();
   METALMOCK(p_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(expectedDidNotMatchFileMessage));
}

RUN_TESTS(RenameFilesSubProgramTests)
