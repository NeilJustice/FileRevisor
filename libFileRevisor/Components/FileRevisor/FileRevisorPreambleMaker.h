#pragma once
class FileSystem;

class FileRevisorPreambleMaker
{
   friend class FileRevisorPreambleMakerTests;
private:
   // Constant Components
   unique_ptr<const FileSystem> _fileSystem;
public:
   FileRevisorPreambleMaker();
   virtual ~FileRevisorPreambleMaker() = default;
   virtual string MakePreambleLines(const FileRevisorArgs& args) const;
};
