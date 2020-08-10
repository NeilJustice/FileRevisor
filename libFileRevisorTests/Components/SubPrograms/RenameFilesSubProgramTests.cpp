#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/RenameFilesSubProgram.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/RenameResultZenUnitFunctions.h"

TESTS(RenameFilesSubProgramTests)
AFACT(DefaultConstructor_NewsComponents)
FACTS(Run_CallsRenameFileOnEachFilePathInArgsDirPath_PrintsNumberOfFilesThatWereRenamedOrWouldBeRenamedDependingOnPreviewTrueOrFalse_Returns0)
FACTS(DidRenameFileIsTrue_ReturnsTrueIfDidRenameFileIsTrue)
AFACT(RenameFileIfFileNameMatchesFromPattern_RegexReplacedFileNameEqualsSourceFileName_PrintsDidNotMatchFileMessageIfVerboseMode_ReturnsFalseRenameResult)
AFACT(RenameFileIfFileNameMatchesFromPattern_RegexReplacedFileNameDoesNotEqualSourceFileName_PreviewIsTrue_PrintsWouldRenameMessage_ReturnsTrueRenameResult)
AFACT(RenameFileIfFileNameMatchesFromPattern_RegexReplacedFileNameDoesNotEqualSourceFileName_PreviewIsFalse_RenamesTheFile_ReturnsTrueRenameResult)
AFACT(PrintDidNotMatchFileMessageIfVerboseMode_VerboseIsFalse_DoesNothing)
AFACT(PrintDidNotMatchFileMessageIfVerboseMode_VerboseIsTrue_WritesDidNotMatchFileMessage)
EVIDENCE

RenameFilesSubProgram _renameFilesSubProgram;
// Constant Components
ConsoleMock* _protected_consoleMock = nullptr;
FileSystemMock* _protected_fileSystemMock = nullptr;
PluralizerMock* _pluralizerMock = nullptr;
PredicateCounterMock<vector<RenameResult>, RenameResult>* _predicateCounterMock = nullptr;
RegexerMock* _regexerMock = nullptr;
// Function Callers
using OneExtraArgMemberFunctionTransformerMockType = OneExtraArgMemberFunctionTransformerMock<
   RenameFilesSubProgram, fs::path, RenameResult, const FileRevisorArgs&>;
OneExtraArgMemberFunctionTransformerMockType* _transformer_RenameFileIfFileNameMatchesFromPatternMock = nullptr;
const VoidTwoArgMemberFunctionCallerMock<RenameFilesSubProgram, bool, const fs::path&>*
   _caller_PrintDidNotMatchFileMessageIfVerboseModeMock = nullptr;

STARTUP
{
   // Constant Components
   _renameFilesSubProgram._protected_console.reset(_protected_consoleMock = new ConsoleMock);
   _renameFilesSubProgram._protected_fileSystem.reset(_protected_fileSystemMock = new FileSystemMock);
   _renameFilesSubProgram._protected_pluralizer.reset(_pluralizerMock = new PluralizerMock);
   _renameFilesSubProgram._predicateCounter.reset(_predicateCounterMock = new PredicateCounterMock<vector<RenameResult>, RenameResult>);
   _renameFilesSubProgram._regexer.reset(_regexerMock = new RegexerMock);
   // Function Callers
   _renameFilesSubProgram._caller_PrintDidNotMatchFileMessageIfVerboseMode.reset(_caller_PrintDidNotMatchFileMessageIfVerboseModeMock = new VoidTwoArgMemberFunctionCallerMock<RenameFilesSubProgram, bool, const fs::path&>);
   _renameFilesSubProgram._transformer_RenameFileIfFileNameMatchesFromPattern.reset(_transformer_RenameFileIfFileNameMatchesFromPatternMock = new OneExtraArgMemberFunctionTransformerMockType);
}

