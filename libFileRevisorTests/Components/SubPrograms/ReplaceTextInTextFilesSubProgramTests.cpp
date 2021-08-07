#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/ReplaceTextInTextFilesSubProgram.h"
#include "libFileRevisor/UtilityComponents/DataStructures/CharArray64Helper.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/DirectoryIteratorMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileOpenerCloserMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileReaderMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"
#include "libFileRevisorTests/UtilityComponents/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/UtilityComponents/FunctionCallers/Member/MetalMock/VoidTwoArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/UtilityComponents/Iteration/Math/MetalMock/OneExtraArgMemberFunctionAccumulatorMock.h"
#include "libFileRevisorTests/UtilityComponents/Strings/MetalMock/PluralizerMock.h"
#include "libFileRevisorTests/UtilityComponents/Strings/MetalMock/RegexerMock.h"

TESTS(ReplaceTextInTextFilesSubProgramTests)
AFACT(DefaultConstructor_NewsFileSystem)
FACTS(Run_ReadsTextFilesInWorkingDirectory_CallsRegexReplaceFileTextOnEachTextFilePath_Returns0)
AFACT(RegexReplaceTextInTextFile_PreviewIsTrueOrFalse_ReplacedFileTextEqualsOriginalFileText_DoesNothing_Returns0)
AFACT(RegexReplaceTextInTextFile_PreviewIsTrue_ReplacedFileTextDiffersFromOriginalFileText_DoesNotModifyFile_PrintsWouldReplaceTextInFileMessage_Returns1)
AFACT(RegexReplaceTextInTextFile_PreviewIsFalse_ReplacedFileTextDiffersFromOriginalFileText_WritesRegexReplacedTextToFile_PrintsReplacedTextInFileMessage_Returns1)
AFACT(PrintReadingFileMessageIfVerboseIsTrue_VerboseIsFalse_DoesNothing)
AFACT(PrintReadingFileMessageIfVerboseIsTrue_VerboseIsTrue_PrintsReadingFileMessage)
EVIDENCE

ReplaceTextInTextFilesSubProgram _replaceTextInTextFilesSubProgram;
// Function Pointers
const VoidTwoArgMemberFunctionCallerMock<ReplaceTextInTextFilesSubProgram, bool, const fs::path&>*
_call_PrintReadingFileMessageIfVerboseModeMock = nullptr;
// Function Callers
using OneExtraArgMemberFunctionAccumulatorMockType = OneExtraArgMemberFunctionAccumulatorMock<
   ReplaceTextInTextFilesSubProgram,
   size_t,
   vector,
   fs::path,
   const FileRevisorArgs&>;
OneExtraArgMemberFunctionAccumulatorMockType* _memberFunctionAccumulator_RegexReplaceTextInTextFileMock = nullptr;
// Base Class Constant Components
ConsoleMock* _protected_consoleMock = nullptr;
FileSystemMock* _protected_fileSystemMock = nullptr;
PluralizerMock* _protected_pluralizerMock = nullptr;
// Constant Components
RegexerMock* _regexerMock = nullptr;
// Mutable Components
DirectoryIteratorMock* _directoryIteratorMock = nullptr;

STARTUP
{
   // Function Pointers
   _replaceTextInTextFilesSubProgram._call_PrintReadingFileMessageIfVerboseMode.reset(_call_PrintReadingFileMessageIfVerboseModeMock = new VoidTwoArgMemberFunctionCallerMock<ReplaceTextInTextFilesSubProgram, bool, const fs::path&>);
   // Function Callers
   _replaceTextInTextFilesSubProgram._memberFunctionAccumulator_RegexReplaceTextInTextFile.reset(_memberFunctionAccumulator_RegexReplaceTextInTextFileMock = new OneExtraArgMemberFunctionAccumulatorMockType);
   // Base Class Constant Components
   _replaceTextInTextFilesSubProgram._protected_console.reset(_protected_consoleMock = new ConsoleMock);
   _replaceTextInTextFilesSubProgram._protected_fileSystem.reset(_protected_fileSystemMock = new FileSystemMock);
   _replaceTextInTextFilesSubProgram._protected_pluralizer.reset(_protected_pluralizerMock = new PluralizerMock);
   // Constant Components
   _replaceTextInTextFilesSubProgram._regexer.reset(_regexerMock = new RegexerMock);
   // Mutable Components
   _replaceTextInTextFilesSubProgram._directoryIterator.reset(_directoryIteratorMock = new DirectoryIteratorMock);
}

