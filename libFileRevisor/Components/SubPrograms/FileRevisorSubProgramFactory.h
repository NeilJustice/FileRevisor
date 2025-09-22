#pragma once
class FileRevisorSubProgram;

class FileRevisorSubProgramFactory
{
public:
   virtual shared_ptr<FileRevisorSubProgram> NewSubProgram(ProgramMode programMode) const;
   virtual ~FileRevisorSubProgramFactory() = default;
};