TEST(DefaultConstructor_NewsComponents)
{
   RenameFilesSubProgram renameFilesSubProgram;
   // Constant Components
   DELETE_TO_ASSERT_NEWED(renameFilesSubProgram._protected_console);
   DELETE_TO_ASSERT_NEWED(renameFilesSubProgram._protected_fileSystem);
   DELETE_TO_ASSERT_NEWED(renameFilesSubProgram._protected_pluralizer);
   DELETE_TO_ASSERT_NEWED(renameFilesSubProgram._predicateCounter);
   DELETE_TO_ASSERT_NEWED(renameFilesSubProgram._regexer);
   // Function Callers
   DELETE_TO_ASSERT_NEWED(renameFilesSubProgram._caller_PrintDidNotMatchFileMessageIfVerboseMode);
   DELETE_TO_ASSERT_NEWED(renameFilesSubProgram._transformer_RenameFileIfFileNameMatchesFromPattern);
}

TEST2X2(Run_CallsRenameFileOnEachFilePathInArgsDirPath_PrintsNumberOfFilesThatWereRenamedOrWouldBeRenamedDependingOnPreviewTrueOrFalse_Returns0,
   bool preview, string_view expectedRenamedFilesMessagePrefix,
   false, "[FileRevisor] Result: Renamed ",
   true, "[FileRevisor] Result: Would rename ")
{
   const vector<fs::path> filePathsInAndPossiblyBelowDirectory = ZenUnit::RandomVector<fs::path>();
   _protected_fileSystemMock->GetFilePathsInDirectoryMock.Return(filePathsInAndPossiblyBelowDirectory);

   const vector<RenameResult> fileRenameResults = ZenUnit::RandomVector<RenameResult>();
   _transformer_RenameFileIfFileNameMatchesFromPatternMock->TransformMock.Return(fileRenameResults);

   const size_t numberOfRenamedFiles = _predicateCounterMock->CountWhereMock.ReturnRandom();

   const string fileOrFiles = _pluralizerMock->PotentiallyPluralizeWordMock.ReturnRandom();

   _protected_consoleMock->WriteLineMock.Expect();

   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.preview = preview;
   //
   const int exitCode = _renameFilesSubProgram.Run(args);
   //
   METALMOCK(_protected_fileSystemMock->GetFilePathsInDirectoryMock.CalledOnceWith(args.targetDirectoryPath, args.recursive));
   METALMOCK(_transformer_RenameFileIfFileNameMatchesFromPatternMock->TransformMock.CalledOnceWith(
      filePathsInAndPossiblyBelowDirectory, &_renameFilesSubProgram, &RenameFilesSubProgram::RenameFileIfFileNameMatchesFromPattern, args));
   METALMOCK(_predicateCounterMock->CountWhereMock.CalledOnceWith(
      fileRenameResults, RenameFilesSubProgram::DidRenameFileIsTrue));
   METALMOCK(_pluralizerMock->PotentiallyPluralizeWordMock.CalledOnceWith(numberOfRenamedFiles, "file", "files"));
   const string expectedRenamedXNumberOfFilesMessage = String::Concat(
      expectedRenamedFilesMessagePrefix, numberOfRenamedFiles, ' ', fileOrFiles);
   METALMOCK(_protected_consoleMock->WriteLineMock.CalledOnceWith(expectedRenamedXNumberOfFilesMessage));
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
   const string regexReplacedFileName = fileName;
   _regexerMock->ReplaceMock.Return(regexReplacedFileName);

   _caller_PrintDidNotMatchFileMessageIfVerboseModeMock->ConstCallMock.Expect();

   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   const RenameResult fileRenameResult = _renameFilesSubProgram.RenameFileIfFileNameMatchesFromPattern(filePath, args);
   //
   METALMOCK(_regexerMock->ReplaceMock.CalledOnceWith(fileName, args.fromRegexPattern, args.toRegexPattern));
   METALMOCK(_caller_PrintDidNotMatchFileMessageIfVerboseModeMock->ConstCallMock.CalledOnceWith(
      &_renameFilesSubProgram, &RenameFilesSubProgram::PrintDidNotMatchFileMessageIfVerboseMode, args.verbose, filePath));
   const RenameResult expectedRenameResult(false, filePath, filePath);
   ARE_EQUAL(expectedRenameResult, fileRenameResult);
}

