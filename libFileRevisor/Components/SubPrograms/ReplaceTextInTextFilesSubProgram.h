#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

class Console;
class DirectoryIterator;
template<
   typename ClassType,
   typename AccumulatedType,
   typename ElementType,
   typename ExtraArgType>
class OneExtraArgMemberFunctionAccumulator;
class TextReplacer;

template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgMemberFunctionCaller;

class ReplaceTextInTextFilesSubProgram : public FileRevisorSubProgram
{
   friend class ReplaceTextInTextFilesSubProgramTests;
private:
   // Function Pointers
   unique_ptr<const VoidTwoArgMemberFunctionCaller<ReplaceTextInTextFilesSubProgram, bool, const fs::path&>>
      _call_PrintReadingFileMessageIfVerboseMode;
   // Function Callers
   using OneExtraArgMemberFunctionAccumulatorType = OneExtraArgMemberFunctionAccumulator<
      ReplaceTextInTextFilesSubProgram, size_t, fs::path, const FileRevisorArgs&>;
   unique_ptr<const OneExtraArgMemberFunctionAccumulatorType> _memberFunctionAccumulator_ReplaceTextInTextFile;
   // Constant Components
   unique_ptr<const TextReplacer> _textReplacer;
   // Mutable Components
   unique_ptr<DirectoryIterator> _directoryIterator;
public:
   ReplaceTextInTextFilesSubProgram();
   ~ReplaceTextInTextFilesSubProgram() override;
   int Run(const FileRevisorArgs& args) const override;
private:
   void PrintReadingFileMessageIfVerboseIsTrue(bool verbose, const fs::path& textFilePath) const;
   size_t ReplaceTextInTextFile(const fs::path& textFilePath, const FileRevisorArgs& args) const;
};
