#pragma once
template<typename ReturnType, typename ClassType, typename ArgumentType>
class NonVoidOneArgMemberFunctionCaller;

template<typename ReturnType, typename ClassType, typename Argument1Type, typename Argument2Type>
class NonVoidTwoArgMemberFunctionCaller;

namespace Utils
{
   template<typename ClassType, typename T, typename Arg2Type, typename Arg3Type, typename Arg4Type>
   class FourArgMemberFunctionForEacher;
}

template<typename ClassType, typename Arg1Type>
class VoidOneArgMemberFunctionCaller;

template<typename ClassType, typename Arg1Type, typename Arg2Type>
class VoidTwoArgMemberFunctionCaller;

class Console;
class ConstCharPointerGetter;
class FileOpenerCloser;
class FileSystemExceptionMaker;
class RecursiveFileDeleter;

class FileSystem
{
   friend class FileSystemTests;
private:
   // Function Pointers
   function<int(FILE*)> _call_fclose;
   function<FILE* (const char*, const char*)> _call_fopen;
   function<bool(const fs::path&)> _call_fs_is_directory;
   function<bool(const fs::path&)> _call_fs_remove;
   function<uintmax_t(const fs::path&)> _call_fs_remove_all;
   function<int(const char*, const char*)> _call_std_rename;

#ifdef _WIN32
   fs::path(*_call_fs_absolute_as_assignable_function_pointer)(const fs::path&);
   function<fs::path(const fs::path&)> _call_fs_absolute;
#endif

   fs::path(*_call_fs_current_path_as_assignable_function_pointer)();
   function<fs::path()> _call_fs_current_path;

   bool(*_call_fs_exists_as_assignable_function_pointer)(const fs::path&);
   function<bool(const fs::path&)> _call_fs_exists;

   void(*_call_fs_rename_with_error_code_as_assignable_function_pointer)(const fs::path&, const fs::path&, std::error_code&);
   function<void(const fs::path&, const fs::path&, std::error_code&)> _call_fs_rename_with_error_code;

   // Function Callers
   using _caller_DeleteFileOrDirectoryType = VoidTwoArgMemberFunctionCaller<FileSystem, const fs::path&, bool>;
   unique_ptr<const _caller_DeleteFileOrDirectoryType> _caller_DeleteFileOrDirectory;

   using _caller_DoDeleteFileOrDirectoryType = VoidOneArgMemberFunctionCaller<FileSystem, const fs::path&>;
   unique_ptr<const _caller_DoDeleteFileOrDirectoryType> _caller_DoDeleteFileOrDirectory;

   using _caller_FileSystem_ExistsType = NonVoidOneArgMemberFunctionCaller<bool, FileSystem, const fs::path&>;
   unique_ptr<const _caller_FileSystem_ExistsType> _caller_FileSystem_Exists;

   using _caller_GetFileOrFolderPathsInDirectoryType = NonVoidTwoArgMemberFunctionCaller<vector<fs::path>, FileSystem, const fs::path&, bool>;
   unique_ptr<const _caller_GetFileOrFolderPathsInDirectoryType> _caller_GetFileOrFolderPathsInDirectory;

   using _foreacher_DeleteFileOrDirectoryType = Utils::FourArgMemberFunctionForEacher<FileSystem, fs::path, bool, bool, bool>;
   unique_ptr<const _foreacher_DeleteFileOrDirectoryType> _foreacher_DeleteFileOrDirectory;
   // Constant Components
   unique_ptr<const Console> _console;
   unique_ptr<const ConstCharPointerGetter> _constCharPointerGetter;
   unique_ptr<const FileOpenerCloser> _fileOpenerCloser;
   unique_ptr<const FileSystemExceptionMaker> _fileSystemExceptionMaker;
   unique_ptr<const RecursiveFileDeleter> _recursiveFileDeleter;
public:
   FileSystem();
   virtual ~FileSystem();

   // Deletes
   virtual void DeleteTopLevelFilesAndEmptyDirectoriesInDirectory(
      const fs::path& directoryPath, bool skipFilesInUse, bool dryRun, bool quietMode) const;
   virtual void RecursivelyDeleteAllFilesInDirectory(const string& directoryPath, const FileRevisorArgs& args) const;
   virtual void RemoveFile(const char* filePath, bool ignoreFileDeleteError) const;
   virtual void DeleteFileOrDirectory(const fs::path& fileOrFolderPath, bool ignoreFileDeleteError, bool dryRun, bool quietMode) const;

   // Open File
   virtual shared_ptr<FILE> OpenFile(const fs::path& filePath, const char* fileOpenMode) const;

   // Queries
   virtual fs::path GetAbsolutePath(const fs::path& fileOrFolderPath) const;
   virtual fs::path CurrentFolderPath() const;
   virtual bool FileOrDirectoryExists(const fs::path& fileOrFolderPath) const;
   virtual vector<fs::path> GetFolderPathsInDirectory(const fs::path& directoryPath, bool recurse) const;
   virtual vector<fs::path> GetFilePathsInDirectory(const fs::path& directoryPath, bool recurse) const;
   virtual vector<string> GetStringFolderPathsInDirectory(const fs::path& directoryPath, bool recurse) const;

   // Reads
   virtual std::string ReadText(const fs::path& textFilePath) const;

   // Readonly Flags
   virtual void RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows(const fs::path& directoryPath, bool dryRun) const;

   // Writes
   virtual void CreateDirectories(const fs::path& directoryPath) const;
   virtual void CreateTextFile(const fs::path& filePath, string_view fileText) const;
   virtual void CreateFileWithBytes(const fs::path& filePath, const char* bytes, size_t bytesLength) const;
   virtual fs::path RenameFile(const fs::path& filePath, string_view newFileName) const;
   virtual fs::path RenameDirectory(const fs::path& directoryPath, string_view newDirectoryName) const;
private:
   virtual void DoDeleteFileOrDirectory(const fs::path& fileOrFolderPath) const;
   virtual size_t GetFileSize(std::ifstream& file) const;
   virtual void EraseTrailingBinaryZeros(std::string& outStr) const;
};
