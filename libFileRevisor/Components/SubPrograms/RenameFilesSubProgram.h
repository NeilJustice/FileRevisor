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
   unique_ptr<const VoidTwoArgMemberFunctionCaller<
      RenameFilesSubProgram, bool, const fs::path& >> _caller_PrintDidNotMatchFileMessageIfVerboseMode;
   using OneExtraArgMemberFunctionTransformerType = OneExtraArgMemberFunctionTransformer<
      RenameFilesSubProgram, fs::path, RenameResult, const FileRevisorArgs&>;
   unique_ptr<const OneExtraArgMemberFunctionTransformerType> _transformer_RenameFileIfFileNameMatchesFromPattern;
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
