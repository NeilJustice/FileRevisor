#pragma once
#include <functional>
#include <memory>
class Console;
class Stopwatch;
template<typename ClassType, typename ArgumentType>
class TryCatchCaller;
class FileRevisorArgsParser;
class FileRevisorSubProgramFactory;

class FileRevisorProgram
{
   friend class FileRevisorProgramTests;
private:
   // Constant Components
   std::unique_ptr<const FileRevisorArgsParser> _argsParser;
   std::unique_ptr<const Console> _console;
   std::unique_ptr<const FileRevisorSubProgramFactory> _fileRevisorSubProgramFactory;
   std::unique_ptr<const TryCatchCaller<FileRevisorProgram, const std::vector<std::string>&>> _tryCatchCaller;
   // Function Callers
   std::function<std::string(const std::exception*)> _call_Utils_Exception_ClassNameAndWhat;
   std::function<std::vector<std::string>(int, char**)> _call_Utils_Vector_FromArgcArgv;
   // Mutable Components
   std::unique_ptr<Stopwatch> _stopwatch;
public:
   FileRevisorProgram();
   virtual ~FileRevisorProgram();
   int Main(int argc, char* argv[]);
private:
   int Run(const std::vector<std::string>& stringArgs);
   int ExceptionHandler(const std::exception& ex, const std::vector<std::string>& stringArgs) const;
};
