#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/RenameDirectoriesSubProgram.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"
#include "libFileRevisorTests/Components/Utility/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/Components/Utility/FunctionCallers/Member/MetalMock/VoidTwoArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/Utility/Iteration/Counter/MetalMock/PredicateCounterMock.h"
#include "libFileRevisorTests/Components/Utility/Iteration/Transform/MetalMock/OneExtraArgMemberFunctionTransformerMock.h"
#include "libFileRevisorTests/Components/Utility/Strings/MetalMock/PluralizerMock.h"
#include "libFileRevisorTests/Components/Utility/Strings/MetalMock/TextReplacerMock.h"

TESTS(RenameDirectoriesSubProgramTests)
AFACT(DefaultConstructor_NewsComponents)
FACTS(Run_CallsRenameDirectoryOnEachFolderPathInArgsDirPath_PrintsNumberOfDirectoriesRenamedOrWouldBeRenamed_Returns0)
AFACT(RenameDirectory_ReplacedDirectoryNameEqualsSourceDirectoryName_PrintsDidNotMatchDirectoryIfVerboseMode_ReturnsFalseRenameResult)
AFACT(RenameDirectory_ReplacedDirectoryNameDoesNotEqualSourceDirectoryName_DryRunIsTrue_PrintsWouldRenameDirectory_ReturnsTrueRenameResult)
AFACT(RenameDirectory_ReplacedDirectoryNameDoesNotEqualSourceDirectoryName_DryRunIsFalse_RenamesDirectory_PrintsRenamedDirectory_ReturnsTrueRenameResult)
AFACT(PrintDidNotMatchDirectoryMessageIfVerboseMode_VerboseIsFalse_DoesNothing)
AFACT(PrintDidNotMatchDirectoryMessageIfVerboseMode_VerboseIsTrue_PrintsDidNotMatchDirectoryMessage)
EVIDENCE

RenameDirectoriesSubProgram _renameDirectoriesSubProgram;
// Function Pointers
const VoidTwoArgMemberFunctionCallerMock<RenameDirectoriesSubProgram, bool, const fs::path&>* _call_PrintDidNotMatchDirectoryMessageIfVerboseModeMock = nullptr;
// Function Callers
using OneExtraArgMemberFunctionTransformerMockType = OneExtraArgMemberFunctionTransformerMock<RenameDirectoriesSubProgram, fs::path, RenameResult, const FileRevisorArgs&>;
OneExtraArgMemberFunctionTransformerMockType* _directoryPathsTransformer_RenameDirectoryMock = nullptr;
// Base Class Constant Components
ConsoleMock* _consoleMock = nullptr;
FileSystemMock* _fileSystemMock = nullptr;
PluralizerMock* _pluralizerMock = nullptr;
// Constant Components
PredicateCounterMock<RenameResult>* _predicateCounterMock = nullptr;
TextReplacerMock* _textReplacerMock = nullptr;

STARTUP
{
   // Function Pointers
   _renameDirectoriesSubProgram._call_PrintDidNotMatchDirectoryMessageIfVerboseMode.reset(_call_PrintDidNotMatchDirectoryMessageIfVerboseModeMock = new VoidTwoArgMemberFunctionCallerMock<RenameDirectoriesSubProgram, bool, const fs::path&>);
   // Function Callers
   _renameDirectoriesSubProgram._directoryPathsTransformer_RenameDirectory.reset(_directoryPathsTransformer_RenameDirectoryMock = new OneExtraArgMemberFunctionTransformerMockType);
   // Base Class Constant Components
   _renameDirectoriesSubProgram._console.reset(_consoleMock = new ConsoleMock);
   _renameDirectoriesSubProgram._fileSystem.reset(_fileSystemMock = new FileSystemMock);
   _renameDirectoriesSubProgram._pluralizer.reset(_pluralizerMock = new PluralizerMock);
   // Constant Components
   _renameDirectoriesSubProgram._predicateCounter.reset(_predicateCounterMock = new PredicateCounterMock<RenameResult>);
   _renameDirectoriesSubProgram._textReplacer.reset(_textReplacerMock = new TextReplacerMock);
}

