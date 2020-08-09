#include "pch.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorArgsParser.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorProgram.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgramFactory.h"
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"

FileRevisorProgram::FileRevisorProgram()
	: _call_Utils_Vector_FromArgcArgv(Vector::FromArgcArgv)
   , _call_Utils_Exception_ClassNameAndWhat(Exception::ClassNameAndWhat)
   , _argsParser(make_unique<FileRevisorArgsParser>())
   , _console(make_unique<Console>())
   , _fileRevisorSubProgramFactory(make_unique<FileRevisorSubProgramFactory>())
   , _tryCatchCaller(make_unique<TryCatchCaller<FileRevisorProgram, const vector<string>&>>())
	, _fileRevisorRunTimeStopwatch(make_unique<Stopwatch>())
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
   _fileRevisorRunTimeStopwatch->Start();
   const vector<string> stringArgs = _call_Utils_Vector_FromArgcArgv(argc, argv);
   const int exitCode = _tryCatchCaller->TryCatchCall(
      this, &FileRevisorProgram::Run, stringArgs, &FileRevisorProgram::ExceptionHandler);
   const string elapsedSeconds = _fileRevisorRunTimeStopwatch->StopAndGetElapsedSeconds();
   const string durationLine = "[FileRevisor] Duration: " + elapsedSeconds + " seconds";
   _console->WriteLine(durationLine);
   const string exitCodeLine = "[FileRevisor] ExitCode: " + to_string(exitCode);
   _console->WriteLine(exitCodeLine);
   return exitCode;
}

int FileRevisorProgram::Run(const vector<string>& stringArgs)
{
   const FileRevisorArgs args = _argsParser->ParseArgs(stringArgs);
   _argsParser->PrintPreamble(args);
   const shared_ptr<FileRevisorSubProgram> fileRevisorSubProgram =
      _fileRevisorSubProgramFactory->NewFileRevisorSubProgram(args.programMode);
   const int exitCode = fileRevisorSubProgram->Run(args);
   return exitCode;
}

int FileRevisorProgram::ExceptionHandler(const exception& ex, const vector<string>& /*stringArgs*/)
{
   const string exceptionTypeNameAndWhat = _call_Utils_Exception_ClassNameAndWhat(&ex);
   const string exceptionMessage = "[FileRevisor] Error: Uncaught exception: " + exceptionTypeNameAndWhat;
   _console->WriteLine(exceptionMessage);
   return 1;
}
