#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

template<typename T, typename ClassType, typename MemberFunctionType, typename ExtraArgType>
class OneExtraArgMemberForEacher;

template<typename T, typename ClassType, typename MemberFunctionType, typename ExtraArgType>
class ParallelOneExtraArgMemberForEacher;

template<typename ClassType, typename Argument1Type, typename Argument2Type>
class VoidTwoArgTryCatchCaller;

class DeleteDirectorySubProgram : public FileRevisorSubProgram
{
   friend class DeleteDirectorySubProgramTests;
private:
   // Function Callers
   using OneExtraArgMemberForEacherType = OneExtraArgMemberForEacher<
      string,
      DeleteDirectorySubProgram,
      void(DeleteDirectorySubProgram::*)(const string&, const FileRevisorArgs&) const,
      const FileRevisorArgs&>;
   unique_ptr<const OneExtraArgMemberForEacherType> _oneExtraArgMemberForEacher_DeleteDirectory;

   using ParallelOneExtraArgMemberForEacherType = ParallelOneExtraArgMemberForEacher<
      string,
      DeleteDirectorySubProgram,
      void(DeleteDirectorySubProgram::*)(const string&, const FileRevisorArgs&) const,
      const FileRevisorArgs&>;
   unique_ptr<const ParallelOneExtraArgMemberForEacherType> _parallelOneExtraArgMemberForEacher_DeleteDirectory;

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
