#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/RenameDirectoriesSubProgram.h"
#include "libFileRevisorTests/Components/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/MetalMock/VoidTwoArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/Iteration/Counter/MetalMock/PredicateCounterMock.h"
#include "libFileRevisorTests/Components/Iteration/ForEach/MetalMock/OneExtraArgMemberForEacherMock.h"
#include "libFileRevisorTests/Components/Iteration/Transform/MetalMock/OneExtraArgMemberFunctionTransformerMock.h"
#include "libFileRevisorTests/Components/Strings/MetalMock/PluralizerMock.h"
#include "libFileRevisorTests/Components/Strings/MetalMock/RegexerMock.h"

TESTS(RenameDirectoriesSubProgramTests)
AFACT(DefaultConstructor_NewsComponents)
FACTS(Run_CallsRenameDirectoryOnEachDirectoryPathInArgsDirPath_PrintsNumberOfDirectoriesRenamedOrWouldBeRenamed_Returns0)
AFACT(RenameDirectory_RegexReplacedDirectoryNameEqualsSourceDirectoryName_PrintsDidNotMatchDirectoryIfVerboseMode_ReturnsFalseRenameResult)
AFACT(RenameDirectory_RegexReplacedDirectoryNameDoesNotEqualSourceDirectoryName_PreviewIsTrue_PrintsWouldRenameDirectory_ReturnsTrueRenameResult)
AFACT(RenameDirectory_RegexReplacedDirectoryNameDoesNotEqualSourceDirectoryName_PreviewIsFalse_RenamesDirectory_PrintsRenamedDirectory_ReturnsTrueRenameResult)
AFACT(PrintDidNotMatchDirectoryMessageIfVerboseMode_VerboseIsFalse_DoesNothing)
AFACT(PrintDidNotMatchDirectoryMessageIfVerboseMode_VerboseIsTrue_PrintsDidNotMatchDirectoryMessage)
EVIDENCE

RenameDirectoriesSubProgram _renameDirectoriesSubProgram;
// Function Pointers
const VoidTwoArgMemberFunctionCallerMock<RenameDirectoriesSubProgram, bool, const fs::path&>* _call_PrintDidNotMatchDirectoryMessageIfVerboseModeMock = nullptr;
// Function Callers
using OneExtraArgMemberFunctionTransformerMockType = OneExtraArgMemberFunctionTransformerMock<RenameDirectoriesSubProgram, fs::path, RenameResult, const FileRevisorArgs&>;
OneExtraArgMemberFunctionTransformerMockType* _directoryPathsTransformer_RenameDirectoryMock = nullptr;
// Constant Components
ConsoleMock* _protected_consoleMock = nullptr;
FileSystemMock* _protected_fileSystemMock = nullptr;
PluralizerMock* _protected_pluralizerMock = nullptr;
PredicateCounterMock<vector<RenameResult>, RenameResult>* _predicateCounterMock = nullptr;
RegexerMock* _regexerMock = nullptr;

STARTUP
{
   // Function Pointers
   _renameDirectoriesSubProgram._call_PrintDidNotMatchDirectoryMessageIfVerboseMode.reset(_call_PrintDidNotMatchDirectoryMessageIfVerboseModeMock = new VoidTwoArgMemberFunctionCallerMock<RenameDirectoriesSubProgram, bool, const fs::path&>);
   // Function Callers
   _renameDirectoriesSubProgram._directoryPathsTransformer_RenameDirectory.reset(_directoryPathsTransformer_RenameDirectoryMock = new OneExtraArgMemberFunctionTransformerMockType);
   // Constant Components
	_renameDirectoriesSubProgram._protected_console.reset(_protected_consoleMock = new ConsoleMock);
   _renameDirectoriesSubProgram._protected_fileSystem.reset(_protected_fileSystemMock = new FileSystemMock);
   _renameDirectoriesSubProgram._protected_pluralizer.reset(_protected_pluralizerMock = new PluralizerMock);
   _renameDirectoriesSubProgram._predicateCounter.reset(_predicateCounterMock = new PredicateCounterMock<vector<RenameResult>, RenameResult>);
   _renameDirectoriesSubProgram._regexer.reset(_regexerMock = new RegexerMock);
}

