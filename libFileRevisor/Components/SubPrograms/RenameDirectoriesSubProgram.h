#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

template<typename ClassType, typename ElementType, typename TransformedElementType, typename ExtraArgType>
class OneExtraArgMemberFunctionTransformer;

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
   unique_ptr<const VoidTwoArgMemberFunctionCaller<RenameDirectoriesSubProgram, bool, const fs::path& >>
      _call_PrintDidNotMatchDirectoryMessageIfVerboseMode;
   // Function Callers
   using OneExtraArgMemberFunctionTransformerType = OneExtraArgMemberFunctionTransformer<
      RenameDirectoriesSubProgram, fs::path, RenameResult, const FileRevisorArgs&>;
   unique_ptr<const OneExtraArgMemberFunctionTransformerType> _directoryPathsTransformer_RenameDirectory;
   // Constant Components
   unique_ptr<const PredicateCounter<RenameResult>> _predicateCounter;
   unique_ptr<const TextReplacer> _textReplacer;
public:
   RenameDirectoriesSubProgram();
   ~RenameDirectoriesSubProgram() override;
   int Run(const FileRevisorArgs& args) const override;
private:
   RenameResult RenameDirectory(const fs::path& directoryPath, const FileRevisorArgs& args) const;
   void PrintDidNotMatchDirectoryMessageIfVerboseMode(bool verbose, const fs::path& directoryPath) const;
};
