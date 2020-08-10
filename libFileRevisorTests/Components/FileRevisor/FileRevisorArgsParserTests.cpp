#include "pch.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorArgsParser.h"
#include "libFileRevisor/Components/DataStructures/Vector.h"
#include "libFileRevisor/Components/Strings/StringUtil.h"
#include "libFileRevisorTests/Components/FileRevisor/MetalMock/FileRevisorPreambleMakerMock.h"
#include "libFileRevisorTests/Components/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/Components/Docopt/MetalMock/DocoptParserMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/MetalMock/NonVoidTwoArgMemberFunctionCallerMock.h"

TESTS(FileRevisorArgsParserTests)
AFACT(DefaultConstructor_NewsComponents_SetsDetermineProgramModeFunctionPointer)
AFACT(ParseArgs_ParsesEachArgument_ReturnsFileRevisorArgs)
AFACT(PrintPreambleLines_WritesPreambleLinesToConsole)
AFACT(ParseTargetAndFromAndToArguments_IsDeleteDirectoryModeIsTrue_ParsesDirArgumentAsRequired_ReturnsDirAndFromAndToArgumentValues)
AFACT(ParseTargetAndFromAndToArguments_IsDeleteDirectoryModeIsFalse_FromArgumentIsEmpty_ThrowsInvalidArgumentException)
AFACT(ParseTargetAndFromAndToArguments_IsDeleteDirectoryModeIsFalse_FromArgumentIsNotEmpty_ParsesDirArgumentAsOptional_ReturnsDirAndFromAndToArgumentValues)
FACTS(DetermineProgramMode_OneOfTheFourProgramModeBoolsTrue_ReturnsExpectedProgramMode)
AFACT(DetermineProgramMode_AllFourProgramModeBoolsAreFalse_ThrowsInvalidArgument)
EVIDENCE

FileRevisorArgsParser _fileRevisorArgsParser;
// Constant Components
ConsoleMock* _consoleMock = nullptr;
FileSystemMock* _fileSystemMock = nullptr;
DocoptParserMock* _docoptParserMock = nullptr;
FileRevisorPreambleMakerMock* _fileRevisorPreambleMakerMock = nullptr;
// Function Callers
METALMOCK_NONVOID4_STATIC(ProgramMode, FileRevisorArgsParser, DetermineProgramMode, bool, bool, bool, bool)
using NonVoidTwoArgMemberFunctionCallerMockType = NonVoidTwoArgMemberFunctionCallerMock<
   tuple<fs::path, string, string>, FileRevisorArgsParser, const map<string, docopt::Value>&, bool>;
NonVoidTwoArgMemberFunctionCallerMockType* _caller_ParseDirAndFromAndToArgumentsMock = nullptr;

STARTUP
{
   // Constant Components
   _fileRevisorArgsParser._console.reset(_consoleMock = new ConsoleMock);
   _fileRevisorArgsParser._fileSystem.reset(_fileSystemMock = new FileSystemMock);
   _fileRevisorArgsParser._docoptParser.reset(_docoptParserMock = new DocoptParserMock);
   _fileRevisorArgsParser._fileRevisorPreambleMaker.reset(_fileRevisorPreambleMakerMock = new FileRevisorPreambleMakerMock);
   // Function Callers
   _fileRevisorArgsParser._call_DetermineProgramMode = BIND_4ARG_METALMOCK_OBJECT(DetermineProgramModeMock);
   _fileRevisorArgsParser._caller_ParseDirAndFromAndToArguments.reset(
      _caller_ParseDirAndFromAndToArgumentsMock = new NonVoidTwoArgMemberFunctionCallerMockType);
}