TEST(DefaultConstructor_NewsComponents)
{
   RenameDirectoriesSubProgram renameDirectoriesSubProgram;
   // Function Pointers
   DELETE_TO_ASSERT_NEWED(renameDirectoriesSubProgram._call_PrintDidNotMatchDirectoryMessageIfVerboseMode);
   // Function Callers
   DELETE_TO_ASSERT_NEWED(renameDirectoriesSubProgram._directoryPathsTransformer_RenameDirectory);
   // Constant Components
   DELETE_TO_ASSERT_NEWED(renameDirectoriesSubProgram._protected_console);
   DELETE_TO_ASSERT_NEWED(renameDirectoriesSubProgram._protected_fileSystem);
   DELETE_TO_ASSERT_NEWED(renameDirectoriesSubProgram._protected_pluralizer);
   DELETE_TO_ASSERT_NEWED(renameDirectoriesSubProgram._predicateCounter);
   DELETE_TO_ASSERT_NEWED(renameDirectoriesSubProgram._regexer);
}

TEST2X2(Run_CallsRenameDirectoryOnEachDirectoryPathInArgsDirPath_PrintsNumberOfDirectoriesRenamedOrWouldBeRenamed_Returns0,
   bool preview, string_view expectedNumberOfDirectoriesMessagePrefix,
   true, "[FileRevisor] Result: Would rename ",
   false, "[FileRevisor] Result: Renamed ")
{
   const vector<fs::path> directoryPathsInDirectory =
      _protected_fileSystemMock->GetDirectoryPathsInDirectoryMock.ReturnRandom();

   const vector<RenameResult> directoryRenameResults =
      _directoryPathsTransformer_RenameDirectoryMock->TransformMock.ReturnRandom();

   const size_t numberOfRenamedDirectories = _predicateCounterMock->CountWhereMock.ReturnRandom();

   const string directoryOrDirectories = _protected_pluralizerMock->PotentiallyPluralizeWordMock.ReturnRandom();

   _protected_consoleMock->WriteLineMock.Expect();

   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.preview = preview;
   //
   const int exitCode = _renameDirectoriesSubProgram.Run(args);
   //
   METALMOCK(_protected_fileSystemMock->GetDirectoryPathsInDirectoryMock.CalledOnceWith(args.targetDirectoryPath, args.recurse));
   METALMOCK(_directoryPathsTransformer_RenameDirectoryMock->TransformMock.CalledOnceWith(
      directoryPathsInDirectory, &_renameDirectoriesSubProgram, &RenameDirectoriesSubProgram::RenameDirectory, args));
   METALMOCK(_predicateCounterMock->CountWhereMock.CalledOnceWith(directoryRenameResults, RenameResult::DidRenameFileOrDirectoryFieldIsTrue));
   METALMOCK(_protected_pluralizerMock->PotentiallyPluralizeWordMock.CalledOnceWith(numberOfRenamedDirectories, "directory", "directories"));
   const string expectedNumberOfDirectoriesMessage = String::Concat(
      expectedNumberOfDirectoriesMessagePrefix, numberOfRenamedDirectories, ' ', directoryOrDirectories);
   METALMOCK(_protected_consoleMock->WriteLineMock.CalledOnceWith(expectedNumberOfDirectoriesMessage));
   IS_ZERO(exitCode);
}

TEST(RenameDirectory_RegexReplacedDirectoryNameEqualsSourceDirectoryName_PrintsDidNotMatchDirectoryIfVerboseMode_ReturnsFalseRenameResult)
{
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   const string directoryName = directoryPath.filename().string();
   _regexerMock->ReplaceMock.Return(directoryName);

   _call_PrintDidNotMatchDirectoryMessageIfVerboseModeMock->ConstCallMock.Expect();

   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   const RenameResult renameResult = _renameDirectoriesSubProgram.RenameDirectory(directoryPath, args);
   //
   METALMOCK(_regexerMock->ReplaceMock.CalledOnceWith(directoryName, args.fromRegexPattern, args.toRegexPattern));
   METALMOCK(_call_PrintDidNotMatchDirectoryMessageIfVerboseModeMock->ConstCallMock.CalledOnceWith(
      &_renameDirectoriesSubProgram,
      &RenameDirectoriesSubProgram::PrintDidNotMatchDirectoryMessageIfVerboseMode,
      args.verbose, directoryPath));
   const RenameResult expectedRenameResult(false, directoryPath, directoryPath);
   ARE_EQUAL(expectedRenameResult, renameResult);
}

