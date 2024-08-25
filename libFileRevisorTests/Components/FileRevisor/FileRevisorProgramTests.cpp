#include "pch.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorArgsParser.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorProgram.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisorTests/Components/FileRevisor/MetalMock/FileRevisorArgsParserMock.h"
#include "libFileRevisorTests/Components/SubPrograms/MetalMock/FileRevisorSubProgramFactoryMock.h"
#include "libFileRevisorTests/Components/SubPrograms/MetalMock/FileRevisorSubProgramMock.h"
#include "libFileRevisorTests/Components/Utility/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/Components/Utility/FunctionCallers/TryCatchCallers/MetalMock/NonVoidOneArgTryCatchCallerMock.h"
#include "libFileRevisorTests/Components/Utility/Strings/MetalMock/PluralizerMock.h"
#include "libFileRevisorTests/Components/Utility/Time/MetalMock/StopwatchMock.h"

TESTS(FileRevisorProgramTests)
AFACT(DefaultConstructor_SetsFunctionPointers_NewsComponents)
AFACT(Main_ArgcIs1_WritesCommandLineUsage_Returns0)
AFACT(Main_ArgcIsNot1_CallsTryCatchCallRunWithStringVectorOfArgs_PrintsElapsedTime_ReturnsTryCatchCallReturnValue)
AFACT(Run_ParsesArgs_GetsAndRunsSubProgramSpecifiedByCommandLineArguments_ReturnsSubProgramExitCode)
AFACT(ExceptionHandler_PrintsExceptionClassNameAndWhat_Returns1)
EVIDENCE

FileRevisorProgram _fileRevisorProgram;
// Function Pointers
METALMOCK_NONVOID2_STATIC_OR_FREE(vector<string>, _call_Vector_FromArgcArgv, int, char**)
METALMOCK_NONVOID1_STATIC_OR_FREE(string, _call_Type_GetExceptionClassNameAndMessage, const exception*)
// Constant Components
FileRevisorArgsParserMock* _argsParserMock = nullptr;
ConsoleMock* _consoleMock = nullptr;
FileRevisorSubProgramFactoryMock* _fileRevisorSubProgramFactoryMock = nullptr;
NonVoidOneArgTryCatchCallerMock<int, FileRevisorProgram, const vector<string>&>* _nonVoidOneArgTryCatchCallerMock = nullptr;
// Mutable Components
StopwatchMock* _stopwatchMock = nullptr;

STARTUP
{
   // Function Pointers
   _fileRevisorProgram._call_Vector_FromArgcArgv = BIND_2ARG_METALMOCK_OBJECT(_call_Vector_FromArgcArgvMock);
   _fileRevisorProgram._call_Type_GetExceptionClassNameAndMessage = BIND_1ARG_METALMOCK_OBJECT(_call_Type_GetExceptionClassNameAndMessageMock);
   // Components
   _fileRevisorProgram._console.reset(_consoleMock = new ConsoleMock);
   _fileRevisorProgram._nonVoidOneArgTryCatchCaller.reset(_nonVoidOneArgTryCatchCallerMock = new NonVoidOneArgTryCatchCallerMock<int, FileRevisorProgram, const vector<string>&>);
   _fileRevisorProgram._argsParser.reset(_argsParserMock = new FileRevisorArgsParserMock);
   _fileRevisorProgram._fileRevisorSubProgramFactory.reset(_fileRevisorSubProgramFactoryMock = new FileRevisorSubProgramFactoryMock);
   // Mutable Components
   _fileRevisorProgram._stopwatch.reset(_stopwatchMock = new StopwatchMock);
}

