#pragma once
class Console;
class FileSystem;

class FileRevisorPreambleMaker
{
   friend class FileRevisorPreambleMakerTests;
private:
   // Constant Components
   unique_ptr<const Console> p_console;
   unique_ptr<const FileSystem> p_fileSystem;
public:
   FileRevisorPreambleMaker();
   virtual ~FileRevisorPreambleMaker();
   virtual void PrintPreambleLines(const FileRevisorArgs& args) const;
};
