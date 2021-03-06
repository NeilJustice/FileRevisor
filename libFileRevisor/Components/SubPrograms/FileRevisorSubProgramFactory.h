#pragma once
class FileRevisorSubProgram;

class FileRevisorSubProgramFactory
{
public:
   virtual shared_ptr<FileRevisorSubProgram> NewFileRevisorSubProgram(ProgramMode programMode) const;
   virtual ~FileRevisorSubProgramFactory() = default;
};
