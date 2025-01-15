#include "pch.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorArgsParser.h"
#include "libFileRevisor/Components/Utility/Docopt/docopt.h"
#include "libFileRevisorTests/Components/FileRevisor/MetalMock/FileRevisorPreambleMakerMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"
#include "libFileRevisorTests/Components/Utility/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/Components/Utility/Docopt/MetalMock/DocoptParserMock.h"
#include "libFileRevisorTests/Components/Utility/FunctionCallers/Member/MetalMock/NonVoidTwoArgMemberFunctionCallerMock.h"

TESTS(FileRevisorArgsParserTests)
AFACT(ParseArgs_ParsesEachArgument_ReturnsFileRevisorArgs)
AFACT(PrintPreambleLines_WritesPreambleLinesToConsole)
AFACT(ParseTargetAndFromAndToArguments_IsDeleteDirectoryModeIsTrue_ParsesDirArgumentAsRequired_ReturnsDirAndFromAndToArgumentValues)
AFACT(ParseTargetAndFromAndToArguments_IsDeleteDirectoryModeIsFalse_FromArgumentIsEmpty_ThrowsInvalidArgumentException)
AFACT(ParseTargetAndFromAndToArguments_IsDeleteDirectoryModeIsFalse_FromArgumentIsNotEmpty_ParsesDirArgumentAsOptional_ReturnsDirAndFromAndToArgumentValues)
FACTS(DetermineProgramMode_OneOfTheFourProgramModeBoolsTrue_ReturnsExpectedProgramMode)
AFACT(DetermineProgramMode_AllFourProgramModeBoolsAreFalse_ThrowsInvalidArgument)
EVIDENCE

FileRevisorArgsParser _fileRevisorArgsParser;
// Function Pointers
METALMOCK_NONVOID4_STATIC_OR_FREE(ProgramMode, _call_FileRevisorArgsParser_DetermineProgramMode, bool, bool, bool, bool)
using NonVoidTwoArgMemberFunctionCallerMockType = NonVoidTwoArgMemberFunctionCallerMock<
   tuple<fs::path, string, string>, FileRevisorArgsParser, const map<string, docopt::Value>&, bool>;
// Function Callers
NonVoidTwoArgMemberFunctionCallerMockType* _caller_ParseDirAndFromAndToArgumentsMock = nullptr;
// Constant Components
ConsoleMock* _consoleMock = nullptr;
FileSystemMock* _fileSystemMock = nullptr;
DocoptParserMock* _docoptParserMock = nullptr;
FileRevisorPreambleMakerMock* _fileRevisorPreambleMakerMock = nullptr;

STARTUP
{
   // Function Pointers
   _fileRevisorArgsParser._call_DetermineProgramMode = BIND_4ARG_METALMOCK_OBJECT(_call_FileRevisorArgsParser_DetermineProgramModeMock);
   // Function Callers
   _fileRevisorArgsParser._caller_ParseDirAndFromAndToArguments.reset(_caller_ParseDirAndFromAndToArgumentsMock = new NonVoidTwoArgMemberFunctionCallerMockType);
   // Constant Components
   _fileRevisorArgsParser._console.reset(_consoleMock = new ConsoleMock);
   _fileRevisorArgsParser._fileSystem.reset(_fileSystemMock = new FileSystemMock);
   _fileRevisorArgsParser._docoptParser.reset(_docoptParserMock = new DocoptParserMock);
   _fileRevisorArgsParser._fileRevisorPreambleMaker.reset(_fileRevisorPreambleMakerMock = new FileRevisorPreambleMakerMock);
}

