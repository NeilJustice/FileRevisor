#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

template<typename ClassType, typename T>
class OneArgMemberFunctionForEacher;

template<typename ClassType, typename T, typename Arg2Type>
class TwoArgMemberFunctionForEacher;

template<typename ClassType, typename T>
class ParallelOneArgMemberFunctionForEacher;

template<typename ClassType>
class VoidZeroArgMemberFunctionCaller;

template<typename ClassType, typename Arg1Type>
class VoidOneArgMemberFunctionCaller;

template<typename ClassType, typename ArgType>
class VoidOneArgTryCatchCaller;

class DeleteDirectorySubProgram : public FileRevisorSubProgram
{
   friend class DeleteDirectorySubProgramTests;
private:
   // Function Callers
   using _caller_DeleteTargetDirectoryIfNotCurrentDirectoryType = VoidZeroArgMemberFunctionCaller<DeleteDirectorySubProgram>;
   unique_ptr<const _caller_DeleteTargetDirectoryIfNotCurrentDirectoryType> _caller_DeleteTargetDirectoryIfNotCurrentDirectory;

   using  _oneExtraArgMemberForEacher_DeleteDirectoryType = OneArgMemberFunctionForEacher<DeleteDirectorySubProgram, string>;
   unique_ptr<const _oneExtraArgMemberForEacher_DeleteDirectoryType> _oneExtraArgMemberForEacher_DeleteDirectory;

   using _parallelTwoArgMemberFunctionForEacher_DeleteDirectoryType = ParallelOneArgMemberFunctionForEacher<DeleteDirectorySubProgram, string>;
   unique_ptr<const _parallelTwoArgMemberFunctionForEacher_DeleteDirectoryType> _parallelTwoArgMemberFunctionForEacher_DeleteDirectory;

   using _tryCatchCaller_DeleteDirectoryType = VoidOneArgTryCatchCaller<DeleteDirectorySubProgram, const string&>;
   unique_ptr<const _tryCatchCaller_DeleteDirectoryType> _tryCatchCaller_DeleteDirectory;
public:
   DeleteDirectorySubProgram();
   ~DeleteDirectorySubProgram() override;
   // Actions
   int Run() const override;
private:
   void DeleteDirectory(const string& directoryPath) const;
   void DeleteTargetDirectoryIfNotCurrentDirectory() const;
   void TryCatchCallDeleteDirectory(const string& directoryPath) const;
   void ParallelExceptionHandler(string_view exceptionClassNameAndMessage) const;
};