TEST(RenameDirectory_RegexReplacedDirectoryNameDoesNotEqualSourceDirectoryName_PreviewIsTrue_PrintsWouldRenameDirectory_ReturnsTrueRenameResult)
{
   const string regexReplacedDirectoryName = _regexerMock->ReplaceMock.ReturnRandom();

   _protected_consoleMock->WriteLineMock.Expect();

   const fs::path directoryPath = ZenUnit::RandomNotEqualToValue<string>(regexReplacedDirectoryName);
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.preview = true;
   //
   const RenameResult renameResult = _renameDirectoriesSubProgram.RenameDirectory(directoryPath, args);
   //
   const string originalDirectoryName = directoryPath.filename().string();
   METALMOCK(_regexerMock->ReplaceMock.CalledOnceWith(originalDirectoryName, args.fromRegexPattern, args.toRegexPattern));
   const fs::path expectedRenamedDirectoryPath = directoryPath.parent_path() / regexReplacedDirectoryName;
   const string expectedFileRenamedMessage =
      "[FileRevisor] Preview: Would rename directory " + directoryPath.string() + " to " + regexReplacedDirectoryName;
   METALMOCK(_protected_consoleMock->WriteLineMock.CalledOnceWith(expectedFileRenamedMessage));
   const RenameResult expectedRenameResult(true, directoryPath, expectedRenamedDirectoryPath);
   ARE_EQUAL(expectedRenameResult, renameResult);
}

TEST(RenameDirectory_RegexReplacedDirectoryNameDoesNotEqualSourceDirectoryName_PreviewIsFalse_RenamesDirectory_PrintsRenamedDirectory_ReturnsTrueRenameResult)
{
   const string regexReplacedDirectoryName = _regexerMock->ReplaceMock.ReturnRandom();

   const fs::path renamedDirectoryPath = _protected_fileSystemMock->RenameDirectoryMock.ReturnRandom();

   _protected_consoleMock->WriteLineMock.Expect();

   const fs::path directoryPath = ZenUnit::RandomNotEqualToValue<string>(regexReplacedDirectoryName);
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.preview = false;
   //
   const RenameResult renameResult = _renameDirectoriesSubProgram.RenameDirectory(directoryPath, args);
   //
   const string originalDirectoryName = directoryPath.filename().string();
   METALMOCK(_regexerMock->ReplaceMock.CalledOnceWith(originalDirectoryName, args.fromRegexPattern, args.toRegexPattern));
   METALMOCK(_protected_fileSystemMock->RenameDirectoryMock.CalledOnceWith(directoryPath, regexReplacedDirectoryName));
   const string expectedRenamedDirectoryMessage =
      "[FileRevisor] Renamed directory " + directoryPath.string() + " to " + regexReplacedDirectoryName;
   METALMOCK(_protected_consoleMock->WriteLineMock.CalledOnceWith(expectedRenamedDirectoryMessage));
   const RenameResult expectedRenameResult(true, directoryPath, renamedDirectoryPath);
   ARE_EQUAL(expectedRenameResult, renameResult);
}

TEST(PrintDidNotMatchDirectoryMessageIfVerboseMode_VerboseIsFalse_DoesNothing)
{
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   _renameDirectoriesSubProgram.PrintDidNotMatchDirectoryMessageIfVerboseMode(false, directoryPath);
}

TEST(PrintDidNotMatchDirectoryMessageIfVerboseMode_VerboseIsTrue_PrintsDidNotMatchDirectoryMessage)
{
   _protected_consoleMock->WriteLineMock.Expect();
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   //
   _renameDirectoriesSubProgram.PrintDidNotMatchDirectoryMessageIfVerboseMode(true, directoryPath);
   //
   const string expectedDidNotMatchDirectoryMessage =
      "[FileRevisor] Verbose: Did not match " + directoryPath.string();
   METALMOCK(_protected_consoleMock->WriteLineMock.CalledOnceWith(expectedDidNotMatchDirectoryMessage));
}

RUN_TESTS(RenameDirectoriesSubProgramTests)