TEST(DefaultConstructor_NewsComponents)
{
   RenameDirectoriesSubProgram renameDirectoriesSubProgram;
   // Function Pointers
   DELETE_TO_ASSERT_NEWED(renameDirectoriesSubProgram._call_PrintDidNotMatchDirectoryMessageIfVerboseMode);
   // Function Callers
   DELETE_TO_ASSERT_NEWED(renameDirectoriesSubProgram._directoryPathsTransformer_RenameDirectory);
   // Base Class Constant Components
   DELETE_TO_ASSERT_NEWED(renameDirectoriesSubProgram._console);
   DELETE_TO_ASSERT_NEWED(renameDirectoriesSubProgram._fileSystem);
   DELETE_TO_ASSERT_NEWED(renameDirectoriesSubProgram._pluralizer);
   // Constant Components
   DELETE_TO_ASSERT_NEWED(renameDirectoriesSubProgram._predicateCounter);
   DELETE_TO_ASSERT_NEWED(renameDirectoriesSubProgram._textReplacer);
}

TEST2X2(Run_CallsRenameDirectoryOnEachFolderPathInArgsDirPath_PrintsNumberOfDirectoriesRenamedOrWouldBeRenamed_Returns0,
   bool dryrun, string_view expectedNumberOfDirectoriesMessagePrefix,
   true, "Result: Would rename ",
   false, "Result: Renamed ")
{
   const vector<fs::path> directoryPathsInDirectory = _fileSystemMock->GetFolderPathsInDirectoryMock.ReturnRandom();

   const vector<RenameResult> directoryRenameResults = _directoryPathsTransformer_RenameDirectoryMock->TransformMock.ReturnRandom();

   const size_t numberOfRenamedDirectories = _predicateCounterMock->CountWhereMock.ReturnRandom();

   const string directoryOrDirectories = _pluralizerMock->PotentiallyPluralizeWordMock.ReturnRandom();

   _consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.dryrun = dryrun;
   //
   const int exitCode = _renameDirectoriesSubProgram.Run(args);
   //
   const string expectedNumberOfDirectoriesMessage = String::ConcatValues(
      expectedNumberOfDirectoriesMessagePrefix, numberOfRenamedDirectories, ' ', directoryOrDirectories);
   METALMOCKTHEN(_fileSystemMock->GetFolderPathsInDirectoryMock.CalledOnceWith(args.targetFolderPath, args.recurse)).Then(
   METALMOCKTHEN(_directoryPathsTransformer_RenameDirectoryMock->TransformMock.CalledOnceWith(
      directoryPathsInDirectory, &_renameDirectoriesSubProgram, &RenameDirectoriesSubProgram::RenameDirectory, args))).Then(
   METALMOCKTHEN(_predicateCounterMock->CountWhereMock.CalledOnceWith(directoryRenameResults, RenameResult::DidRenameFileOrDirectoryFieldIsTrue))).Then(
   METALMOCKTHEN(_pluralizerMock->PotentiallyPluralizeWordMock.CalledOnceWith(numberOfRenamedDirectories, "directory", "directories"))).Then(
   METALMOCKTHEN(_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(expectedNumberOfDirectoriesMessage)));
   IS_ZERO(exitCode);
}

TEST(RenameDirectory_ReplacedDirectoryNameEqualsSourceDirectoryName_PrintsDidNotMatchDirectoryIfVerboseMode_ReturnsFalseRenameResult)
{
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   const string directoryName = directoryPath.filename().string();
   _textReplacerMock->ReplaceTextMock.Return(directoryName);

   _call_PrintDidNotMatchDirectoryMessageIfVerboseModeMock->CallConstMemberFunctionMock.Expect();

   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   const RenameResult renameResult = _renameDirectoriesSubProgram.RenameDirectory(directoryPath, args);
   //
   METALMOCK(_textReplacerMock->ReplaceTextMock.CalledOnceWith(directoryName, args.fromRegexPattern, args.toRegexPattern));
   METALMOCK(_call_PrintDidNotMatchDirectoryMessageIfVerboseModeMock->CallConstMemberFunctionMock.CalledOnceWith(
      &_renameDirectoriesSubProgram, &RenameDirectoriesSubProgram::PrintDidNotMatchDirectoryMessageIfVerboseMode, args.verbose, directoryPath));
   const RenameResult expectedRenameResult(false, directoryPath, directoryPath);
   ARE_EQUAL(expectedRenameResult, renameResult);
}

