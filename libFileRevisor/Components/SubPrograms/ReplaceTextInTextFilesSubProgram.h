#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

class DirectoryIterator;
template<
   typename ClassType,
   typename AccumulatedType,
   typename ElementType>
class OneArgMemberFunctionAccumulator;
class TextReplacer;

template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgMemberFunctionCaller;

class ReplaceTextInTextFilesSubProgram : public FileRevisorSubProgram
{
   friend class ReplaceTextInTextFilesSubProgramTests;
private:
   // Function Pointers
   using _call_PrintReadingFileMessageIfVerboseModeType =
      VoidTwoArgMemberFunctionCaller<ReplaceTextInTextFilesSubProgram, bool, const fs::path&>;
   unique_ptr<const _call_PrintReadingFileMessageIfVerboseModeType> _call_PrintReadingFileMessageIfVerboseMode;
   // Function Callers
   using _memberFunctionAccumulator_ReplaceTextInTextFileType = OneArgMemberFunctionAccumulator<ReplaceTextInTextFilesSubProgram, size_t, fs::path>;
   unique_ptr<const _memberFunctionAccumulator_ReplaceTextInTextFileType> _memberFunctionAccumulator_ReplaceTextInTextFile;
   // Constant Components
   unique_ptr<const TextReplacer> _textReplacer;
   // Mutable Components
   unique_ptr<DirectoryIterator> _directoryIterator;
public:
   ReplaceTextInTextFilesSubProgram();
   ~ReplaceTextInTextFilesSubProgram() override;
   int Run() const override;
private:
   void PrintReadingFileMessageIfVerboseIsTrue(bool verbose, const fs::path& textFilePath) const;
   size_t ReplaceTextInTextFile(const fs::path& textFilePath) const;
};
