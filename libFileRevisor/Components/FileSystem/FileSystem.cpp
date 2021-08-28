#include "pch.h"
#include "libFileRevisor/Components/Exceptions/FileSystemExceptionMaker.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"
#include "libFileRevisor/Components/FileSystem/FCloseDeleter.h"
#include "libFileRevisor/Components/FileSystem/FileReader.h"
#include "libFileRevisor/Components/FileSystem/FileOpenerCloser.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/FileSystem/RecursiveFileDeleter.h"
#include "libFileRevisor/Exceptions/FileSystemException.h"
#include "libFileRevisor/UtilityComponents/DataStructures/CharArray64Helper.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/Member/NonVoidOneArgMemberFunctionCaller.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/Member/NonVoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/Member/VoidOneArgMemberFunctionCaller.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/UtilityComponents/Iteration/ForEach/ThreeArgMemberForEacher.h"
#include "libFileRevisor/UtilityComponents/Strings/ConstCharPointerGetter.h"

FileSystem::FileSystem()
   // Function Pointers
   : _call_fclose(::fclose)
   , _call_fopen(::fopen)
   , _call_fs_remove(static_cast<bool(*)(const fs::path&)>(fs::remove))
   , _call_fs_remove_all(static_cast<uintmax_t(*)(const fs::path&, error_code&)>(fs::remove_all))
   , _call_std_rename(std::rename)
#ifdef _WIN32
   , _call_fs_absolute_as_assignable_function_pointer(fs::absolute)
#endif
   , _call_fs_current_path_as_assignable_function_pointer(fs::current_path)
   , _call_fs_exists_as_assignable_function_pointer(fs::exists)
   , _call_fs_rename_with_error_code_as_assignable_function_pointer(fs::rename)
   // Function Callers
   , _caller_Exists(make_unique<NonVoidOneArgMemberFunctionCaller<bool, FileSystem, const fs::path&>>())
   , _caller_GetFileOrDirectoryPathsInDirectory(make_unique<_caller_GetFileOrDirectoryPathsInDirectoryType>())
   , _caller_RemoveFileSystemFileOrDirectory(make_unique<VoidTwoArgMemberFunctionCaller<FileSystem, const fs::path&, bool>>())
   , _foreacher_DeleteFileOrDirectory(make_unique<_foreacher_DeleteFileOrDirectoryType>())
   // Constant Components
   , _console(make_unique<Console>())
   , _constCharPointerGetter(make_unique<ConstCharPointerGetter>())
   , _fileOpenerCloser(make_unique<FileOpenerCloser>())
   , _fileSystemExceptionMaker(make_unique<FileSystemExceptionMaker>())
   , _recursiveFileDeleter(make_unique<RecursiveFileDeleter>())
{
#ifdef _WIN32
   _call_fs_absolute = _call_fs_absolute_as_assignable_function_pointer;
#endif
   _call_fs_current_path = _call_fs_current_path_as_assignable_function_pointer;
   _call_fs_exists = _call_fs_exists_as_assignable_function_pointer;
   _call_fs_rename_with_error_code = _call_fs_rename_with_error_code_as_assignable_function_pointer;
}

FileSystem::~FileSystem()
{
}

// Queries

fs::path FileSystem::GetAbsolutePath(const fs::path& fileOrDirectoryPath) const
{
#if defined __linux__|| defined __APPLE__
   fs::path absoluteFileOrDirectoryPath = fs::absolute(fileOrDirectoryPath);
   const fs::path absoluteFileOrDirectoryPathFileName = absoluteFileOrDirectoryPath.filename();
   if (absoluteFileOrDirectoryPathFileName == ".")
   {
      // On Linux, "/dir/subdir/." becomes "/dir/subdir"
      absoluteFileOrDirectoryPath = absoluteFileOrDirectoryPath.parent_path();
   }
   return absoluteFileOrDirectoryPath;
#elif _WIN32
   fs::path absoluteFileOrDirectoryPath = _call_fs_absolute(fileOrDirectoryPath);
   return absoluteFileOrDirectoryPath;
#endif
}

