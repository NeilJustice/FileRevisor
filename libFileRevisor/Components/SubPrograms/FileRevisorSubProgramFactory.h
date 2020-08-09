#pragma once
class FileRevisorSubProgram;
enum class ProgramMode;

class FileRevisorSubProgramFactory
{
public:
   virtual shared_ptr<FileRevisorSubProgram> NewFileRevisorSubProgram(ProgramMode programMode) const;
   virtual ~FileRevisorSubProgramFactory() = default;
};
