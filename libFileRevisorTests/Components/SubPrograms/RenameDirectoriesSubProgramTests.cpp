#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/RenameDirectoriesSubProgram.h"
#include "libFileRevisorTests/Components/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/MetalMock/VoidTwoArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/Iteration/Counter/MetalMock/PredicateCounterMock.h"
#include "libFileRevisorTests/Components/Iteration/Transform/MetalMock/OneArgMemberFunctionTransformerMock.h"
#include "libFileRevisorTests/Components/Strings/MetalMock/PluralizerMock.h"
#include "libFileRevisorTests/Components/Strings/MetalMock/TextReplacerMock.h"

TESTS(RenameDirectoriesSubProgramTests)
FACTS(Run_CallsRenameDirectoryOnEachFolderPathInArgsDirPath_PrintsNumberOfDirectoriesRenamedOrWouldBeRenamed_Returns0)
AFACT(RenameDirectory_ReplacedDirectoryNameEqualsSourceDirectoryName_PrintsDidNotMatchDirectoryIfVerboseMode_ReturnsFalseRenameResult)
AFACT(RenameDirectory_ReplacedDirectoryNameDoesNotEqualSourceDirectoryName_DryRunIsTrue_PrintsWouldRenameDirectory_ReturnsTrueRenameResult)
AFACT(RenameDirectory_ReplacedDirectoryNameDoesNotEqualSourceDirectoryName_DryRunIsFalse_RenamesDirectory_PrintsRenamedDirectory_ReturnsTrueRenameResult)
AFACT(PrintDidNotMatchDirectoryMessageIfVerboseMode_VerboseIsFalse_DoesNothing)
AFACT(PrintDidNotMatchDirectoryMessageIfVerboseMode_VerboseIsTrue_PrintsDidNotMatchDirectoryMessage)
EVIDENCE

RenameDirectoriesSubProgram _renameDirectoriesSubProgram;
// Function Pointers
using _call_PrintDidNotMatchDirectoryMessageIfVerboseModeMockType =
   VoidTwoArgMemberFunctionCallerMock<RenameDirectoriesSubProgram, bool, const fs::path&>;
_call_PrintDidNotMatchDirectoryMessageIfVerboseModeMockType* _call_PrintDidNotMatchDirectoryMessageIfVerboseModeMock = nullptr;
// Function Callers
using OneArgMemberFunctionTransformerMockType =
   OneArgMemberFunctionTransformerMock<RenameDirectoriesSubProgram, fs::path, RenameResult>;
OneArgMemberFunctionTransformerMockType* _directoryPathsTransformer_RenameDirectoryMock = nullptr;
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
   // Function Pointers
   _renameDirectoriesSubProgram._call_PrintDidNotMatchDirectoryMessageIfVerboseMode.reset(_call_PrintDidNotMatchDirectoryMessageIfVerboseModeMock = new _call_PrintDidNotMatchDirectoryMessageIfVerboseModeMockType);
   // Function Callers
   _renameDirectoriesSubProgram._directoryPathsTransformer_RenameDirectory.reset(_directoryPathsTransformer_RenameDirectoryMock = new OneArgMemberFunctionTransformerMockType);
   // Base Class Constant Components
   _renameDirectoriesSubProgram.p_console.reset(p_consoleMock = new ConsoleMock);
   _renameDirectoriesSubProgram.p_fileSystem.reset(p_fileSystemMock = new FileSystemMock);
   _renameDirectoriesSubProgram.p_pluralizer.reset(p_pluralizerMock = new PluralizerMock);
   // Constant Components
   _renameDirectoriesSubProgram._predicateCounter.reset(_predicateCounterMock = new PredicateCounterMock<RenameResult>);
   _renameDirectoriesSubProgram._textReplacer.reset(_textReplacerMock = new TextReplacerMock);
   // Mutable Fields
   _renameDirectoriesSubProgram.p_args = p_args = ZenUnit::Random<FileRevisorArgs>();
}

TEST2X2(Run_CallsRenameDirectoryOnEachFolderPathInArgsDirPath_PrintsNumberOfDirectoriesRenamedOrWouldBeRenamed_Returns0,
   bool dryrun, string_view expectedNumberOfDirectoriesMessagePrefix,
   true, "Result: Would rename ",
   false, "Result: Renamed ")
{
   const vector<fs::path> directoryPathsInDirectory = p_fileSystemMock->GetFolderPathsInDirectoryMock.ReturnRandom();

   const vector<RenameResult> directoryRenameResults = _directoryPathsTransformer_RenameDirectoryMock->TransformMock.ReturnRandom();

   const size_t numberOfRenamedDirectories = _predicateCounterMock->CountWhereMock.ReturnRandom();

   const string directoryOrDirectories = p_pluralizerMock->PotentiallyPluralizeWordMock.ReturnRandom();

   p_consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   _renameDirectoriesSubProgram.p_args.dryrun = dryrun;
   //
   const int exitCode = _renameDirectoriesSubProgram.Run();
   //
   const string expectedNumberOfDirectoriesMessage = Utils::String::ConcatValues(
      expectedNumberOfDirectoriesMessagePrefix, numberOfRenamedDirectories, ' ', directoryOrDirectories);

   METALMOCKTHEN(p_fileSystemMock->GetFolderPathsInDirectoryMock.CalledOnceWith(
      p_args.targetFolderPath, p_args.recurse)).Then(

   METALMOCKTHEN(_directoryPathsTransformer_RenameDirectoryMock->TransformMock.CalledOnceWith(
      directoryPathsInDirectory, &_renameDirectoriesSubProgram, &RenameDirectoriesSubProgram::RenameDirectory))).Then(

   METALMOCKTHEN(_predicateCounterMock->CountWhereMock.CalledOnceWith(
      directoryRenameResults, RenameResult::DidRenameFileOrDirectoryFieldIsTrue))).Then(

   METALMOCKTHEN(p_pluralizerMock->PotentiallyPluralizeWordMock.CalledOnceWith(
      numberOfRenamedDirectories, "directory", "directories"))).Then(

   METALMOCKTHEN(p_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(
      expectedNumberOfDirectoriesMessage)));

   IS_ZERO(exitCode);
}

