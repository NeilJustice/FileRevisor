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
   // Mutable Fields
   FileRevisorArgs p_args;
public:
   FileRevisorSubProgram();
   virtual ~FileRevisorSubProgram();
   virtual void Initialize(const FileRevisorArgs& args);
   // Actions
   virtual int Run() const;
};