TEST(DefaultConstructor_SetsFunctionPointers_NewsComponents)
{
   FileRevisorProgram fileRevisorProgram;
   // Function Callers
   STD_FUNCTION_TARGETS(Vector::FromArgcArgv, fileRevisorProgram._call_Vector_FromArgcArgv);
   STD_FUNCTION_TARGETS(Type::GetExceptionClassNameAndMessage, fileRevisorProgram._call_Type_GetExceptionClassNameAndMessage);
   // Components
   DELETE_TO_ASSERT_NEWED(fileRevisorProgram._argsParser);
   DELETE_TO_ASSERT_NEWED(fileRevisorProgram._console);
   DELETE_TO_ASSERT_NEWED(fileRevisorProgram._fileRevisorSubProgramFactory);
   DELETE_TO_ASSERT_NEWED(fileRevisorProgram._nonVoidOneArgTryCatchCaller);
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
   _call_Vector_FromArgcArgvMock.Return(vectorArgs);

   int tryCatchCallReturnValue = ZenUnit::Random<int>();
   _nonVoidOneArgTryCatchCallerMock->TryCatchCallConstMemberFunctionMock.Return(tryCatchCallReturnValue);

   const string elapsedSeconds = _stopwatchMock->StopAndGetElapsedSecondsMock.ReturnRandom();

   _consoleMock->ProgramNameThreadIdWriteLineMock.Expect();

   const int argc = ZenUnit::RandomBetween<int>(2, 5);
   const string exePath = ZenUnit::Random<string>();
   const string secondArg = ZenUnit::Random<string>();
   const char* argv[] = { exePath.c_str(), secondArg.c_str() };
   //
   const int exitCode = _fileRevisorProgram.Main(argc, const_cast<char**>(argv));
   //
   METALMOCK(_consoleMock->ProgramNameThreadIdWriteLineMock.CalledNTimes(2));
   const string expectedDurationLine = "Duration: " + elapsedSeconds + " seconds";
   const string expectedExitCodeLine = "ExitCode: " + to_string(exitCode);
   METALMOCKTHEN(_stopwatchMock->StartMock.CalledOnce()).Then(
   METALMOCKTHEN(_call_Vector_FromArgcArgvMock.CalledOnceWith(argc, const_cast<char**>(argv)))).Then(
   METALMOCKTHEN(_nonVoidOneArgTryCatchCallerMock->TryCatchCallConstMemberFunctionMock.CalledOnceWith(
      &_fileRevisorProgram, &FileRevisorProgram::Run, vectorArgs, &FileRevisorProgram::ExceptionHandler))).Then(
   METALMOCKTHEN(_stopwatchMock->StopAndGetElapsedSecondsMock.CalledOnce())).Then(
   METALMOCKTHEN(_consoleMock->ProgramNameThreadIdWriteLineMock.CalledWith(expectedDurationLine))).Then(
   METALMOCKTHEN(_consoleMock->ProgramNameThreadIdWriteLineMock.CalledWith(expectedExitCodeLine)));
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
   METALMOCKTHEN(_argsParserMock->ParseArgsMock.CalledOnceWith(stringArgs)).Then(
   METALMOCKTHEN(_argsParserMock->PrintPreambleLinesMock.CalledOnceWith(args))).Then(
   METALMOCKTHEN(_fileRevisorSubProgramFactoryMock->NewFileRevisorSubProgramMock.CalledOnceWith(args.programMode))).Then(
   METALMOCKTHEN(fileRevisorSubProgramMock->RunMock.CalledOnceWith(args)));
   ARE_EQUAL(runReturnValue, exitCode);
}

TEST(ExceptionHandler_PrintsExceptionClassNameAndWhat_Returns1)
{
   const string exceptionTypeNameAndMessage = ZenUnit::Random<string>();
   _call_Type_GetExceptionClassNameAndMessageMock.Return(exceptionTypeNameAndMessage);

   _consoleMock->ProgramNameThreadIdWriteLineColorMock.Expect();

   const exception ex;
   //
   int exitCode = _fileRevisorProgram.ExceptionHandler(ex);
   //
   METALMOCK(_call_Type_GetExceptionClassNameAndMessageMock.CalledOnceWith(&ex));
   const string expectedExceptionMessage = "Error: Exception thrown: " + exceptionTypeNameAndMessage;
   METALMOCK(_consoleMock->ProgramNameThreadIdWriteLineColorMock.CalledOnceWith(expectedExceptionMessage, Color::Red));
   ARE_EQUAL(1, exitCode);
}

RUN_TESTS(FileRevisorProgramTests)
