#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

template<typename ClassType, typename T, typename Arg2Type>
class TwoArgMemberFunctionForEacher;

template<typename ClassType, typename T, typename Arg2Type>
class ParallelTwoArgMemberFunctionForEacher;

template<typename ClassType, typename Arg1Type>
class VoidOneArgMemberFunctionCaller;

template<typename ClassType, typename Argument1Type, typename Argument2Type>
class VoidTwoArgTryCatchCaller;

class DeleteDirectorySubProgram : public FileRevisorSubProgram
{
   friend class DeleteDirectorySubProgramTests;
private:
   // Function Callers
   using _caller_DeleteTargetDirectoryIfNotCurrentDirectoryType =
      VoidOneArgMemberFunctionCaller<DeleteDirectorySubProgram, const FileRevisorArgs&>;
   unique_ptr<const _caller_DeleteTargetDirectoryIfNotCurrentDirectoryType> _caller_DeleteTargetDirectoryIfNotCurrentDirectory;

   using  _oneExtraArgMemberForEacher_DeleteDirectoryType =
      TwoArgMemberFunctionForEacher<DeleteDirectorySubProgram, string, const FileRevisorArgs&>;
   unique_ptr<const _oneExtraArgMemberForEacher_DeleteDirectoryType> _oneExtraArgMemberForEacher_DeleteDirectory;

   using _parallelTwoArgMemberFunctionForEacher_DeleteDirectoryType =
      ParallelTwoArgMemberFunctionForEacher<DeleteDirectorySubProgram, string, const FileRevisorArgs&>;
   unique_ptr<const _parallelTwoArgMemberFunctionForEacher_DeleteDirectoryType> _parallelTwoArgMemberFunctionForEacher_DeleteDirectory;

   using _tryCatchCaller_DeleteDirectoryType =
      VoidTwoArgTryCatchCaller<DeleteDirectorySubProgram, const string&, const FileRevisorArgs&>;
   unique_ptr<const _tryCatchCaller_DeleteDirectoryType> _tryCatchCaller_DeleteDirectory;
public:
   DeleteDirectorySubProgram();
   ~DeleteDirectorySubProgram() override;
   int Run(const FileRevisorArgs& args) const override;
private:
   void DeleteDirectory(const string& directoryPath, const FileRevisorArgs& args) const;
   void DeleteTargetDirectoryIfNotCurrentDirectory(const FileRevisorArgs& args) const;
   void TryCatchCallDeleteDirectory(const string& directoryPath, const FileRevisorArgs& args) const;
   void ParallelExceptionHandler(string_view exceptionClassNameAndMessage) const;
};