TEST(RenameDirectory_ReplacedDirectoryNameEqualsSourceDirectoryName_PrintsDidNotMatchDirectoryIfVerboseMode_ReturnsFalseRenameResult)
{
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   const string directoryName = directoryPath.filename().string();
   _textReplacerMock->ReplaceTextMock.Return(directoryName);

   _call_PrintDidNotMatchDirectoryMessageIfVerboseModeMock->CallConstMemberFunctionMock.Expect();
   //
   const RenameResult renameResult = _renameDirectoriesSubProgram.RenameDirectory(directoryPath);
   //
   METALMOCKTHEN(_textReplacerMock->ReplaceTextMock.CalledOnceWith(
      directoryName, p_args.fromRegexPattern, p_args.toRegexPattern)).Then(

   METALMOCKTHEN(_call_PrintDidNotMatchDirectoryMessageIfVerboseModeMock->CallConstMemberFunctionMock.CalledOnceWith(
      &_renameDirectoriesSubProgram, &RenameDirectoriesSubProgram::PrintDidNotMatchDirectoryMessageIfVerboseMode,
      p_args.verbose, directoryPath)));

   const RenameResult expectedRenameResult(false, directoryPath, directoryPath);
   ARE_EQUAL(expectedRenameResult, renameResult);
}

TEST(RenameDirectory_ReplacedDirectoryNameDoesNotEqualSourceDirectoryName_DryRunIsTrue_PrintsWouldRenameDirectory_ReturnsTrueRenameResult)
{
   const string regexReplacedDirectoryName = _textReplacerMock->ReplaceTextMock.ReturnRandom();

   p_consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   const fs::path directoryPath = ZenUnit::RandomNotEqualTo<string>(regexReplacedDirectoryName);
   _renameDirectoriesSubProgram.p_args.dryrun = true;
   //
   const RenameResult renameResult = _renameDirectoriesSubProgram.RenameDirectory(directoryPath);
   //
   const string originalDirectoryName = directoryPath.filename().string();
   const fs::path expectedRenamedFolderPath = directoryPath.parent_path() / regexReplacedDirectoryName;
   const string expectedFileRenamedMessage = "DryRun: Would rename directory " + directoryPath.string() + " to " + regexReplacedDirectoryName;

   METALMOCKTHEN(_textReplacerMock->ReplaceTextMock.CalledOnceWith(
      originalDirectoryName, p_args.fromRegexPattern, p_args.toRegexPattern)).Then(
   METALMOCKTHEN(p_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(expectedFileRenamedMessage)));

   const RenameResult expectedRenameResult(true, directoryPath, expectedRenamedFolderPath);
   ARE_EQUAL(expectedRenameResult, renameResult);
}

TEST(RenameDirectory_ReplacedDirectoryNameDoesNotEqualSourceDirectoryName_DryRunIsFalse_RenamesDirectory_PrintsRenamedDirectory_ReturnsTrueRenameResult)
{
   const string regexReplacedDirectoryName = _textReplacerMock->ReplaceTextMock.ReturnRandom();

   const fs::path renamedFolderPath = p_fileSystemMock->RenameDirectoryMock.ReturnRandom();

   p_consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   const fs::path directoryPath = ZenUnit::RandomNotEqualTo<string>(regexReplacedDirectoryName);
   _renameDirectoriesSubProgram.p_args.dryrun = false;
   //
   const RenameResult renameResult = _renameDirectoriesSubProgram.RenameDirectory(directoryPath);
   //
   const string originalDirectoryName = directoryPath.filename().string();
   const string expectedRenamedDirectoryMessage = "Renamed directory " + directoryPath.string() + " to " + regexReplacedDirectoryName;

   METALMOCKTHEN(_textReplacerMock->ReplaceTextMock.CalledOnceWith(
      originalDirectoryName, p_args.fromRegexPattern, p_args.toRegexPattern)).Then(

   METALMOCKTHEN(p_fileSystemMock->RenameDirectoryMock.CalledOnceWith(
      directoryPath, regexReplacedDirectoryName))).Then(

   METALMOCKTHEN(p_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(
      expectedRenamedDirectoryMessage)));

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
   p_consoleMock->ProgramNameThreadIdWriteLineMock.Expect();
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   //
   _renameDirectoriesSubProgram.PrintDidNotMatchDirectoryMessageIfVerboseMode(true, directoryPath);
   //
   const string expectedDidNotMatchDirectoryMessage = "Verbose: Did not match " + directoryPath.string();
   METALMOCK(p_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(expectedDidNotMatchDirectoryMessage));
}

RUN_TESTS(RenameDirectoriesSubProgramTests)
