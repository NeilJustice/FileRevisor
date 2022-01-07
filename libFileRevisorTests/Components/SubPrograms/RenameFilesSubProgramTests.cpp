#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/RenameFilesSubProgram.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"
#include "libFileRevisorTests/UtilityComponents/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/UtilityComponents/FunctionCallers/Member/MetalMock/VoidTwoArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/UtilityComponents/Iteration/Counter/MetalMock/PredicateCounterMock.h"
#include "libFileRevisorTests/UtilityComponents/Iteration/Transform/MetalMock/OneExtraArgMemberFunctionTransformerMock.h"
#include "libFileRevisorTests/UtilityComponents/Strings/MetalMock/PluralizerMock.h"
#include "libFileRevisorTests/UtilityComponents/Strings/MetalMock/RegexerMock.h"

TESTS(RenameFilesSubProgramTests)
AFACT(DefaultConstructor_NewsComponents)
FACTS(Run_CallsRenameFileOnEachFilePathInArgsDirPath_PrintsNumberOfFilesThatWereRenamedOrWouldBeRenamedDependingOnDryRunTrueOrFalse_Returns0)
FACTS(DidRenameFileIsTrue_ReturnsTrueIfDidRenameFileIsTrue)
AFACT(RenameFileIfFileNameMatchesFromPattern_RegexReplacedFileNameEqualsSourceFileName_PrintsDidNotMatchFileMessageIfVerboseMode_ReturnsFalseRenameResult)
AFACT(RenameFileIfFileNameMatchesFromPattern_RegexReplacedFileNameDoesNotEqualSourceFileName_DryRunIsTrue_PrintsWouldRenameMessage_ReturnsTrueRenameResult)
AFACT(RenameFileIfFileNameMatchesFromPattern_RegexReplacedFileNameDoesNotEqualSourceFileName_DryRunIsFalse_RenamesTheFile_ReturnsTrueRenameResult)
AFACT(PrintDidNotMatchFileMessageIfVerboseMode_VerboseIsFalse_DoesNothing)
AFACT(PrintDidNotMatchFileMessageIfVerboseMode_VerboseIsTrue_WritesDidNotMatchFileMessage)
EVIDENCE

RenameFilesSubProgram _renameFilesSubProgram;
// Function Callers
using OneExtraArgMemberFunctionTransformerMockType = OneExtraArgMemberFunctionTransformerMock<RenameFilesSubProgram, fs::path, RenameResult, const FileRevisorArgs&>;
OneExtraArgMemberFunctionTransformerMockType* _transformer_RenameFileIfFileNameMatchesFromPatternMock = nullptr;
const VoidTwoArgMemberFunctionCallerMock<RenameFilesSubProgram, bool, const fs::path&>* _caller_PrintDidNotMatchFileMessageIfVerboseModeMock = nullptr;
// Base Class Constant Components
ConsoleMock* _consoleMock = nullptr;
FileSystemMock* _fileSystemMock = nullptr;
// Constant Components
PluralizerMock* _pluralizerMock = nullptr;
PredicateCounterMock<vector<RenameResult>, RenameResult>* _predicateCounterMock = nullptr;
RegexerMock* _regexerMock = nullptr;

STARTUP
{
   // Function Callers
   _renameFilesSubProgram._caller_PrintDidNotMatchFileMessageIfVerboseMode.reset(_caller_PrintDidNotMatchFileMessageIfVerboseModeMock = new VoidTwoArgMemberFunctionCallerMock<RenameFilesSubProgram, bool, const fs::path&>);
   _renameFilesSubProgram._transformer_RenameFileIfFileNameMatchesFromPattern.reset(_transformer_RenameFileIfFileNameMatchesFromPatternMock = new OneExtraArgMemberFunctionTransformerMockType);
   // Base Class Constant Components
   _renameFilesSubProgram._console.reset(_consoleMock = new ConsoleMock);
   _renameFilesSubProgram._fileSystem.reset(_fileSystemMock = new FileSystemMock);
   // Constant Components
   _renameFilesSubProgram._pluralizer.reset(_pluralizerMock = new PluralizerMock);
   _renameFilesSubProgram._predicateCounter.reset(_predicateCounterMock = new PredicateCounterMock<vector<RenameResult>, RenameResult>);
   _renameFilesSubProgram._regexer.reset(_regexerMock = new RegexerMock);
}

