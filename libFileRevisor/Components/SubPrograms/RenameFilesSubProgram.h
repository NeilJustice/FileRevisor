#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"
template<typename T>
class PredicateCounter;

template<typename ClassType, typename ElementType, typename TransformedElementType, typename ExtraArgType>
class OneExtraArgMemberFunctionTransformer;

class TextReplacer;

template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgMemberFunctionCaller;

class RenameFilesSubProgram : public FileRevisorSubProgram
{
   friend class RenameFilesSubProgramTests;
private:
   // Function Callers
   using _caller_PrintDidNotMatchFileMessageIfVerboseModeType =
      VoidTwoArgMemberFunctionCaller<RenameFilesSubProgram, bool, const fs::path&>;
   unique_ptr<const _caller_PrintDidNotMatchFileMessageIfVerboseModeType> _caller_PrintDidNotMatchFileMessageIfVerboseMode;

   using _transformer_RenameFileIfFileNameMatchesFromPatternType =
      OneExtraArgMemberFunctionTransformer<RenameFilesSubProgram, fs::path, RenameResult, const FileRevisorArgs&>;
   unique_ptr<const _transformer_RenameFileIfFileNameMatchesFromPatternType> _transformer_RenameFileIfFileNameMatchesFromPattern;
   // Constant Components
   unique_ptr<const PredicateCounter<RenameResult>> _predicateCounter;
   unique_ptr<const TextReplacer> _textReplacer;
public:
   RenameFilesSubProgram();
   ~RenameFilesSubProgram() override;
   int Run(const FileRevisorArgs& args) const override;
private:
   static bool DidRenameFileIsTrue(const RenameResult& fileRenameResult);
   RenameResult RenameFileIfFileNameMatchesFromPattern(const fs::path& filePath, const FileRevisorArgs& args) const;
   void PrintDidNotMatchFileMessageIfVerboseMode(bool verbose, const fs::path& filePath) const;
};