TEST(ParseArgs_ParsesEachArgument_ReturnsFileRevisorArgs)
{
   map<string, docopt::Value> docoptValues;
   docoptValues[ZenUnit::Random<string>()] = docopt::Value(ZenUnit::Random<string>());
   _docoptParserMock->ParseArgsMock.Return(docoptValues);

   const bool isRenameFilesMode = ZenUnit::Random<bool>();
   const bool isRenameDirectoriesMode = ZenUnit::Random<bool>();
   const bool isReplaceTextInTextFilesMode = ZenUnit::Random<bool>();
   const bool isDeleteDirectoryMode = ZenUnit::Random<bool>();
   _docoptParserMock->GetRequiredBoolMock.ReturnValues(
      isRenameFilesMode,
      isRenameDirectoriesMode,
      isReplaceTextInTextFilesMode,
      isDeleteDirectoryMode);

   const bool recurse = ZenUnit::Random<bool>();
   const bool parallel = ZenUnit::Random<bool>();
   const bool skipFilesInUse = ZenUnit::Random<bool>();
   const bool dryrun = ZenUnit::Random<bool>();
   const bool quiet = ZenUnit::Random<bool>();
   const bool verbose = ZenUnit::Random<bool>();
   _docoptParserMock->GetOptionalBoolMock.ReturnValues(
      recurse, parallel, skipFilesInUse, dryrun, quiet, verbose);

   const ProgramMode programMode = ZenUnit::RandomEnum<ProgramMode>();
   _call_FileRevisorArgsParser_DetermineProgramModeMock.Return(programMode);

   const tuple<fs::path, string, string> targetDirectory_fromRegexPattern_toRegexPattern(
      ZenUnit::Random<string>(), ZenUnit::Random<string>(), ZenUnit::Random<string>());
   _caller_ParseDirAndFromAndToArgumentsMock->CallConstMemberFunctionMock.Return(targetDirectory_fromRegexPattern_toRegexPattern);

   const vector<string> stringArgs = ZenUnit::RandomVector<string>();
   //
   const FileRevisorArgs args = _fileRevisorArgsParser.ParseArgs(stringArgs);
   //
   METALMOCK(_docoptParserMock->GetRequiredBoolMock.CalledNTimes(4));
   METALMOCK(_docoptParserMock->GetOptionalBoolMock.CalledNTimes(6));

   METALMOCKTHEN(_docoptParserMock->ParseArgsMock.CalledOnceWith(FileRevisorArgs::CommandLineUsage, stringArgs)).Then(
   METALMOCKTHEN(_docoptParserMock->GetRequiredBoolMock.CalledWith(docoptValues, "rename-files"))).Then(
   METALMOCKTHEN(_docoptParserMock->GetRequiredBoolMock.CalledWith(docoptValues, "rename-directories"))).Then(
   METALMOCKTHEN(_docoptParserMock->GetRequiredBoolMock.CalledWith(docoptValues, "replace-text"))).Then(
   METALMOCKTHEN(_docoptParserMock->GetRequiredBoolMock.CalledWith(docoptValues, "delete-directory"))).Then(

   METALMOCKTHEN(_call_FileRevisorArgsParser_DetermineProgramModeMock.CalledOnceWith(
      isRenameFilesMode, isRenameDirectoriesMode, isReplaceTextInTextFilesMode, isDeleteDirectoryMode))).Then(

   METALMOCKTHEN(_caller_ParseDirAndFromAndToArgumentsMock->CallConstMemberFunctionMock.CalledOnceWith(
      &_fileRevisorArgsParser, &FileRevisorArgsParser::ParseTargetAndFromAndToArguments,
      docoptValues, isDeleteDirectoryMode))).Then(

   METALMOCKTHEN(_docoptParserMock->GetOptionalBoolMock.CalledWith(docoptValues, "--recurse"))).Then(
   METALMOCKTHEN(_docoptParserMock->GetOptionalBoolMock.CalledWith(docoptValues, "--parallel"))).Then(
   METALMOCKTHEN(_docoptParserMock->GetOptionalBoolMock.CalledWith(docoptValues, "--skip-files-in-use"))).Then(
   METALMOCKTHEN(_docoptParserMock->GetOptionalBoolMock.CalledWith(docoptValues, "--dryrun"))).Then(
   METALMOCKTHEN(_docoptParserMock->GetOptionalBoolMock.CalledWith(docoptValues, "--quiet"))).Then(
   METALMOCKTHEN(_docoptParserMock->GetOptionalBoolMock.CalledWith(docoptValues, "--verbose")));

   FileRevisorArgs expectedArgs{};
   expectedArgs.commandLine = Vector::Join(stringArgs, ' ');
   expectedArgs.programMode = programMode;
   expectedArgs.targetFolderPath = get<0>(targetDirectory_fromRegexPattern_toRegexPattern);
   expectedArgs.fromRegexPattern = get<1>(targetDirectory_fromRegexPattern_toRegexPattern);
   expectedArgs.toRegexPattern = get<2>(targetDirectory_fromRegexPattern_toRegexPattern);
   expectedArgs.recurse = recurse;
   expectedArgs.parallel = parallel;
   expectedArgs.skipFilesInUse = skipFilesInUse;
   expectedArgs.dryrun = dryrun;
   expectedArgs.quiet = quiet;
   expectedArgs.verbose = verbose;
   ARE_EQUAL(expectedArgs, args);
}

