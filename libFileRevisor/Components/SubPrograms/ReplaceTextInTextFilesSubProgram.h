#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"
template<
   typename ClassType,
   typename AccumulatedType,
   template<typename _ElementType, typename...>
typename ContainerType,
typename ElementType,
typename ExtraArgType>
class OneExtraArgMemberFunctionSumator;

class Console;

template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgMemberFunctionCaller;

struct FileRevisorArgs;
class Regexer;

class ReplaceTextInTextFilesSubProgram : public FileRevisorSubProgram
{
   friend class ReplaceTextInTextFilesSubProgramTests;
private:
   using OneExtraArgMemberFunctionSumatorType = OneExtraArgMemberFunctionSumator<
      ReplaceTextInTextFilesSubProgram, size_t, vector, fs::path, const FileRevisorArgs&>;
   unique_ptr<const OneExtraArgMemberFunctionSumatorType> _oneExtraArgMemberFunctionSumator;
   unique_ptr<const VoidTwoArgMemberFunctionCaller<
      ReplaceTextInTextFilesSubProgram, bool, const fs::path&>> _call_PrintReadingFileMessageIfVerboseMode;
   unique_ptr<const Regexer> _regexer;
public:
   ReplaceTextInTextFilesSubProgram();
   int Run(const FileRevisorArgs& args) const override;
private:
   void PrintReadingFileMessageIfVerboseIsTrue(bool verbose, const fs::path& textFilePath) const;
   size_t RegexReplaceTextInTextFile(const fs::path& textFilePath, const FileRevisorArgs& args) const;
};