fs::path FileSystem::CurrentDirectoryPath() const
{
   fs::path currentDirectoryPath = _call_fs_current_path();
   return currentDirectoryPath;
}

bool FileSystem::FileOrDirectoryExists(const fs::path& fileOrDirectoryPath) const
{
   const bool fileOrDirectoryPathExists = _call_fs_exists(fileOrDirectoryPath);
   return fileOrDirectoryPathExists;
}

vector<fs::path> FileSystem::GetDirectoryPathsInDirectory(const fs::path& directoryPath, bool recurse) const
{
   vector<fs::path> directoryPaths;
   DirectoryIterator directoryIterator;
   directoryIterator.SetDirectoryIterator(directoryPath, recurse);
   while (true)
   {
      fs::path nonIgnoredDirectoryPath = directoryIterator.NextNonIgnoredDirectoryPath();
      static const fs::path endIterationMarker{};
      if (nonIgnoredDirectoryPath == endIterationMarker)
      {
         break;
      }
      directoryPaths.emplace_back(std::move(nonIgnoredDirectoryPath));
   }
   return directoryPaths;
}

vector<fs::path> FileSystem::GetFilePathsInDirectory(const fs::path& directoryPath, bool recurse) const
{
   vector<fs::path> filePaths;
   DirectoryIterator directoryIterator;
   directoryIterator.SetDirectoryIterator(directoryPath, recurse);
   static const fs::path endIterationMarker{};
   while (true)
   {
      fs::path nextNonIgnoredFilePath = directoryIterator.NextNonIgnoredFilePath();
      if (nextNonIgnoredFilePath == endIterationMarker)
      {
         break;
      }
      filePaths.emplace_back(std::move(nextNonIgnoredFilePath));
   }
   return filePaths;
}

vector<string> FileSystem::GetStringDirectoryPathsInDirectory(const fs::path& directoryPath, bool recurse) const
{
   const vector<fs::path> subdirectoryPaths = GetDirectoryPathsInDirectory(directoryPath, recurse);
   const size_t numberOfSubdirectoryPaths = subdirectoryPaths.size();
   vector<string> stringSubdirectoryPaths(numberOfSubdirectoryPaths);
   for (size_t i = 0; i < numberOfSubdirectoryPaths; ++i)
   {
      const fs::path& subdirectoryPath = subdirectoryPaths[i];
      stringSubdirectoryPaths[i] = subdirectoryPath.string();
   }
   return stringSubdirectoryPaths;
}

void FileSystem::RecursivelyDeleteAllFilesInDirectory(const string& directoryPath, const FileRevisorArgs& args) const
{
   _recursiveFileDeleter->RecursivelyDeleteAllFilesInDirectory(directoryPath.c_str(), args);
}

// Reads

string FileSystem::ReadText(const fs::path& textFilePath) const
{
   ifstream textFileStream(textFilePath.c_str());
   if (!textFileStream.is_open())
   {
      const FileSystemException fileSystemException = _fileSystemExceptionMaker->MakeFileSystemExceptionForFailedToOpenFileWithFStream(textFilePath);
      throw fileSystemException;
   }
   const size_t fileSize = GetFileSize(textFileStream);
   string fileText(fileSize, 0);
   textFileStream.read(const_cast<char*>(fileText.c_str()), static_cast<streamsize>(fileSize));
   EraseTrailingBinaryZeros(fileText);
   return fileText;
}

// Writes

void FileSystem::CreateDirectories(const fs::path& directoryPath) const
{
   fs::create_directories(directoryPath);
}

void FileSystem::CreateTextFile(const fs::path& filePath, string_view fileText) const
{
   CreateBinaryOrTextFile(filePath, false, fileText.data(), fileText.size());
}

void FileSystem::CreateBinaryFile(const fs::path& filePath, const char* bytes, size_t bytesLength) const
{
   CreateBinaryOrTextFile(filePath, true, bytes, bytesLength);
}

