#pragma once
class Console;
class FileSystem;
class Pluralizer;

class FileRevisorSubProgram
{
   friend class FileRevisorSubProgramTests;
protected:
   // Base Class Constant Components
   unique_ptr<const Console> _protected_console;
   unique_ptr<const FileSystem> _protected_fileSystem;
   unique_ptr<const Pluralizer> _protected_pluralizer;
public:
   FileRevisorSubProgram();
   virtual ~FileRevisorSubProgram();
   virtual int Run(const FileRevisorArgs& args) const;
};
