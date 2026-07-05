#pragma once
class FileSystem;

class DirectoryDeleter
{
   friend class DirectoryDeleterTests;
private:
   // Constant Components
   unique_ptr<const FileSystem> _fileSystem;
public:
   DirectoryDeleter();
   virtual ~DirectoryDeleter();

   virtual void DeleteTargetDirectoryIfNotContentsOnlyAndNotCurrentDirectory(const FileRevisorArgs& args) const;
};
