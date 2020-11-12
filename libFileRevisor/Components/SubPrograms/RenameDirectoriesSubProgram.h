#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"
class Regexer;

template<typename ClassType, typename ElementType, typename TransformedElementType, typename ExtraArgType>
class OneExtraArgMemberFunctionTransformer;

template<typename ContainerType, typename T>
class PredicateCounter;

template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgMemberFunctionCaller;

class RenameDirectoriesSubProgram : public FileRevisorSubProgram
{
   friend class RenameDirectoriesSubProgramTests;
private:
   // Constant Components
   unique_ptr<const PredicateCounter<vector<RenameResult>, RenameResult>> _predicateCounter;
   unique_ptr<const Regexer> _regexer;
   // Function Callers
   unique_ptr<const VoidTwoArgMemberFunctionCaller<
      RenameDirectoriesSubProgram, bool, const fs::path& >> _call_PrintDidNotMatchDirectoryMessageIfVerboseMode;
   using OneExtraArgMemberFunctionTransformerType = OneExtraArgMemberFunctionTransformer<
      RenameDirectoriesSubProgram, fs::path, RenameResult, const FileRevisorArgs&>;
   unique_ptr<const OneExtraArgMemberFunctionTransformerType> _directoryPathsTransformer_RenameDirectory;
public:
   RenameDirectoriesSubProgram();
   virtual ~RenameDirectoriesSubProgram() = default;
   int Run(const FileRevisorArgs& args) const override;
private:
   RenameResult RenameDirectory(const fs::path& directoryPath, const FileRevisorArgs& args) const;
   void PrintDidNotMatchDirectoryMessageIfVerboseMode(bool verbose, const fs::path& directoryPath) const;
};
