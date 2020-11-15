#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"
class Regexer;

template<typename ContainerType, typename T>
class PredicateCounter;

template<typename ClassType, typename ElementType, typename TransformedElementType, typename ExtraArgType>
class OneExtraArgMemberFunctionTransformer;

template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgMemberFunctionCaller;

class RenameFilesSubProgram : public FileRevisorSubProgram
{
   friend class RenameFilesSubProgramTests;
private:
   // Constant Components
   unique_ptr<const PredicateCounter<vector<RenameResult>, RenameResult>> _predicateCounter;
   unique_ptr<const Regexer> _regexer;
   // Function Callers
   unique_ptr<const VoidTwoArgMemberFunctionCaller<
      RenameFilesSubProgram, bool, const fs::path& >> _caller_PrintDidNotMatchFileMessageIfVerboseMode;
   using OneExtraArgMemberFunctionTransformerType = OneExtraArgMemberFunctionTransformer<
      RenameFilesSubProgram, fs::path, RenameResult, const FileRevisorArgs&>;
   unique_ptr<const OneExtraArgMemberFunctionTransformerType> _transformer_RenameFileIfFileNameMatchesFromPattern;
public:
   RenameFilesSubProgram();
   virtual ~RenameFilesSubProgram() = default;
   int Run(const FileRevisorArgs& args) const override;
private:
   static bool DidRenameFileIsTrue(const RenameResult& fileRenameResult);
   RenameResult RenameFileIfFileNameMatchesFromPattern(const fs::path& filePath, const FileRevisorArgs& args) const;
   void PrintDidNotMatchFileMessageIfVerboseMode(bool verbose, const fs::path& filePath) const;
};
