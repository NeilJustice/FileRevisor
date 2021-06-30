#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

class Console;
class DirectoryIterator;
template<
   typename ClassType,
   typename AccumulatedType,
   template<typename...>
   typename ContainerType,
   typename ElementType,
   typename ExtraArgType>
class OneExtraArgMemberFunctionAccumulator;
class Regexer;
template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgMemberFunctionCaller;

class ReplaceTextInTextFilesSubProgram : public FileRevisorSubProgram
{
   friend class ReplaceTextInTextFilesSubProgramTests;
private:
   // Function Pointers
   unique_ptr<const VoidTwoArgMemberFunctionCaller<
      ReplaceTextInTextFilesSubProgram, bool, const fs::path&>> _call_PrintReadingFileMessageIfVerboseMode;
   using OneExtraArgMemberFunctionAccumulatorType = OneExtraArgMemberFunctionAccumulator<
      ReplaceTextInTextFilesSubProgram, size_t, vector, fs::path, const FileRevisorArgs&>;
   // Function Callers
   unique_ptr<const OneExtraArgMemberFunctionAccumulatorType> _memberFunctionAccumulator_RegexReplaceTextInTextFile;
   // Constant Components
   unique_ptr<const Regexer> _regexer;
   // Mutable Components
   unique_ptr<DirectoryIterator> _directoryIterator;
public:
   ReplaceTextInTextFilesSubProgram();
   virtual ~ReplaceTextInTextFilesSubProgram();
   int Run(const FileRevisorArgs& args) const override;
private:
   void PrintReadingFileMessageIfVerboseIsTrue(bool verbose, const fs::path& textFilePath) const;
   size_t RegexReplaceTextInTextFile(const fs::path& textFilePath, const FileRevisorArgs& args) const;
};