TEST(RenameFileIfFileNameMatchesFromPattern_RegexReplacedFileNameDoesNotEqualSourceFileName_PreviewIsTrue_PrintsWouldRenameMessage_ReturnsTrueRenameResult)
{
   const string regexReplacedFileName = _regexerMock->ReplaceMock.ReturnRandom();
   _protected_consoleMock->WriteLineMock.Expect();
   const fs::path filePath = ZenUnit::Random<fs::path>();
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.preview = true;
   //
   const RenameResult fileRenameResult = _renameFilesSubProgram.RenameFileIfFileNameMatchesFromPattern(filePath, args);
   //
   const string originalFileName = filePath.filename().string();
   METALMOCK(_regexerMock->ReplaceMock.CalledOnceWith(originalFileName, args.fromRegexPattern, args.toRegexPattern));
   const fs::path expectedRenamedFilePath = filePath.parent_path() / regexReplacedFileName;
   const string expectedFileRenamedMessage = String::Concat(
      "[FileRevisor]  Preview: Would rename file ", filePath.string(), " to ", regexReplacedFileName);
   METALMOCK(_protected_consoleMock->WriteLineMock.CalledOnceWith(expectedFileRenamedMessage));
   const RenameResult expectedRenameResult(true, filePath, expectedRenamedFilePath);
   ARE_EQUAL(expectedRenameResult, fileRenameResult);
}

TEST(RenameFileIfFileNameMatchesFromPattern_RegexReplacedFileNameDoesNotEqualSourceFileName_PreviewIsFalse_RenamesTheFile_ReturnsTrueRenameResult)
{
   const string regexReplacedFileName = _regexerMock->ReplaceMock.ReturnRandom();
   const fs::path renamedFilePath = _protected_fileSystemMock->RenameFileMock.ReturnRandom();
   _protected_consoleMock->WriteLineMock.Expect();
   const fs::path filePath = ZenUnit::Random<fs::path>();
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.preview = false;
   //
   const RenameResult fileRenameResult = _renameFilesSubProgram.RenameFileIfFileNameMatchesFromPattern(filePath, args);
   //
   const string originalFileName = filePath.filename().string();
   METALMOCK(_regexerMock->ReplaceMock.CalledOnceWith(originalFileName, args.fromRegexPattern, args.toRegexPattern));
   METALMOCK(_protected_fileSystemMock->RenameFileMock.CalledOnceWith(filePath, regexReplacedFileName));
   const string expectedRenamedFileMessage = String::Concat(
      "[FileRevisor] Renamed file ", filePath.string(), " to ", regexReplacedFileName);
   METALMOCK(_protected_consoleMock->WriteLineMock.CalledOnceWith(expectedRenamedFileMessage));
   const RenameResult expectedRenameResult(true, filePath, renamedFilePath);
   ARE_EQUAL(expectedRenameResult, fileRenameResult);
}

TEST(PrintDidNotMatchFileMessageIfVerboseMode_VerboseIsFalse_DoesNothing)
{
   const fs::path filePath = ZenUnit::Random<fs::path>();
   _renameFilesSubProgram.PrintDidNotMatchFileMessageIfVerboseMode(false, filePath);
}

TEST(PrintDidNotMatchFileMessageIfVerboseMode_VerboseIsTrue_WritesDidNotMatchFileMessage)
{
   _protected_consoleMock->WriteLineMock.Expect();
   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   _renameFilesSubProgram.PrintDidNotMatchFileMessageIfVerboseMode(true, filePath);
   //
   const string expectedDidNotMatchFileMessage = "[FileRevisor]  Verbose: Did not match " + filePath.string();
   METALMOCK(_protected_consoleMock->WriteLineMock.CalledOnceWith(expectedDidNotMatchFileMessage));
}

RUN_TESTS(RenameFilesSubProgramTests)