TEST(DefaultConstructor_NewsComponents)
{
   RenameFilesSubProgram renameFilesSubProgram;
   // Function Callers
   DELETE_TO_ASSERT_NEWED(renameFilesSubProgram._caller_PrintDidNotMatchFileMessageIfVerboseMode);
   DELETE_TO_ASSERT_NEWED(renameFilesSubProgram._transformer_RenameFileIfFileNameMatchesFromPattern);
   // Base Class Constant Components
   DELETE_TO_ASSERT_NEWED(renameFilesSubProgram._console);
   DELETE_TO_ASSERT_NEWED(renameFilesSubProgram._fileSystem);
   DELETE_TO_ASSERT_NEWED(renameFilesSubProgram._pluralizer);
   // Constant Components
   DELETE_TO_ASSERT_NEWED(renameFilesSubProgram._predicateCounter);
   DELETE_TO_ASSERT_NEWED(renameFilesSubProgram._regexer);
}

TEST2X2(Run_CallsRenameFileOnEachFilePathInArgsDirPath_PrintsNumberOfFilesThatWereRenamedOrWouldBeRenamedDependingOnDryRunTrueOrFalse_Returns0,
   bool dryrun, string_view expectedRenamedFilesMessagePrefix,
   false, "Result: Renamed ",
   true, "Result: Would rename ")
{
   const vector<fs::path> filePathsInAndPossiblyBelowDirectory =
      _fileSystemMock->GetFilePathsInDirectoryMock.ReturnRandom();

   const vector<RenameResult> fileRenameResults =
      _transformer_RenameFileIfFileNameMatchesFromPatternMock->TransformMock.ReturnRandom();

   const size_t numberOfRenamedFiles = _predicateCounterMock->CountWhereMock.ReturnRandom();

   const string fileOrFiles = _pluralizerMock->PotentiallyPluralizeWordMock.ReturnRandom();

   _consoleMock->ThreadIdWriteLineMock.Expect();

   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.dryrun = dryrun;
   //
   const int exitCode = _renameFilesSubProgram.Run(args);
   //
   METALMOCK(_fileSystemMock->GetFilePathsInDirectoryMock.CalledOnceWith(args.targetDirectoryPath, args.recurse));
   METALMOCK(_transformer_RenameFileIfFileNameMatchesFromPatternMock->TransformMock.CalledOnceWith(
      filePathsInAndPossiblyBelowDirectory, &_renameFilesSubProgram, &RenameFilesSubProgram::RenameFileIfFileNameMatchesFromPattern, args));
   METALMOCK(_predicateCounterMock->CountWhereMock.CalledOnceWith(
      fileRenameResults, RenameFilesSubProgram::DidRenameFileIsTrue));
   METALMOCK(_pluralizerMock->PotentiallyPluralizeWordMock.CalledOnceWith(numberOfRenamedFiles, "file", "files"));
   const string expectedRenamedXNumberOfFilesMessage = String::ConcatValues(expectedRenamedFilesMessagePrefix, numberOfRenamedFiles, ' ', fileOrFiles);
   METALMOCK(_consoleMock->ThreadIdWriteLineMock.CalledOnceWith(expectedRenamedXNumberOfFilesMessage));
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

TEST(RenameFileIfFileNameMatchesFromPattern_RegexReplacedFileNameEqualsSourceFileName_PrintsDidNotMatchFileMessageIfVerboseMode_ReturnsFalseRenameResult)
{
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const string fileName = filePath.filename().string();
   _regexerMock->ReplaceMock.Return(fileName);

   _caller_PrintDidNotMatchFileMessageIfVerboseModeMock->CallConstMemberFunctionMock.Expect();

   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   const RenameResult fileRenameResult = _renameFilesSubProgram.RenameFileIfFileNameMatchesFromPattern(filePath, args);
   //
   const RenameResult expectedRenameResult(false, filePath, filePath);
   METALMOCKTHEN(_regexerMock->ReplaceMock.CalledOnceWith(fileName, args.fromRegexPattern, args.toRegexPattern)).Then(
   METALMOCKTHEN(_caller_PrintDidNotMatchFileMessageIfVerboseModeMock->CallConstMemberFunctionMock.CalledOnceWith(
      &_renameFilesSubProgram, &RenameFilesSubProgram::PrintDidNotMatchFileMessageIfVerboseMode, args.verbose, filePath)));
   ARE_EQUAL(expectedRenameResult, fileRenameResult);
}

TEST(RenameFileIfFileNameMatchesFromPattern_RegexReplacedFileNameDoesNotEqualSourceFileName_DryRunIsTrue_PrintsWouldRenameMessage_ReturnsTrueRenameResult)
{
   const string regexReplacedFileName = _regexerMock->ReplaceMock.ReturnRandom();

   _consoleMock->ThreadIdWriteLineMock.Expect();

   const fs::path filePath = ZenUnit::RandomNotEqualTo<string>(regexReplacedFileName);
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.dryrun = true;
   //
   const RenameResult fileRenameResult = _renameFilesSubProgram.RenameFileIfFileNameMatchesFromPattern(filePath, args);
   //
   const string originalFileName = filePath.filename().string();
   const fs::path expectedRenamedFilePath = filePath.parent_path() / regexReplacedFileName;
   const string expectedFileRenamedMessage = String::ConcatStrings("DryRun: Would rename file ", filePath.string(), " to ", regexReplacedFileName);
   METALMOCKTHEN(_regexerMock->ReplaceMock.CalledOnceWith(originalFileName, args.fromRegexPattern, args.toRegexPattern)).Then(
   METALMOCKTHEN(_consoleMock->ThreadIdWriteLineMock.CalledOnceWith(expectedFileRenamedMessage)));
   const RenameResult expectedRenameResult(true, filePath, expectedRenamedFilePath);
   ARE_EQUAL(expectedRenameResult, fileRenameResult);
}

TEST(RenameFileIfFileNameMatchesFromPattern_RegexReplacedFileNameDoesNotEqualSourceFileName_DryRunIsFalse_RenamesTheFile_ReturnsTrueRenameResult)
{
   const string regexReplacedFileName = _regexerMock->ReplaceMock.ReturnRandom();

   const fs::path renamedFilePath = _fileSystemMock->RenameFileMock.ReturnRandom();

   _consoleMock->ThreadIdWriteLineMock.Expect();

   const fs::path filePath = ZenUnit::RandomNotEqualTo<string>(regexReplacedFileName);
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.dryrun = false;
   //
   const RenameResult fileRenameResult = _renameFilesSubProgram.RenameFileIfFileNameMatchesFromPattern(filePath, args);
   //
   const string originalFileName = filePath.filename().string();
   const string expectedRenamedFileMessage = String::ConcatStrings("Renamed file ", filePath.string(), " to ", regexReplacedFileName);
   const RenameResult expectedRenameResult(true, filePath, renamedFilePath);
   METALMOCKTHEN(_regexerMock->ReplaceMock.CalledOnceWith(originalFileName, args.fromRegexPattern, args.toRegexPattern)).Then(
   METALMOCKTHEN(_fileSystemMock->RenameFileMock.CalledOnceWith(filePath, regexReplacedFileName))).Then(
   METALMOCKTHEN(_consoleMock->ThreadIdWriteLineMock.CalledOnceWith(expectedRenamedFileMessage)));
   ARE_EQUAL(expectedRenameResult, fileRenameResult);
}

TEST(PrintDidNotMatchFileMessageIfVerboseMode_VerboseIsFalse_DoesNothing)
{
   const fs::path filePath = ZenUnit::Random<fs::path>();
   _renameFilesSubProgram.PrintDidNotMatchFileMessageIfVerboseMode(false, filePath);
}

TEST(PrintDidNotMatchFileMessageIfVerboseMode_VerboseIsTrue_WritesDidNotMatchFileMessage)
{
   _consoleMock->ThreadIdWriteLineMock.Expect();
   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   _renameFilesSubProgram.PrintDidNotMatchFileMessageIfVerboseMode(true, filePath);
   //
   const string expectedDidNotMatchFileMessage = "Verbose: Did not match " + filePath.string();
   METALMOCK(_consoleMock->ThreadIdWriteLineMock.CalledOnceWith(expectedDidNotMatchFileMessage));
}

RUN_TESTS(RenameFilesSubProgramTests)