TEST(DefaultConstructor_NewsComponents_SetsDetermineProgramModeFunctionPointer)
{
   FileRevisorArgsParser argsParser;
   // Constant Components
   DELETE_TO_ASSERT_NEWED(argsParser._console);
   DELETE_TO_ASSERT_NEWED(argsParser._fileSystem);
   DELETE_TO_ASSERT_NEWED(argsParser._docoptParser);
   DELETE_TO_ASSERT_NEWED(argsParser._fileRevisorPreambleMaker);
   // Function Callers
   STD_FUNCTION_TARGETS(FileRevisorArgsParser::DetermineProgramMode, argsParser._call_DetermineProgramMode);
   DELETE_TO_ASSERT_NEWED(argsParser._caller_ParseDirAndFromAndToArguments);
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

   const bool recursive = ZenUnit::Random<bool>();
   const bool parallel = ZenUnit::Random<bool>();
   const bool skipFilesInUse = ZenUnit::Random<bool>();
   const bool preview = ZenUnit::Random<bool>();
   const bool minimal = ZenUnit::Random<bool>();
   const bool verbose = ZenUnit::Random<bool>();
   _docoptParserMock->GetOptionalBoolMock.ReturnValues(
      recursive, parallel, skipFilesInUse, preview, minimal, verbose);

   const ProgramMode programMode = ZenUnit::RandomEnum<ProgramMode>(ProgramMode::MaxValue);
   DetermineProgramModeMock.Return(programMode);

   const tuple<fs::path, string, string> targetDirectory_fromRegexPattern_toRegexPattern(
      ZenUnit::Random<string>(), ZenUnit::Random<string>(), ZenUnit::Random<string>());
   _caller_ParseDirAndFromAndToArgumentsMock->ConstCallMock.Return(targetDirectory_fromRegexPattern_toRegexPattern);

   const vector<string> stringArgs = ZenUnit::RandomVector<string>();
   //
   const FileRevisorArgs args = _fileRevisorArgsParser.ParseArgs(stringArgs);
   //
   METALMOCK(_docoptParserMock->ParseArgsMock.CalledOnceWith(FileRevisorArgs::CommandLineUsage, stringArgs));
   METALMOCK(_docoptParserMock->GetRequiredBoolMock.CalledAsFollows(
   {
      { docoptValues, "rename-files" },
      { docoptValues, "rename-directories" },
      { docoptValues, "replace-text" },
      { docoptValues, "delete-directory" }
   }));
   METALMOCK(DetermineProgramModeMock.CalledOnceWith(
      isRenameFilesMode, isRenameDirectoriesMode, isReplaceTextInTextFilesMode, isDeleteDirectoryMode));
   METALMOCK(_caller_ParseDirAndFromAndToArgumentsMock->ConstCallMock.CalledOnceWith(
      &_fileRevisorArgsParser, &FileRevisorArgsParser::ParseTargetAndFromAndToArguments,
      docoptValues, isDeleteDirectoryMode));
   METALMOCK(_docoptParserMock->GetOptionalBoolMock.CalledAsFollows(
   {
      { docoptValues, "--recursive" },
      { docoptValues, "--parallel" },
      { docoptValues, "--skip-files-in-use" },
      { docoptValues, "--preview" },
      { docoptValues, "--minimal" },
      { docoptValues, "--verbose" }
   }));
   FileRevisorArgs expectedArgs;
   expectedArgs.commandLine = Vector::Join(stringArgs, ' ');
   expectedArgs.programMode = programMode;
   expectedArgs.targetDirectoryPath = get<0>(targetDirectory_fromRegexPattern_toRegexPattern);
   expectedArgs.fromRegexPattern = get<1>(targetDirectory_fromRegexPattern_toRegexPattern);
   expectedArgs.toRegexPattern = get<2>(targetDirectory_fromRegexPattern_toRegexPattern);
   expectedArgs.recursive = recursive;
   expectedArgs.parallel = parallel;
   expectedArgs.skipFilesInUse = skipFilesInUse;
   expectedArgs.preview = preview;
   expectedArgs.minimal = minimal;
   expectedArgs.verbose = verbose;
   ARE_EQUAL(expectedArgs, args);
}

TEST(PrintPreambleLines_WritesPreambleLinesToConsole)
{
   const string preambleLines = _fileRevisorPreambleMakerMock->MakePreambleLinesMock.ReturnRandom();
   _consoleMock->WriteLineMock.Expect();
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   _fileRevisorArgsParser.PrintPreambleLines(args);
   //
   METALMOCK(_fileRevisorPreambleMakerMock->MakePreambleLinesMock.CalledOnceWith(args));
   METALMOCK(_consoleMock->WriteLineMock.CalledOnceWith(preambleLines));
}