TEST(PrintPreambleLines_WritesPreambleLinesToConsole)
{
   _fileRevisorPreambleMakerMock->PrintPreambleLinesMock.Expect();
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   _fileRevisorArgsParser.PrintPreambleLines(args);
   //
   METALMOCK(_fileRevisorPreambleMakerMock->PrintPreambleLinesMock.CalledOnceWith(args));
}

TEST(ParseTargetAndFromAndToArguments_IsDeleteDirectoryModeIsTrue_ParsesDirArgumentAsRequired_ReturnsDirAndFromAndToArgumentValues)
{
   const string targetFolderPathString = _docoptParserMock->GetRequiredStringMock.ReturnRandom();

   const fs::path targetFolderPath = _fileSystemMock->GetAbsolutePathMock.ReturnRandom();

   map<string, docopt::Value> docoptValues;
   docoptValues[ZenUnit::Random<string>()] = docopt::Value(ZenUnit::Random<string>());
   //
   tuple<fs::path, string, string> targetFolderPath_fromRegexPattern_toRegexPattern =
      _fileRevisorArgsParser.ParseTargetAndFromAndToArguments(docoptValues, true);
   //
   METALMOCK(_docoptParserMock->GetRequiredStringMock.CalledOnceWith(docoptValues, "--target"));
   METALMOCK(_fileSystemMock->GetAbsolutePathMock.CalledOnceWith(targetFolderPathString));
   tuple<fs::path, string, string> expected_targetFolderPath_fromRegexPattern_toRegexPattern(targetFolderPath, "", "");
   ARE_EQUAL(expected_targetFolderPath_fromRegexPattern_toRegexPattern, targetFolderPath_fromRegexPattern_toRegexPattern);
}

TEST(ParseTargetAndFromAndToArguments_IsDeleteDirectoryModeIsFalse_FromArgumentIsEmpty_ThrowsInvalidArgumentException)
{
   const string targetFolderPathString = _docoptParserMock->GetOptionalStringWithDefaultValueMock.ReturnRandom();

   const string fromRegexPattern;
   _docoptParserMock->GetRequiredStringMock.Return(fromRegexPattern);

   map<string, docopt::Value> docoptValues;
   docoptValues[ZenUnit::Random<string>()] = docopt::Value(ZenUnit::Random<string>());
   //
   tuple<fs::path, string, string> targetFolderPath_fromRegexPattern_toRegexPattern;
   THROWS_EXCEPTION(targetFolderPath_fromRegexPattern_toRegexPattern =
      _fileRevisorArgsParser.ParseTargetAndFromAndToArguments(docoptValues, false),
      invalid_argument, "--from value cannot be empty");
   //
   METALMOCK(_docoptParserMock->GetOptionalStringWithDefaultValueMock.CalledOnceWith(docoptValues, "--target", "."));
   METALMOCK(_docoptParserMock->GetRequiredStringMock.CalledOnceWith(docoptValues, "--from"));
}

