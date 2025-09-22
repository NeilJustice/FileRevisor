#include "pch.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorArgsParser.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorProgram.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgramFactory.h"
#include "libFileRevisor/Components/FunctionCallers/TryCatchCallers/NonVoidOneArgTryCatchCaller.h"
#include "libFileRevisor/Components/Time/Stopwatch.h"

FileRevisorProgram::FileRevisorProgram()
   // Function Pointers
   : _call_Type_GetExceptionClassNameAndMessage(Type::GetExceptionClassNameAndMessage)
   , _call_Vector_FromArgcArgv(Vector::FromArgcArgv)
   // Constant Components
   , _argsParser(make_unique<FileRevisorArgsParser>())
   , _console(make_unique<Console>())
   , _fileRevisorSubProgramFactory(make_unique<FileRevisorSubProgramFactory>())
   , _nonVoidOneArgTryCatchCaller(make_unique<NonVoidOneArgTryCatchCaller<int, FileRevisorProgram, const vector<string>&>>())
   // Mutable Components
   , _stopwatch(make_unique<Stopwatch>())
{
}

FileRevisorProgram::~FileRevisorProgram()
{
}

int FileRevisorProgram::Main(int argc, char* argv[])
{
   if (argc == 1)
   {
      _console->WriteLine(FileRevisorArgs::CommandLineUsage);
      return 0;
   }
   _stopwatch->Start();
   const vector<string> stringArgs = _call_Vector_FromArgcArgv(argc, argv);
   const int exitCode = _nonVoidOneArgTryCatchCaller->TryCatchCallConstMemberFunction(
      this, &FileRevisorProgram::Run, stringArgs, &FileRevisorProgram::ExceptionHandler);
   const string elapsedSeconds = _stopwatch->StopAndGetElapsedSeconds();
   const string durationLine = "Duration: " + elapsedSeconds + " seconds";
   _console->ProgramNameThreadIdWriteLine(durationLine);
   const string exitCodeLine = "ExitCode: " + to_string(exitCode);
   _console->ProgramNameThreadIdWriteLine(exitCodeLine);
   return exitCode;
}

int FileRevisorProgram::Run(const vector<string>& stringArgs) const
{
   const FileRevisorArgs args = _argsParser->ParseStringArgs(stringArgs);
   _argsParser->PrintPreambleLines(args);
   const shared_ptr<FileRevisorSubProgram> fileRevisorSubProgram = _fileRevisorSubProgramFactory->NewSubProgram(args.programMode);
   int exitCode = fileRevisorSubProgram->Run(args);
   return exitCode;
}

int FileRevisorProgram::ExceptionHandler(const exception& ex) const
{
   const string exceptionClassNameAndMessage = _call_Type_GetExceptionClassNameAndMessage(&ex);
   const string exceptionMessage = "Error: Exception thrown: " + exceptionClassNameAndMessage;
   _console->ProgramNameThreadIdWriteLineColor(exceptionMessage, Color::Red);
   return 1;
}