fs::path FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) const
{
   const bool filePathExists = _caller_Exists->CallConstMemberFunction(this, &FileSystem::FileOrDirectoryExists, filePath);
   if (!filePathExists)
   {
      const string exceptionMessage = String::ConcatStrings(
         "FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) error: filePath does not exist: ", filePath.string());
      throw runtime_error(exceptionMessage);
   }
   const fs::path sourceDirectoryPath = filePath.parent_path();
   fs::path renamedFilePath = sourceDirectoryPath / newFileName;
   const bool destinationFilePathExists = _caller_Exists->CallConstMemberFunction(this, &FileSystem::FileOrDirectoryExists, renamedFilePath);
   if (destinationFilePathExists)
   {
      const string exceptionMessage = String::ConcatStrings(
         "FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) error: renamedFilePath already exists: ", renamedFilePath.string());
      throw runtime_error(exceptionMessage);
   }
   const string filePathString = filePath.string();
   const char* const filePathStringCCP = _constCharPointerGetter->GetStringConstCharPointer(filePathString);
   const string renamedFilePathString = renamedFilePath.string();
   const char* const renamedFilePathStringCCP = _constCharPointerGetter->GetStringConstCharPointer(renamedFilePathString);
   const int renameReturnValue = _call_std_rename(filePathStringCCP, renamedFilePathStringCCP);
   if (renameReturnValue != 0)
   {
      const string exceptionMessage = String::ConcatValues(
         "FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) error: std::rename(\"",
         filePathString, "\", \"", renamedFilePathString, "\") returned a non-0 value: ", renameReturnValue);
      throw runtime_error(exceptionMessage);
   }
   return renamedFilePath;
}

fs::path FileSystem::RenameDirectory(const fs::path& directoryPath, string_view newDirectoryName) const
{
   const fs::path directoryPathMinusLeafDirectory = [&]()
   {
      return fs::path(directoryPath).remove_filename();
   }();
   fs::path renamedDirectoryPath = directoryPathMinusLeafDirectory / newDirectoryName;
   error_code renameErrorCode;
   _call_fs_rename_with_error_code(directoryPath, renamedDirectoryPath, renameErrorCode);
   const int renameErrorCodeValue = renameErrorCode.value();
   if (renameErrorCodeValue != 0)
   {
      const FileSystemException fileSystemException =
         _fileSystemExceptionMaker->MakeFileSystemExceptionForFailedToRenameDirectory(directoryPath, renamedDirectoryPath, renameErrorCode);
      throw fileSystemException;
   }
   return renamedDirectoryPath;
}

// Deletes

void FileSystem::DeleteTopLevelFilesAndEmptyDirectoriesInDirectory(const fs::path& directoryPath, bool skipFilesInUse, bool dryRun) const
{
   _caller_RemoveFileSystemFileOrDirectory->CallConstMemberFunction(
      this, &FileSystem::RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows, directoryPath, dryRun);

   const vector<fs::path> topLevelDirectoryPaths = _caller_GetFileOrDirectoryPathsInDirectory->CallConstMemberFunction(
      this, &FileSystem::GetDirectoryPathsInDirectory, directoryPath, false);
   const vector<fs::path> topLevelFilePaths = _caller_GetFileOrDirectoryPathsInDirectory->CallConstMemberFunction(
      this, &FileSystem::GetFilePathsInDirectory, directoryPath, false);

   _foreacher_DeleteFileOrDirectory->CallConstMemberFunctionWithEachElement(
      topLevelDirectoryPaths, this, &FileSystem::RemoveFileSystemFileOrDirectory, skipFilesInUse, dryRun);
   _foreacher_DeleteFileOrDirectory->CallConstMemberFunctionWithEachElement(
      topLevelFilePaths, this, &FileSystem::RemoveFileSystemFileOrDirectory, skipFilesInUse, dryRun);
}

void FileSystem::RemoveFile(const char* filePath, bool ignoreFileDeleteError) const
{
#if defined __linux__ || defined __APPLE__
   const int unlinkReturnValue = unlink(filePath);
#elif _WIN32
   const int unlinkReturnValue = _unlink(filePath);
#endif
   if (unlinkReturnValue != 0)
   {
      if (!ignoreFileDeleteError)
      {
         const FileSystemException fileSystemException = _fileSystemExceptionMaker->MakeFileSystemExceptionForFailedToDeleteFile(filePath);
         throw fileSystemException;
      }
   }
}

