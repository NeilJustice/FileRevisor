#pragma once
class Console;
class FileSystem;
class Pluralizer;

class FileRevisorSubProgram
{
   friend class FileRevisorSubProgramTests;
protected:
   // Base Class Constant Components
   unique_ptr<const Console> p_console;
   unique_ptr<const FileSystem> p_fileSystem;
   unique_ptr<const Pluralizer> p_pluralizer;
public:
   FileRevisorSubProgram();
   virtual ~FileRevisorSubProgram();
   virtual int Run(const FileRevisorArgs& args) const;
};
