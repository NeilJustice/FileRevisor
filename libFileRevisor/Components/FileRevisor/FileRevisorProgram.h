#pragma once
#include <functional>
#include <memory>
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
   std::function<std::string(const std::exception*)> _call_Type_GetExceptionClassNameAndMessage;
   std::function<std::vector<std::string>(int, char**)> _call_Vector_FromArgcArgv;
   // Constant Components
   std::unique_ptr<const FileRevisorArgsParser> _argsParser;
   std::unique_ptr<const Console> _console;
   std::unique_ptr<const FileRevisorSubProgramFactory> _fileRevisorSubProgramFactory;
   std::unique_ptr<const NonVoidOneArgTryCatchCaller<int, FileRevisorProgram, const std::vector<std::string>&>> _nonVoidOneArgTryCatchCaller;
   // Mutable Components
   std::unique_ptr<Stopwatch> _stopwatch;
public:
   FileRevisorProgram();
   virtual ~FileRevisorProgram();
   int Main(int argc, char* argv[]);
private:
   int Run(const std::vector<std::string>& stringArgs) const;
   int ExceptionHandler(const std::exception& ex) const;
};