TEST(DefaultConstructor_NewsFileSystem)
{
   ReplaceTextInTextFilesSubProgram replaceTextInTextFilesSubProgram;
   // Function Pointers
   DELETE_TO_ASSERT_NEWED(replaceTextInTextFilesSubProgram._call_PrintReadingFileMessageIfVerboseMode);
   // Function Callers
   DELETE_TO_ASSERT_NEWED(replaceTextInTextFilesSubProgram._memberFunctionAccumulator_RegexReplaceTextInTextFile);
   // Base Class Constant Components
   DELETE_TO_ASSERT_NEWED(replaceTextInTextFilesSubProgram._protected_console);
   DELETE_TO_ASSERT_NEWED(replaceTextInTextFilesSubProgram._protected_fileSystem);
   DELETE_TO_ASSERT_NEWED(replaceTextInTextFilesSubProgram._protected_pluralizer);
   // Constant Components
   DELETE_TO_ASSERT_NEWED(replaceTextInTextFilesSubProgram._regexer);
   // Mutable Components
   DELETE_TO_ASSERT_NEWED(replaceTextInTextFilesSubProgram._directoryIterator);
}

TEST2X2(Run_ReadsTextFilesInWorkingDirectory_CallsRegexReplaceFileTextOnEachTextFilePath_Returns0,
   bool preview, const string& expectedMessagePrefix,
   true, "[FileRevisor] PreviewResult: Would replace text in ",
   false, "[FileRevisor] Result: Replaced text in ")
{
   _directoryIteratorMock->SetDirectoryIteratorMock.Expect();

   _directoryIteratorMock->SetFileAndDirectoryPathIgnoreSubstringsMock.Expect();

   const vector<fs::path> nonEmptyNonIgnoredTextFilePathsInTargetDirectory =
      _directoryIteratorMock->GetNonEmptyNonIgnoredTextFilePathsMock.ReturnRandom();

   const size_t numberOfFilesThatWereOrWouldBeModified =
      _memberFunctionAccumulator_RegexReplaceTextInTextFileMock->SumElementsWithFunctionMock.ReturnRandom();

   const string fileOrFiles = _protected_pluralizerMock->PotentiallyPluralizeWordMock.ReturnRandom();

   _protected_consoleMock->WriteLineMock.Expect();

   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.preview = preview;
   //
   const int exitCode = _replaceTextInTextFilesSubProgram.Run(args);
   //
   METALMOCK(_directoryIteratorMock->SetDirectoryIteratorMock.CalledOnceWith(args.targetDirectoryPath, args.recurse));
#if defined __linux__ || defined __APPLE__
   static const vector<string> fileAndDirectoryPathIgnoreSubstrings = { "/.git/" };
#elif defined _WIN32
   static const vector<string> fileAndDirectoryPathIgnoreSubstrings = { "\\.git\\" };
#endif
   METALMOCK(_directoryIteratorMock->SetFileAndDirectoryPathIgnoreSubstringsMock.CalledOnceWith(fileAndDirectoryPathIgnoreSubstrings));
   METALMOCK(_directoryIteratorMock->GetNonEmptyNonIgnoredTextFilePathsMock.CalledOnce());
   METALMOCK(_memberFunctionAccumulator_RegexReplaceTextInTextFileMock->SumElementsWithFunctionMock.CalledOnceWith(
      &_replaceTextInTextFilesSubProgram, nonEmptyNonIgnoredTextFilePathsInTargetDirectory,
      &ReplaceTextInTextFilesSubProgram::RegexReplaceTextInTextFile, args));
   METALMOCK(_protected_pluralizerMock->PotentiallyPluralizeWordMock.CalledOnceWith(numberOfFilesThatWereOrWouldBeModified, "file", "files"));
   const string expectedMessage = expectedMessagePrefix + to_string(numberOfFilesThatWereOrWouldBeModified) + " " + fileOrFiles;
   METALMOCK(_protected_consoleMock->WriteLineMock.CalledOnceWith(expectedMessage));
   IS_ZERO(exitCode);
}

TEST(RegexReplaceTextInTextFile_PreviewIsTrueOrFalse_ReplacedFileTextEqualsOriginalFileText_DoesNothing_Returns0)
{
   _call_PrintReadingFileMessageIfVerboseModeMock->ConstCallMock.Expect();

   const string textFileText = _protected_fileSystemMock->ReadTextMock.ReturnRandom();

   _regexerMock->ReplaceMock.Return(textFileText);

   const fs::path textFilePath = ZenUnit::Random<string>();
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   const size_t numberOfFilesThatWereOrWouldBeModified = _replaceTextInTextFilesSubProgram.RegexReplaceTextInTextFile(textFilePath, args);
   //
   METALMOCK(_call_PrintReadingFileMessageIfVerboseModeMock->ConstCallMock.CalledOnceWith(
      &_replaceTextInTextFilesSubProgram,
      &ReplaceTextInTextFilesSubProgram::PrintReadingFileMessageIfVerboseIsTrue,
      args.verbose, textFilePath));
   METALMOCK(_protected_fileSystemMock->ReadTextMock.CalledOnceWith(textFilePath));
   METALMOCK(_regexerMock->ReplaceMock.CalledOnceWith(textFileText, args.fromRegexPattern, args.toRegexPattern));
   IS_ZERO(numberOfFilesThatWereOrWouldBeModified);
}