void FileSystem::RemoveFileSystemFileOrDirectory(const fs::path& filePath, bool ignoreFileDeleteError, bool dryRun) const
{
   if (dryRun)
   {
      const string wouldDeleteMessage = "DryRun: Would delete " + filePath.string();
      _console->ThreadIdWriteLine(wouldDeleteMessage);
   }
   else
   {
      try
      {
         _call_fs_remove(filePath);
         const string deletedFileMessage = "Deleted " + filePath.string();
         _console->ThreadIdWriteLine(deletedFileMessage);
      }
      catch (const exception& ex)
      {
         if (ignoreFileDeleteError)
         {
            const string exceptionClassNameAndMessage = Type::GetExceptionClassNameAndMessage(&ex);
            const string message = "[FileRevisor] Ignoring exception because --skip-files-in-use: " + exceptionClassNameAndMessage;
            _console->ThreadIdWriteLineColor(message, Color::Yellow);
         }
         else
         {
            throw;
         }
      }
   }
}

// Readonly Flags

void FileSystem::RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows([[maybe_unused]]const fs::path& directoryPath, bool dryRun) const
{
#ifdef _WIN32
   if (!dryRun)
   {
      const vector<fs::path> topLevelFilePathsInDirectory = GetFilePathsInDirectory(directoryPath, false);
      for (const fs::path& filePath : topLevelFilePathsInDirectory)
      {
         _recursiveFileDeleter->RemoveReadonlyFlagFromFileSystemFilePath(filePath);
      }
   }
#endif
}

// Open And Close Files

shared_ptr<FILE> FileSystem::OpenFile(const fs::path& filePath, const char* fileOpenMode) const
{
   FILE* const rawFilePointer = _call_fopen(filePath.string().c_str(), fileOpenMode);
   if (rawFilePointer == nullptr)
   {
      const FileSystemException fileSystemException =
         _fileSystemExceptionMaker->MakeFileSystemExceptionForFailedToOpenFileWithFOpen(filePath, fileOpenMode);
      throw fileSystemException;
   }
   shared_ptr<FILE> autoClosingFilePointer(rawFilePointer, FCloseDeleter());
   return autoClosingFilePointer;
}

// Private Functions

size_t FileSystem::GetFileSize(ifstream& fileStream) const
{
   fileStream.seekg(0, ios::end);
   const streampos fileSizeAsStreamPos = fileStream.tellg();
   fileStream.seekg(0, ios::beg);
   const size_t fileSizeAsSizeT = static_cast<size_t>(fileSizeAsStreamPos);
   return fileSizeAsSizeT;
}

void FileSystem::CreateBinaryOrTextFile(const fs::path& filePath, bool trueBinaryFalseText, const char* bytes, size_t bytesLength) const
{
   const fs::path parentDirectoryPath = filePath.parent_path();
   fs::create_directories(parentDirectoryPath);
   const shared_ptr<FILE> binaryOrTextFileOpenInWriteMode = OpenFile(filePath, trueBinaryFalseText ? "wb" : "w");
#if defined __linux__ || defined __APPLE__
   const size_t numberOfBytesWritten = fwrite(bytes, 1, bytesLength, binaryOrTextFileOpenInWriteMode.get());
#elif defined _WIN32
   const size_t numberOfBytesWritten = _fwrite_nolock(bytes, 1, bytesLength, binaryOrTextFileOpenInWriteMode.get());
#endif
   release_assert(numberOfBytesWritten == bytesLength);
}

void FileSystem::EraseTrailingBinaryZeros(string& outStr) const
{
   while (true)
   {
      const size_t size = outStr.size();
      if (size == 0)
      {
         break;
      }
      const char lastChar = outStr.back();
      if (lastChar == 0)
      {
         outStr.erase(size - 1);
      }
      else
      {
         break;
      }
   }
}
