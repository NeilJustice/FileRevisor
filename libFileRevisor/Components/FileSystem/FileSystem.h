#pragma once

class FileSystemTests;

template<typename ReturnType, typename ClassType, typename ArgumentType>
class NonVoidOneArgMemberFunctionCaller;

template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgMemberFunctionCaller;

class ConstCharPointerGetter;
class DirectoryIterator;
class FileOpenerCloser;
class FileSystemExceptionMaker;
struct FileRevisorArgs;
class RecursiveFileDeleter;

class FileSystem
{
   friend class ::FileSystemTests;
private:
   // Function Pointers
   function<FILE* (const char*, const char*)> _call_fopen;
   function<int(FILE*)> _call_fclose;
   function<uintmax_t(const fs::path&, error_code&)> _call_fs_remove_all;
   function<int(const char*, const char*)> _call_std_rename;

#ifdef _WIN32
   fs::path(*_call_std_filesystem_absolute_as_assignable_function_pointer)(const fs::path&);
   function<fs::path(const fs::path&)> _call_std_filesystem_absolute;
#endif

   fs::path(*_call_std_filesystem_current_path_as_assignable_function_pointer)();
   function<fs::path()> _call_std_filesystem_current_path;

   bool(*_call_std_filesystem_exists_as_assignable_function_pointer)(const fs::path&);
   function<bool(const fs::path&)> _call_std_filesystem_exists;

   void(*_call_std_filesystem_rename_with_error_code_as_assignable_function_pointer)(const fs::path&, const fs::path&, std::error_code&);
   function<void(const fs::path&, const fs::path&, std::error_code&)> _call_std_filesystem_rename_with_error_code;

   // Function Callers
   unique_ptr<const NonVoidOneArgMemberFunctionCaller<bool, FileSystem, const fs::path&>> _caller_Exists;

   // Constant Components
   unique_ptr<const ConstCharPointerGetter> _constCharPointerGetter;
   unique_ptr<const FileOpenerCloser> _fileOpenerCloser;
   unique_ptr<const FileSystemExceptionMaker> _fileSystemExceptionMaker;
   unique_ptr<const RecursiveFileDeleter> _recursiveFileDeleter;
public:
   FileSystem();
   virtual ~FileSystem();

   // Queries
   virtual fs::path GetAbsolutePath(const fs::path& fileOrDirectoryPath) const;
   virtual fs::path CurrentDirectoryPath() const;
   virtual void RecursivelyDeleteAllFilesInDirectory(
      const string& directoryPath, const FileRevisorArgs& args) const;
   virtual vector<fs::path> GetFilePathsInDirectory(const fs::path& directoryPath, bool recurse) const;
   virtual vector<fs::path> GetDirectoryPathsInDirectory(const fs::path& directoryPath, bool recurse) const;
   virtual vector<string> GetStringDirectoryPathsInDirectory(const fs::path& directoryPath, bool recurse) const;
   virtual bool FileOrDirectoryExists(const fs::path& fileOrDirectoryPath) const;

   // Reads
   virtual std::string ReadText(const fs::path& textFilePath) const;

   // Writes
   virtual void CreateDirectories(const fs::path& directoryPath) const;
   virtual void CreateTextFile(const fs::path& filePath, string_view fileText) const;
   virtual void CreateBinaryFile(const fs::path& filePath, const char* bytes, size_t bytesLength) const;
   virtual fs::path RenameFile(const fs::path& filePath, string_view newFileName) const;
   virtual fs::path RenameDirectory(const fs::path& directoryPath, string_view newDirectoryName) const;

   // Deletes
   virtual void RemoveFile(const char* filePath) const;
   virtual unsigned long long RemoveAll(const fs::path& directoryPath) const;

   // Readonly Flags
   virtual void RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows(const fs::path& directoryPath) const;

   // Open And Close Files
   virtual FILE* OpenFile(const fs::path& filePath, const char* fileOpenMode) const;
private:
   virtual size_t GetFileSize(std::ifstream& file) const;
   virtual void CreateBinaryOrTextFile(
      const fs::path& filePath, bool trueBinaryFalseText, const char* bytes, size_t bytesLength) const;
   virtual void EraseTrailingBinaryZeros(std::string& outStr) const;
};
