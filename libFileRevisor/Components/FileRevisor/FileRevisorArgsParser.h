#pragma once
class Console;
class DocoptParser;
class FileSystem;
namespace docopt
{
   struct Value;
}
class FileRevisorPreambleMaker;
template<typename ReturnType, typename ClassType, typename Arg1Type, typename Arg2Type>
class NonVoidTwoArgMemberFunctionCaller;

class FileRevisorArgsParser
{
   friend class FileRevisorArgsParserTests;
private:
   // Function Pointers
   function<ProgramMode(bool, bool, bool, bool)> _call_DetermineProgramMode;
   using NonVoidTwoArgMemberFunctionCallerType = NonVoidTwoArgMemberFunctionCaller<
      tuple<fs::path, string, string>, FileRevisorArgsParser, const map<string, docopt::Value>&, bool>;
   // Function Callers
   unique_ptr<const NonVoidTwoArgMemberFunctionCallerType> _caller_ParseDirAndFromAndToArguments;
   // Constant Components
   unique_ptr<const Console> _console;
   unique_ptr<const FileSystem> _fileSystem;
   unique_ptr<const DocoptParser> _docoptParser;
   unique_ptr<const FileRevisorPreambleMaker> _fileRevisorPreambleMaker;
public:
   FileRevisorArgsParser();
   virtual ~FileRevisorArgsParser();
   virtual FileRevisorArgs ParseArgs(const vector<string>& stringArgs) const;
   virtual void PrintPreambleLines(const FileRevisorArgs& args) const;
private:
   static ProgramMode DetermineProgramMode(
      bool isRenameFilesMode,
      bool isRenameDirectoriesMode,
      bool isReplaceTextInTextFilesMode,
      bool isDeleteDirectoryMode);
   tuple<fs::path, string, string> ParseTargetAndFromAndToArguments(
      const map<string, docopt::Value>& docoptValues, bool isDeleteDirectoryMode) const;
};
