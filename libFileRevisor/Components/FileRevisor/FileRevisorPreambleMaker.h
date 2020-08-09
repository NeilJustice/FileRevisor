#pragma once
class FileSystem;
struct FileRevisorArgs;

class FileRevisorPreambleMaker
{
   friend class FileRevisorPreambleMakerTests;
private:
   unique_ptr<const FileSystem> _fileSystem;
public:
   FileRevisorPreambleMaker();
   virtual ~FileRevisorPreambleMaker() = default;
   virtual string MakePreambleLines(const FileRevisorArgs& args) const;
};
