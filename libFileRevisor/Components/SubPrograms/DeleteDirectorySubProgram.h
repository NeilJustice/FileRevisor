#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

template<typename ClassType, typename T, typename Arg2Type>
class TwoArgMemberFunctionForEacher;

template<typename ClassType, typename T, typename Arg2Type>
class ParallelTwoArgMemberFunctionForEacher;

template<typename ClassType, typename Argument1Type, typename Argument2Type>
class VoidTwoArgTryCatchCaller;

class DeleteDirectorySubProgram : public FileRevisorSubProgram
{
   friend class DeleteDirectorySubProgramTests;
private:
   // Function Callers
   using TwoArgMemberFunctionForEacherType = TwoArgMemberFunctionForEacher<DeleteDirectorySubProgram, string, const FileRevisorArgs&>;
   unique_ptr<const TwoArgMemberFunctionForEacherType> _oneExtraArgMemberForEacher_DeleteDirectory;

   using ParallelTwoArgMemberFunctionForEacherType = ParallelTwoArgMemberFunctionForEacher<DeleteDirectorySubProgram, string, const FileRevisorArgs&>;
   unique_ptr<const ParallelTwoArgMemberFunctionForEacherType> _parallelTwoArgMemberFunctionForEacher_DeleteDirectory;

   using _voidTwoArgTryCatchCallerType = VoidTwoArgTryCatchCaller<DeleteDirectorySubProgram, const string&, const FileRevisorArgs&>;
   unique_ptr<const _voidTwoArgTryCatchCallerType> _voidTwoArgTryCatchCaller;
public:
   DeleteDirectorySubProgram();
   virtual ~DeleteDirectorySubProgram();
   int Run(const FileRevisorArgs& args) const override;
private:
   void TryCatchCallDeleteDirectory(const string& directoryPath, const FileRevisorArgs& args) const;
   void DeleteDirectory(const string& directoryPath, const FileRevisorArgs& args) const;
   void ParallelExceptionHandler(string_view exceptionClassNameAndMessage) const;
};