TEST(RenameDirectory_ReplacedDirectoryNameDoesNotEqualSourceDirectoryName_DryRunIsTrue_PrintsWouldRenameDirectory_ReturnsTrueRenameResult)
{
   const string regexReplacedDirectoryName = _textReplacerMock->ReplaceTextMock.ReturnRandom();

   _consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   const fs::path directoryPath = ZenUnit::RandomNotEqualTo<string>(regexReplacedDirectoryName);
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.dryrun = true;
   //
   const RenameResult renameResult = _renameDirectoriesSubProgram.RenameDirectory(directoryPath, args);
   //
   const string originalDirectoryName = directoryPath.filename().string();
   METALMOCK(_textReplacerMock->ReplaceTextMock.CalledOnceWith(originalDirectoryName, args.fromRegexPattern, args.toRegexPattern));
   const fs::path expectedRenamedFolderPath = directoryPath.parent_path() / regexReplacedDirectoryName;
   const string expectedFileRenamedMessage = "DryRun: Would rename directory " + directoryPath.string() + " to " + regexReplacedDirectoryName;
   METALMOCK(_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(expectedFileRenamedMessage));
   const RenameResult expectedRenameResult(true, directoryPath, expectedRenamedFolderPath);
   ARE_EQUAL(expectedRenameResult, renameResult);
}

TEST(RenameDirectory_ReplacedDirectoryNameDoesNotEqualSourceDirectoryName_DryRunIsFalse_RenamesDirectory_PrintsRenamedDirectory_ReturnsTrueRenameResult)
{
   const string regexReplacedDirectoryName = _textReplacerMock->ReplaceTextMock.ReturnRandom();

   const fs::path renamedFolderPath = _fileSystemMock->RenameDirectoryMock.ReturnRandom();

   _consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   const fs::path directoryPath = ZenUnit::RandomNotEqualTo<string>(regexReplacedDirectoryName);
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.dryrun = false;
   //
   const RenameResult renameResult = _renameDirectoriesSubProgram.RenameDirectory(directoryPath, args);
   //
   const string originalDirectoryName = directoryPath.filename().string();
   METALMOCK(_textReplacerMock->ReplaceTextMock.CalledOnceWith(originalDirectoryName, args.fromRegexPattern, args.toRegexPattern));
   METALMOCK(_fileSystemMock->RenameDirectoryMock.CalledOnceWith(directoryPath, regexReplacedDirectoryName));
   const string expectedRenamedDirectoryMessage = "Renamed directory " + directoryPath.string() + " to " + regexReplacedDirectoryName;
   METALMOCK(_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(expectedRenamedDirectoryMessage));
   const RenameResult expectedRenameResult(true, directoryPath, renamedFolderPath);
   ARE_EQUAL(expectedRenameResult, renameResult);
}

TEST(PrintDidNotMatchDirectoryMessageIfVerboseMode_VerboseIsFalse_DoesNothing)
{
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   _renameDirectoriesSubProgram.PrintDidNotMatchDirectoryMessageIfVerboseMode(false, directoryPath);
}

TEST(PrintDidNotMatchDirectoryMessageIfVerboseMode_VerboseIsTrue_PrintsDidNotMatchDirectoryMessage)
{
   _consoleMock->ProgramNameThreadIdWriteLineMock.Expect();
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   //
   _renameDirectoriesSubProgram.PrintDidNotMatchDirectoryMessageIfVerboseMode(true, directoryPath);
   //
   const string expectedDidNotMatchDirectoryMessage = "Verbose: Did not match " + directoryPath.string();
   METALMOCK(_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(expectedDidNotMatchDirectoryMessage));
}

RUN_TESTS(RenameDirectoriesSubProgramTests)
