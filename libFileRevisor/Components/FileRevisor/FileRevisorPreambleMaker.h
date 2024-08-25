#pragma once
class Console;
class FileSystem;

class FileRevisorPreambleMaker
{
   friend class FileRevisorPreambleMakerTests;
private:
   // Constant Components
   unique_ptr<const Console> _console;
   unique_ptr<const FileSystem> _fileSystem;
public:
   FileRevisorPreambleMaker();
   virtual ~FileRevisorPreambleMaker();
   virtual void PrintPreambleLines(const FileRevisorArgs& args) const;
};
