#pragma once
class Console;
class Stopwatch;
template<typename ReturnType, typename ClassType, typename ArgumentType>
class NonVoidOneArgTryCatchCaller;
class FileRevisorArgsParser;
class FileRevisorSubProgramFactory;

class FileRevisorProgram
{
   friend class FileRevisorProgramTests;
private:
   // Function Pointers
   function<string(const exception*)> _call_Type_GetExceptionClassNameAndMessage;
   function<vector<string>(int, char**)> _call_Vector_FromArgcArgv;
   // Constant Components
   unique_ptr<const FileRevisorArgsParser> _argsParser;
   unique_ptr<const Console> _console;
   unique_ptr<const FileRevisorSubProgramFactory> _fileRevisorSubProgramFactory;
   using _nonVoidOneArgTryCatchCallerType = NonVoidOneArgTryCatchCaller<int, FileRevisorProgram, const vector<string>&>;
   unique_ptr<const _nonVoidOneArgTryCatchCallerType> _nonVoidOneArgTryCatchCaller;
   // Mutable Components
   unique_ptr<Stopwatch> _stopwatch;
public:
   FileRevisorProgram();
   virtual ~FileRevisorProgram();
   int Main(int argc, char* argv[]);
private:
   int Run(const vector<string>& stringArgs) const;
   int ExceptionHandler(const exception& ex) const;
};