TEST(RegexReplaceTextInTextFile_PreviewIsTrue_ReplacedFileTextDiffersFromOriginalFileText_DoesNotModifyFile_PrintsWouldReplaceTextInFileMessage_Returns1)
{
   _call_PrintReadingFileMessageIfVerboseModeMock->ConstCallMock.Expect();

   const string textFileText = _protected_fileSystemMock->ReadTextMock.ReturnRandom();

   const string replacedTextFileText = ZenUnit::Random<string>() + ZenUnit::Random<string>();
   _regexerMock->ReplaceMock.Return(replacedTextFileText);

   _protected_consoleMock->WriteLineMock.Expect();

   const fs::path textFilePath = ZenUnit::Random<string>();
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.preview = true;
   //
   const size_t numberOfFilesThatWereOrWouldBeModified = _replaceTextInTextFilesSubProgram.RegexReplaceTextInTextFile(textFilePath, args);
   //
   METALMOCK(_call_PrintReadingFileMessageIfVerboseModeMock->ConstCallMock.CalledOnceWith(
      &_replaceTextInTextFilesSubProgram,
      &ReplaceTextInTextFilesSubProgram::PrintReadingFileMessageIfVerboseIsTrue,
      args.verbose, textFilePath));
   METALMOCK(_protected_fileSystemMock->ReadTextMock.CalledOnceWith(textFilePath));
   METALMOCK(_regexerMock->ReplaceMock.CalledOnceWith(textFileText, args.fromRegexPattern, args.toRegexPattern));
   const string expectedReplacedTextMessage = "[FileRevisor]  Preview: Would replace text in file " + textFilePath.string();;
   METALMOCK(_protected_consoleMock->WriteLineMock.CalledOnceWith(expectedReplacedTextMessage));
   ARE_EQUAL(1, numberOfFilesThatWereOrWouldBeModified);
}

TEST(RegexReplaceTextInTextFile_PreviewIsFalse_ReplacedFileTextDiffersFromOriginalFileText_WritesRegexReplacedTextToFile_PrintsReplacedTextInFileMessage_Returns1)
{
   _call_PrintReadingFileMessageIfVerboseModeMock->ConstCallMock.Expect();

   const string textFileText = _protected_fileSystemMock->ReadTextMock.ReturnRandom();

   const string replacedTextFileText = ZenUnit::Random<string>() + ZenUnit::Random<string>();
   _regexerMock->ReplaceMock.Return(replacedTextFileText);

   _protected_fileSystemMock->CreateTextFileMock.Expect();

   _protected_consoleMock->WriteLineMock.Expect();

   const fs::path textFilePath = ZenUnit::Random<string>();
   FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   args.preview = false;
   //
   const size_t numberOfFilesThatWereOrWouldBeModified =
      _replaceTextInTextFilesSubProgram.RegexReplaceTextInTextFile(textFilePath, args);
   //
   METALMOCK(_call_PrintReadingFileMessageIfVerboseModeMock->ConstCallMock.CalledOnceWith(
      &_replaceTextInTextFilesSubProgram,
      &ReplaceTextInTextFilesSubProgram::PrintReadingFileMessageIfVerboseIsTrue,
      args.verbose, textFilePath));
   METALMOCK(_protected_fileSystemMock->ReadTextMock.CalledOnceWith(textFilePath));
   METALMOCK(_regexerMock->ReplaceMock.CalledOnceWith(textFileText, args.fromRegexPattern, args.toRegexPattern));
   METALMOCK(_protected_fileSystemMock->CreateTextFileMock.CalledOnceWith(textFilePath, replacedTextFileText));
   const string expectedReplacedTextMessage = "[FileRevisor] Replaced: Text in file " + textFilePath.string();;
   METALMOCK(_protected_consoleMock->WriteLineMock.CalledOnceWith(expectedReplacedTextMessage));
   ARE_EQUAL(1, numberOfFilesThatWereOrWouldBeModified);
}

TEST(PrintReadingFileMessageIfVerboseIsTrue_VerboseIsFalse_DoesNothing)
{
   const fs::path textFilePath = ZenUnit::Random<fs::path>();
   _replaceTextInTextFilesSubProgram.PrintReadingFileMessageIfVerboseIsTrue(false, textFilePath);
}

TEST(PrintReadingFileMessageIfVerboseIsTrue_VerboseIsTrue_PrintsReadingFileMessage)
{
   _protected_consoleMock->WriteLineMock.Expect();
   const fs::path textFilePath = ZenUnit::Random<fs::path>();
   //
   _replaceTextInTextFilesSubProgram.PrintReadingFileMessageIfVerboseIsTrue(true, textFilePath);
   //
   const string expectedReadingFileMessage = "[FileRevisor]  Verbose: Reading file " + textFilePath.string();
   METALMOCK(_protected_consoleMock->WriteLineMock.CalledOnceWith(expectedReadingFileMessage));
}

RUN_TESTS(ReplaceTextInTextFilesSubProgramTests)
