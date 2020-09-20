#pragma once
template<typename ReturnType, typename ClassType, typename Arg1Type, typename Arg2Type>
class NonVoidTwoArgMemberFunctionCaller;

class DirectoryIterator
{
   friend class DirectoryIteratorTests;
private:
   fs::directory_iterator _directoryIterator;
   fs::recursive_directory_iterator _recursiveDirectoryIterator;
   vector<string> _fileAndDirectoryPathIgnoreSubstrings;
   bool _recursiveMode;
public:
   DirectoryIterator() noexcept;
   virtual ~DirectoryIterator() = default;
   virtual void SetFileAndDirectoryPathIgnoreSubstrings(const vector<string>& fileAndDirectoryPathIgnoreSubstrings);
   virtual void SetDirectoryIterator(const fs::path& directoryPath, bool recurse);
   virtual fs::path NextNonIgnoredFilePath();
   virtual fs::path NextNonIgnoredDirectoryPath();
private:
   bool PathContainsAnySubstringCaseInsensitive(const fs::path& path, const vector<string>& pathSubstrings);

   template<typename DirectoryIteratorType>
   fs::path NextNonIgnoredPath(DirectoryIteratorType& iter, fs::file_type requiredFileType);
};
