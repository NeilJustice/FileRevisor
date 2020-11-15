#pragma once
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

template<typename T, typename ClassType, typename MemberFunctionType, typename ExtraArgType>
class OneExtraArgMemberForEacher;

template<typename T, typename ClassType, typename MemberFunctionType, typename ExtraArgType>
class ParallelOneExtraArgMemberForEacher;

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
public:
   DeleteDirectorySubProgram();
   virtual ~DeleteDirectorySubProgram();
   int Run(const FileRevisorArgs& args) const override;
private:
   void DeleteDirectory(const string& directoryPath, const FileRevisorArgs& args) const;
};
