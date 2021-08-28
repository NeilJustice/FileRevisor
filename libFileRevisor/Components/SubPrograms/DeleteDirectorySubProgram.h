#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

template<typename ClassType, typename T, typename Arg2Type>
class TwoArgMemberForEacher;

template<typename ClassType, typename T, typename Arg2Type>
class ParallelTwoArgMemberForEacher;

template<typename ClassType, typename Argument1Type, typename Argument2Type>
class VoidTwoArgTryCatchCaller;

class DeleteDirectorySubProgram : public FileRevisorSubProgram
{
   friend class DeleteDirectorySubProgramTests;
private:
   // Function Callers
   using TwoArgMemberForEacherType = TwoArgMemberForEacher<DeleteDirectorySubProgram, string, const FileRevisorArgs&>;
   unique_ptr<const TwoArgMemberForEacherType> _oneExtraArgMemberForEacher_DeleteDirectory;

   using ParallelTwoArgMemberForEacherType = ParallelTwoArgMemberForEacher<DeleteDirectorySubProgram, string, const FileRevisorArgs&>;
   unique_ptr<const ParallelTwoArgMemberForEacherType> _parallelTwoArgMemberForEacher_DeleteDirectory;

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
