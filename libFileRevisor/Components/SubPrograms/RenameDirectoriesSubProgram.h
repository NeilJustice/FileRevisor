#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"
template<typename ClassType, typename ElementType, typename TransformedElementType>
class OneArgMemberFunctionTransformer;
template<typename T>
class PredicateCounter;
class TextReplacer;

template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgMemberFunctionCaller;

class RenameDirectoriesSubProgram : public FileRevisorSubProgram
{
   friend class RenameDirectoriesSubProgramTests;
private:
   // Function Pointers
   using _call_PrintDidNotMatchDirectoryMessageIfVerboseModeType = VoidTwoArgMemberFunctionCaller<RenameDirectoriesSubProgram, bool, const fs::path&>;
   unique_ptr<const _call_PrintDidNotMatchDirectoryMessageIfVerboseModeType> _call_PrintDidNotMatchDirectoryMessageIfVerboseMode;
   // Function Callers
   using _directoryPathsTransformer_RenameDirectoryType =
      OneArgMemberFunctionTransformer<RenameDirectoriesSubProgram, fs::path, RenameResult>;
   unique_ptr<const _directoryPathsTransformer_RenameDirectoryType> _directoryPathsTransformer_RenameDirectory;
   // Constant Components
   unique_ptr<const PredicateCounter<RenameResult>> _predicateCounter;
   unique_ptr<const TextReplacer> _textReplacer;
public:
   RenameDirectoriesSubProgram();
   ~RenameDirectoriesSubProgram() override;
   int Run() const override;
private:
   RenameResult RenameDirectory(const fs::path& directoryPath) const;
   void PrintDidNotMatchDirectoryMessageIfVerboseMode(bool verbose, const fs::path& directoryPath) const;
};
