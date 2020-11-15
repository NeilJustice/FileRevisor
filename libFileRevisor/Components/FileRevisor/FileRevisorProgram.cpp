#include "pch.h"
#include "libFileRevisor/Components/Console/Console.h"
#include "libFileRevisor/Components/Exceptions/TryCatchCaller.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorArgsParser.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorProgram.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgramFactory.h"
#include "libFileRevisor/Components/Time/Stopwatch.h"
#include "libFileRevisor/StaticUtilities/Exception.h"

FileRevisorProgram::FileRevisorProgram()
   // Constant Components
   : _argsParser(make_unique<FileRevisorArgsParser>())
   , _console(make_unique<Console>())
   , _fileRevisorSubProgramFactory(make_unique<FileRevisorSubProgramFactory>())
   , _tryCatchCaller(make_unique<TryCatchCaller<FileRevisorProgram, const vector<string>&>>())
   // Function Callers
   , _call_Utils_Exception_GetExceptionClassNameAndMessage(Exception::GetExceptionClassNameAndMessage)
   , _call_Utils_Vector_FromArgcArgv(Vector::FromArgcArgv)
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
   const vector<string> stringArgs = _call_Utils_Vector_FromArgcArgv(argc, argv);
   const int exitCode = _tryCatchCaller->TryCatchCall(
      this, &FileRevisorProgram::Run, stringArgs, &FileRevisorProgram::ExceptionHandler);
   const string elapsedSeconds = _stopwatch->StopAndGetElapsedSeconds();
   const string durationLine = "[FileRevisor] Duration: " + elapsedSeconds + " seconds";
   _console->WriteLine(durationLine);
   const string exitCodeLine = "[FileRevisor] ExitCode: " + to_string(exitCode);
   _console->WriteLine(exitCodeLine);
   return exitCode;
}

int FileRevisorProgram::Run(const vector<string>& stringArgs)
{
   const FileRevisorArgs args = _argsParser->ParseArgs(stringArgs);
   _argsParser->PrintPreambleLines(args);
   const shared_ptr<FileRevisorSubProgram> fileRevisorSubProgram =
      _fileRevisorSubProgramFactory->NewFileRevisorSubProgram(args.programMode);
   const int exitCode = fileRevisorSubProgram->Run(args);
   return exitCode;
}

int FileRevisorProgram::ExceptionHandler(const exception& ex, const vector<string>& /*stringArgs*/) const
{
   const string exceptionClassNameAndMessage = _call_Utils_Exception_GetExceptionClassNameAndMessage(&ex);
   const string exceptionMessage = "[FileRevisor] Error: Exception thrown: " + exceptionClassNameAndMessage;
   _console->WriteLine(exceptionMessage);
   return 1;
}
