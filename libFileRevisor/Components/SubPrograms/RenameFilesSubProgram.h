#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"
template<typename T>
class PredicateCounter;

template<typename ClassType, typename ElementType, typename TransformedElementType>
class OneArgMemberFunctionTransformer;

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
      OneArgMemberFunctionTransformer<RenameFilesSubProgram, fs::path, RenameResult>;
   unique_ptr<const _transformer_RenameFileIfFileNameMatchesFromPatternType> _transformer_RenameFileIfFileNameMatchesFromPattern;
   // Constant Components
   unique_ptr<const PredicateCounter<RenameResult>> _predicateCounter;
   unique_ptr<const TextReplacer> _textReplacer;
public:
   RenameFilesSubProgram();
   ~RenameFilesSubProgram() override;
   int Run() const override;
private:
   static bool DidRenameFileIsTrue(const RenameResult& fileRenameResult);
   RenameResult RenameFileIfFileNameMatchesFromPattern(const fs::path& filePath) const;
   void PrintDidNotMatchFileMessageIfVerboseMode(bool verbose, const fs::path& filePath) const;
};