TEST(ParseTargetAndFromAndToArguments_IsDeleteDirectoryModeIsFalse_FromArgumentIsNotEmpty_ParsesDirArgumentAsOptional_ReturnsDirAndFromAndToArgumentValues)
{
   const string targetFolderPathString = _docoptParserMock->GetOptionalStringWithDefaultValueMock.ReturnRandom();

   const fs::path targetFolderPath = _fileSystemMock->GetAbsolutePathMock.ReturnRandom();

   const string fromRegexPattern = ZenUnit::Random<string>();
   const string toRegexPattern = ZenUnit::Random<string>();
   _docoptParserMock->GetRequiredStringMock.ReturnValues(fromRegexPattern, toRegexPattern);

   map<string, docopt::Value> docoptValues;
   docoptValues[ZenUnit::Random<string>()] = docopt::Value(ZenUnit::Random<string>());
   //
   tuple<fs::path, string, string> targetFolderPath_fromRegexPattern_toRegexPattern =
      _fileRevisorArgsParser.ParseTargetAndFromAndToArguments(docoptValues, false);
   //
   METALMOCK(_docoptParserMock->GetRequiredStringMock.CalledNTimes(2));
   METALMOCKTHEN(_docoptParserMock->GetOptionalStringWithDefaultValueMock.CalledOnceWith(docoptValues, "--target", ".")).Then(
   METALMOCKTHEN(_docoptParserMock->GetRequiredStringMock.CalledWith(docoptValues, "--from"))).Then(
   METALMOCKTHEN(_docoptParserMock->GetRequiredStringMock.CalledWith(docoptValues, "--to"))).Then(
   METALMOCKTHEN(_fileSystemMock->GetAbsolutePathMock.CalledOnceWith(targetFolderPathString)));
   tuple<fs::path, string, string> expected_targetFolderPath_fromRegexPattern_toRegexPattern(
      targetFolderPath, fromRegexPattern, toRegexPattern);
   ARE_EQUAL(expected_targetFolderPath_fromRegexPattern_toRegexPattern, targetFolderPath_fromRegexPattern_toRegexPattern);
}

TEST5X5(DetermineProgramMode_OneOfTheFourProgramModeBoolsTrue_ReturnsExpectedProgramMode,
   bool isRenameFilesMode, bool isRenameDirectoriesMode, bool isReplaceTextInTextFilesMode, bool isDeleteDirectoryMode, ProgramMode expectedReturnValue,
   true, false, false, false, ProgramMode::RenameFiles,
   false, true, false, false, ProgramMode::RenameDirectories,
   false, false, true, false, ProgramMode::ReplaceTextInTextFiles,
   false, false, false, true, ProgramMode::DeleteDirectory)
{
   const ProgramMode programMode = FileRevisorArgsParser::DetermineProgramMode(
      isRenameFilesMode, isRenameDirectoriesMode, isReplaceTextInTextFilesMode, isDeleteDirectoryMode);
   ARE_EQUAL(expectedReturnValue, programMode);
}

TEST(DetermineProgramMode_AllFourProgramModeBoolsAreFalse_ThrowsInvalidArgument)
{
   const bool isRenameFilesMode = false;
   const bool isRenameDirectoriesMode = false;
   const bool isReplaceTextInTextFilesMode = false;
   const bool isDeleteDirectoryMode = false;
   THROWS_EXCEPTION(FileRevisorArgsParser::DetermineProgramMode(
      isRenameFilesMode, isRenameDirectoriesMode, isReplaceTextInTextFilesMode, isDeleteDirectoryMode),
      invalid_argument, "All four program mode bools (isRenameFilesMode, isRenameDirectoriesMode, isReplaceTextInTextFilesMode, and isDeleteDirectoryMode) are unexpectedly false");
}

RUN_TESTS(FileRevisorArgsParserTests)
