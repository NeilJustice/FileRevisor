#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"
class Console;
struct FileRevisorArgs;

template<
   typename ClassType,
   typename AccumulatedType,
   template<typename...>
   typename ContainerType,
   typename ElementType,
   typename ExtraArgType>
class OneExtraArgMemberFunctionSumator;
class Regexer;

template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgMemberFunctionCaller;

class ReplaceTextInTextFilesSubProgram : public FileRevisorSubProgram
{
   friend class ReplaceTextInTextFilesSubProgramTests;
private:
   // Constant Components
   unique_ptr<const Regexer> _regexer;
   // Function Callers
   unique_ptr<const VoidTwoArgMemberFunctionCaller<
      ReplaceTextInTextFilesSubProgram, bool, const fs::path&>> _call_PrintReadingFileMessageIfVerboseMode;
   using OneExtraArgMemberFunctionSumatorType = OneExtraArgMemberFunctionSumator<
      ReplaceTextInTextFilesSubProgram, size_t, vector, fs::path, const FileRevisorArgs&>;
   unique_ptr<const OneExtraArgMemberFunctionSumatorType> _memberFunctionSumator_RegexReplaceTextInTextFile;
public:
   ReplaceTextInTextFilesSubProgram();
   virtual ~ReplaceTextInTextFilesSubProgram() = default;
   int Run(const FileRevisorArgs& args) const override;
private:
   void PrintReadingFileMessageIfVerboseIsTrue(bool verbose, const fs::path& textFilePath) const;
   size_t RegexReplaceTextInTextFile(const fs::path& textFilePath, const FileRevisorArgs& args) const;
};