TEST(ParseTargetAndFromAndToArguments_IsDeleteDirectoryModeIsTrue_ParsesDirArgumentAsRequired_ReturnsDirAndFromAndToArgumentValues)
{
   const string targetDirectoryPathString = _docoptParserMock->GetRequiredStringMock.ReturnRandom();

   const fs::path targetDirectoryPath = _fileSystemMock->GetAbsolutePathMock.ReturnRandom();

   map<string, docopt::Value> docoptValues;
   docoptValues[ZenUnit::Random<string>()] = docopt::Value(ZenUnit::Random<string>());
   //
   tuple<fs::path, string, string> targetDirectoryPath_fromRegexPattern_toRegexPattern =
      _fileRevisorArgsParser.ParseTargetAndFromAndToArguments(docoptValues, true);
   //
   METALMOCK(_docoptParserMock->GetRequiredStringMock.CalledOnceWith(docoptValues, "--target"));
   METALMOCK(_fileSystemMock->GetAbsolutePathMock.CalledOnceWith(targetDirectoryPathString));
   tuple<fs::path, string, string> expected_targetDirectoryPath_fromRegexPattern_toRegexPattern(targetDirectoryPath, "", "");
   ARE_EQUAL(expected_targetDirectoryPath_fromRegexPattern_toRegexPattern, targetDirectoryPath_fromRegexPattern_toRegexPattern);
}

TEST(ParseTargetAndFromAndToArguments_IsDeleteDirectoryModeIsFalse_FromArgumentIsEmpty_ThrowsInvalidArgumentException)
{
   const string targetDirectoryPathString = _docoptParserMock->GetOptionalStringWithDefaultValueMock.ReturnRandom();

   const string fromRegexPattern;
   _docoptParserMock->GetRequiredStringMock.Return(fromRegexPattern);

   map<string, docopt::Value> docoptValues;
   docoptValues[ZenUnit::Random<string>()] = docopt::Value(ZenUnit::Random<string>());
   //
   tuple<fs::path, string, string> targetDirectoryPath_fromRegexPattern_toRegexPattern;
   THROWS_EXCEPTION(targetDirectoryPath_fromRegexPattern_toRegexPattern =
      _fileRevisorArgsParser.ParseTargetAndFromAndToArguments(docoptValues, false),
      invalid_argument, "--from value cannot be empty");
   //
   METALMOCK(_docoptParserMock->GetOptionalStringWithDefaultValueMock.CalledOnceWith(docoptValues, "--target", "."));
   METALMOCK(_docoptParserMock->GetRequiredStringMock.CalledOnceWith(docoptValues, "--from"));
}

TEST(ParseTargetAndFromAndToArguments_IsDeleteDirectoryModeIsFalse_FromArgumentIsNotEmpty_ParsesDirArgumentAsOptional_ReturnsDirAndFromAndToArgumentValues)
{
   const string targetDirectoryPathString = _docoptParserMock->GetOptionalStringWithDefaultValueMock.ReturnRandom();

   const fs::path targetDirectoryPath = _fileSystemMock->GetAbsolutePathMock.ReturnRandom();

   const string fromRegexPattern = ZenUnit::Random<string>();
   const string toRegexPattern = ZenUnit::Random<string>();
   _docoptParserMock->GetRequiredStringMock.ReturnValues(fromRegexPattern, toRegexPattern);

   map<string, docopt::Value> docoptValues;
   docoptValues[ZenUnit::Random<string>()] = docopt::Value(ZenUnit::Random<string>());
   //
   tuple<fs::path, string, string> targetDirectoryPath_fromRegexPattern_toRegexPattern =
      _fileRevisorArgsParser.ParseTargetAndFromAndToArguments(docoptValues, false);
   //
   METALMOCK(_docoptParserMock->GetOptionalStringWithDefaultValueMock.CalledOnceWith(docoptValues, "--target", "."));
   METALMOCK(_docoptParserMock->GetRequiredStringMock.CalledAsFollows(
   {
      { docoptValues, "--from" },
      { docoptValues, "--to" }
   }));
   METALMOCK(_fileSystemMock->GetAbsolutePathMock.CalledOnceWith(targetDirectoryPathString));
   tuple<fs::path, string, string> expected_targetDirectoryPath_fromRegexPattern_toRegexPattern(
      targetDirectoryPath, fromRegexPattern, toRegexPattern);
   ARE_EQUAL(expected_targetDirectoryPath_fromRegexPattern_toRegexPattern, targetDirectoryPath_fromRegexPattern_toRegexPattern);
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
