#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"
#include "libFileRevisor/ValueTypes/RenameResult.h"
class Regexer;
template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgMemberFunctionCaller;

class RenameFilesSubProgram : public FileRevisorSubProgram
{
   friend class RenameFilesSubProgramTests;
private:
   using OneExtraArgMemberFunctionTransformerType = OneExtraArgMemberFunctionTransformer<
      RenameFilesSubProgram, fs::path, RenameResult, const FileRevisorArgs&>;
   unique_ptr<const OneExtraArgMemberFunctionTransformerType> _oneExtraArgMemberFunctionTransformer;
   unique_ptr<const Regexer> _regexer;
   unique_ptr<const VoidTwoArgMemberFunctionCaller<
      RenameFilesSubProgram, bool, const fs::path& >> _caller_PrintDidNotMatchFileMessageIfVerboseMode;
   unique_ptr<const PredicateCounter<vector<RenameResult>, RenameResult>> _predicateCounter;
public:
   RenameFilesSubProgram();
   int Run(const FileRevisorArgs& args) const override;
private:
   static bool DidRenameFileIsTrue(const RenameResult& fileRenameResult);
   RenameResult RenameFileIfFileNameMatchesFromPattern(const fs::path& filePath, const FileRevisorArgs& args) const;
   void PrintDidNotMatchFileMessageIfVerboseMode(bool verbose, const fs::path& filePath) const;
};
