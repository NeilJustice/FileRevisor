#include "pch.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorArgsParser.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorProgram.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/StaticUtilities/Exception.h"
#include "libFileRevisorTests/Components/Exceptions/MetalMock/TryCatchCallerMock.h"
#include "libFileRevisorTests/Components/FileRevisor/MetalMock/FileRevisorArgsParserMock.h"
#include "libFileRevisorTests/Components/SubPrograms/MetalMock/FileRevisorSubProgramFactoryMock.h"
#include "libFileRevisorTests/Components/SubPrograms/MetalMock/FileRevisorSubProgramMock.h"
#include "libFileRevisorTests/UtilityComponents/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/UtilityComponents/Strings/MetalMock/PluralizerMock.h"
#include "libFileRevisorTests/UtilityComponents/Time/MetalMock/StopwatchMock.h"

TESTS(FileRevisorProgramTests)
AFACT(DefaultConstructor_SetsFunctionPointers_NewsComponents)
AFACT(Main_ArgcIs1_WritesCommandLineUsage_Returns0)
AFACT(Main_ArgcIsNot1_CallsTryCatchCallRunWithStringVectorOfArgs_PrintsElapsedTime_ReturnsTryCatchCallReturnValue)
AFACT(Run_ParsesArgs_GetsAndRunsSubProgramSpecifiedByCommandLineArguments_ReturnsSubProgramExitCode)
AFACT(ExceptionHandler_PrintsExceptionClassNameAndWhat_Returns1)
EVIDENCE

FileRevisorProgram _fileRevisorProgram;
// Function Pointers
METALMOCK_NONVOID2_STATIC(vector<string>, Vector, FromArgcArgv, int, char**)
METALMOCK_NONVOID1_STATIC(string, Exception, ClassNameAndWhat, const exception*)
// Constant Components
FileRevisorArgsParserMock* _argsParserMock = nullptr;
ConsoleMock* _consoleMock = nullptr;
FileRevisorSubProgramFactoryMock* _fileRevisorSubProgramFactoryMock = nullptr;
TryCatchCallerMock<FileRevisorProgram, const vector<string>&>* _tryCatchCallerMock = nullptr;
// Mutable Components
StopwatchMock* _stopwatchMock = nullptr;

STARTUP
{
   // Function Pointers
   _fileRevisorProgram._call_Utils_Vector_FromArgcArgv = BIND_2ARG_METALMOCK_OBJECT(FromArgcArgvMock);
   _fileRevisorProgram._call_Utils_Exception_GetExceptionClassNameAndMessage = BIND_1ARG_METALMOCK_OBJECT(ClassNameAndWhatMock);
   // Components
   _fileRevisorProgram._console.reset(_consoleMock = new ConsoleMock);
   _fileRevisorProgram._tryCatchCaller.reset(_tryCatchCallerMock = new TryCatchCallerMock<FileRevisorProgram, const vector<string>&>);
   _fileRevisorProgram._argsParser.reset(_argsParserMock = new FileRevisorArgsParserMock);
   _fileRevisorProgram._fileRevisorSubProgramFactory.reset(_fileRevisorSubProgramFactoryMock = new FileRevisorSubProgramFactoryMock);
   // Mutable Components
   _fileRevisorProgram._stopwatch.reset(_stopwatchMock = new StopwatchMock);
}

TEST(DefaultConstructor_SetsFunctionPointers_NewsComponents)
{
   FileRevisorProgram fileRevisorProgram;
   // Function Callers
   STD_FUNCTION_TARGETS(Vector::FromArgcArgv, fileRevisorProgram._call_Utils_Vector_FromArgcArgv);
   STD_FUNCTION_TARGETS(Exception::GetExceptionClassNameAndMessage, fileRevisorProgram._call_Utils_Exception_GetExceptionClassNameAndMessage);
   // Components
   DELETE_TO_ASSERT_NEWED(fileRevisorProgram._argsParser);
   DELETE_TO_ASSERT_NEWED(fileRevisorProgram._console);
   DELETE_TO_ASSERT_NEWED(fileRevisorProgram._fileRevisorSubProgramFactory);
   DELETE_TO_ASSERT_NEWED(fileRevisorProgram._tryCatchCaller);
   // Mutable Components
   DELETE_TO_ASSERT_NEWED(fileRevisorProgram._stopwatch);
}

TEST(Main_ArgcIs1_WritesCommandLineUsage_Returns0)
{
   _consoleMock->WriteLineMock.Expect();
   //
   int exitCode = _fileRevisorProgram.Main(1, nullptr);
   //
   METALMOCK(_consoleMock->WriteLineMock.CalledOnceWith(FileRevisorArgs::CommandLineUsage));
   IS_ZERO(exitCode);
}

