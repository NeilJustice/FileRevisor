#pragma once
class FileOpenerCloser;
template<typename ReturnType, typename ClassType, typename Arg1Type, typename Arg2Type>
class NonVoidTwoArgMemberFunctionCaller;

class DirectoryIterator
{
   friend class DirectoryIteratorTests;
private:
   // Function Pointers
#if defined __linux__
   function<size_t(void*, size_t, size_t, FILE*)> _call_fread;
#elif defined _WIN32
   function<size_t(void*, size_t, size_t, size_t, FILE*)> _call_fread_nolock_s;
#endif
   // Constant Components
   unique_ptr<const Console> _console;
   unique_ptr<const FileOpenerCloser> _fileOpenerCloser;
   // Mutable Fields
   fs::directory_iterator _directoryIterator;
   fs::recursive_directory_iterator _recursiveDirectoryIterator;
   vector<string> _fileAndDirectoryPathIgnoreSubstrings;
   bool _recursiveMode;
public:
   DirectoryIterator() noexcept;
   virtual ~DirectoryIterator() = default;
   virtual vector<fs::path> GetNonEmptyNonIgnoredTextFilePaths();
   virtual fs::path NextNonIgnoredDirectoryPath();
   virtual fs::path NextNonIgnoredFilePath();
   virtual void SetDirectoryIterator(const fs::path& directoryPath, bool recurse);
   virtual void SetFileAndDirectoryPathIgnoreSubstrings(const vector<string>& fileAndDirectoryPathIgnoreSubstrings);
private:
   virtual bool IsFileEmptyOrBinaryOrNotAnsiOrNotOpenable(const fs::path& filePath) const;
   template<typename DirectoryIteratorType>
   fs::path NextNonIgnoredPath(DirectoryIteratorType& iter, fs::file_type requiredFileType);
   static bool PathContainsAnySubstringCaseInsensitive(const fs::path& path, const vector<string>& pathSubstrings);
};