TEST(Main_ArgcIsNot1_CallsTryCatchCallRunWithStringVectorOfArgs_PrintsElapsedTime_ReturnsTryCatchCallReturnValue)
{
   _stopwatchMock->StartMock.Expect();

   const vector<string> vectorArgs{ ZenUnit::Random<string>(), ZenUnit::Random<string>() };
   FromArgcArgvMock.Return(vectorArgs);

   int tryCatchCallReturnValue = ZenUnit::Random<int>();
   _tryCatchCallerMock->TryCatchCallMock.Return(tryCatchCallReturnValue);

   const string elapsedSeconds = _stopwatchMock->StopAndGetElapsedSecondsMock.ReturnRandom();

   _consoleMock->WriteLineMock.Expect();

   const int argc = ZenUnit::RandomBetween<int>(2, 5);
   const string exePath = ZenUnit::Random<string>();
   const string secondArg = ZenUnit::Random<string>();
   const char* argv[] = { exePath.c_str(), secondArg.c_str() };
   //
   const int exitCode = _fileRevisorProgram.Main(argc, const_cast<char**>(argv));
   //
   METALMOCK(_stopwatchMock->StartMock.CalledOnce());
   METALMOCK(FromArgcArgvMock.CalledOnceWith(argc, const_cast<char**>(argv)));
   METALMOCK(_tryCatchCallerMock->TryCatchCallMock.CalledOnceWith(
      &_fileRevisorProgram, &FileRevisorProgram::Run, vectorArgs, &FileRevisorProgram::ExceptionHandler));
   METALMOCK(_stopwatchMock->StopAndGetElapsedSecondsMock.CalledOnce());
   const string expectedDurationLine = "[FileRevisor] Duration: " + elapsedSeconds + " seconds";
   const string expectedExitCodeLine = "[FileRevisor] ExitCode: " + to_string(exitCode);
   METALMOCK(_consoleMock->WriteLineMock.CalledAsFollows(
   {
      { expectedDurationLine },
      { expectedExitCodeLine }
   }));
   ARE_EQUAL(tryCatchCallReturnValue, exitCode);
}

TEST(Run_ParsesArgs_GetsAndRunsSubProgramSpecifiedByCommandLineArguments_ReturnsSubProgramExitCode)
{
   const FileRevisorArgs args = _argsParserMock->ParseArgsMock.ReturnRandom();

   _argsParserMock->PrintPreambleLinesMock.Expect();

   shared_ptr<FileRevisorSubProgramMock> fileRevisorSubProgramMock = make_shared<FileRevisorSubProgramMock>();
   const int runReturnValue = fileRevisorSubProgramMock->RunMock.ReturnRandom();
   _fileRevisorSubProgramFactoryMock->NewFileRevisorSubProgramMock.Return(fileRevisorSubProgramMock);

   const vector<string> stringArgs = ZenUnit::RandomVector<string>();
   //
   const int exitCode = _fileRevisorProgram.Run(stringArgs);
   //
   METALMOCK(_argsParserMock->ParseArgsMock.CalledOnceWith(stringArgs));
   METALMOCK(_argsParserMock->PrintPreambleLinesMock.CalledOnceWith(args));
   METALMOCK(_fileRevisorSubProgramFactoryMock->NewFileRevisorSubProgramMock.CalledOnceWith(args.programMode));
   METALMOCK(fileRevisorSubProgramMock->RunMock.CalledOnceWith(args));
   ARE_EQUAL(runReturnValue, exitCode);
}

TEST(ExceptionHandler_PrintsExceptionClassNameAndWhat_Returns1)
{
   const string exceptionTypeNameAndMessage = ZenUnit::Random<string>();
   ClassNameAndWhatMock.Return(exceptionTypeNameAndMessage);

   _consoleMock->WriteLineMock.Expect();

   const exception ex;
   const vector<string> args = ZenUnit::RandomVector<string>();
   //
   int exitCode = _fileRevisorProgram.ExceptionHandler(ex, args);
   //
   METALMOCK(ClassNameAndWhatMock.CalledOnceWith(&ex));
   const string expectedExceptionMessage = "[FileRevisor] Error: Exception thrown: " + exceptionTypeNameAndMessage;
   METALMOCK(_consoleMock->WriteLineMock.CalledOnceWith(expectedExceptionMessage));
   ARE_EQUAL(1, exitCode);
}

RUN_TESTS(FileRevisorProgramTests)
