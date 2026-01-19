#include "pch.h"
#include "libFileRevisor/Components/Exceptions/FileSystemExceptionMaker.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"
#include "libFileRevisor/Components/FileSystem/FCloseDeleter.h"
#include "libFileRevisor/Components/FileSystem/FileOpenerCloser.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/FileSystem/RecursiveFileDeleter.h"
#include "libFileRevisor/Components/FunctionCallers/Member/NonVoidOneArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/FunctionCallers/Member/NonVoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidOneArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/Iteration/ForEach/FourArgMemberFunctionForEacher.h"
#include "libFileRevisor/Components/Strings/ConstCharPointerGetter.h"
#include "libFileRevisor/Exceptions/FileSystemException.h"

FileSystem::FileSystem()
   // Function Pointers
   : _call_fclose(::fclose)
   , _call_fopen(::fopen)
   , _call_fs_is_directory(static_cast<bool(*)(const fs::path&)>(fs::is_directory))
   , _call_fs_remove(static_cast<bool(*)(const fs::path&)>(fs::remove))
   , _call_fs_remove_all(static_cast<uintmax_t(*)(const fs::path&)>(fs::remove_all))
   , _call_std_rename(std::rename)
#ifdef _WIN32
   , _call_fs_absolute_as_assignable_function_pointer(fs::absolute)
   , _call_fs_absolute(_call_fs_absolute_as_assignable_function_pointer)
#endif
   , _call_fs_current_path_as_assignable_function_pointer(fs::current_path)
   , _call_fs_current_path(_call_fs_current_path_as_assignable_function_pointer)
   , _call_fs_exists_as_assignable_function_pointer(fs::exists)
   , _call_fs_exists(_call_fs_exists_as_assignable_function_pointer)
   , _call_fs_rename_with_error_code_as_assignable_function_pointer(fs::rename)
   , _call_fs_rename_with_error_code(_call_fs_rename_with_error_code_as_assignable_function_pointer)
   // Function Callers
   , _caller_DeleteFileOrDirectory(make_unique<_caller_DeleteFileOrDirectoryType>())
   , _caller_DoDeleteFileOrDirectory(make_unique<_caller_DoDeleteFileOrDirectoryType>())
   , _caller_FileSystem_Exists(make_unique<_caller_FileSystem_ExistsType>())
   , _caller_GetFileOrFolderPathsInDirectory(make_unique<_caller_GetFileOrFolderPathsInDirectoryType>())
   , _foreacher_DeleteFileOrDirectory(make_unique<_foreacher_DeleteFileOrDirectoryType>())
   // Constant Components
   , _console(make_unique<Console>())
   , _constCharPointerGetter(make_unique<ConstCharPointerGetter>())
   , _fileOpenerCloser(make_unique<FileOpenerCloser>())
   , _fileSystemExceptionMaker(make_unique<FileSystemExceptionMaker>())
   , _recursiveFileDeleter(make_unique<RecursiveFileDeleter>())
{
}

FileSystem::~FileSystem()
{
}

// Queries

fs::path FileSystem::GetAbsolutePath(const fs::path& fileOrFolderPath) const
{
#if defined __linux__
   fs::path absoluteFileOrFolderPath = fs::absolute(fileOrFolderPath);
   const fs::path absoluteFileOrFolderPathFileName = absoluteFileOrFolderPath.filename();
   if (absoluteFileOrFolderPathFileName == ".")
   {
      // On Linux, "/dir/subdir/." becomes "/dir/subdir"
      absoluteFileOrFolderPath = absoluteFileOrFolderPath.parent_path();
   }
   return absoluteFileOrFolderPath;
#elif _WIN32
   fs::path absoluteFileOrFolderPath = _call_fs_absolute(fileOrFolderPath);
   return absoluteFileOrFolderPath;
#endif
}

fs::path FileSystem::CurrentFolderPath() const
{
   fs::path currentFolderPath = _call_fs_current_path();
   return currentFolderPath;
}

bool FileSystem::FileOrDirectoryExists(const fs::path& fileOrFolderPath) const
{
   const bool fileOrFolderPathExists = _call_fs_exists(fileOrFolderPath);
   return fileOrFolderPathExists;
}

vector<fs::path> FileSystem::GetFolderPathsInDirectory(const fs::path& directoryPath, bool recurse) const
{
   vector<fs::path> directoryPaths;
   DirectoryIterator directoryIterator;
   directoryIterator.SetDirectoryIterator(directoryPath, recurse);
   while (true)
   {
      fs::path nonIgnoredFolderPath = directoryIterator.NextNonIgnoredFolderPath();
      static const fs::path endIterationMarker;
      if (nonIgnoredFolderPath == endIterationMarker)
      {
         break;
      }
      directoryPaths.emplace_back(std::move(nonIgnoredFolderPath));
   }
   return directoryPaths;
}

vector<fs::path> FileSystem::GetFilePathsInDirectory(const fs::path& directoryPath, bool recurse) const
{
   vector<fs::path> filePaths;
   DirectoryIterator directoryIterator;
   directoryIterator.SetDirectoryIterator(directoryPath, recurse);
   static const fs::path endIterationMarker;
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

vector<string> FileSystem::GetStringFolderPathsInDirectory(const fs::path& directoryPath, bool recurse) const
{
   const vector<fs::path> subdirectoryPaths = GetFolderPathsInDirectory(directoryPath, recurse);
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
   ifstream textFileStream(textFilePath.c_str(), ios_base::binary);
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
   CreateFileWithBytes(filePath, fileText.data(), fileText.size());
}

void FileSystem::CreateFileWithBytes(const fs::path& filePath, const char* bytes, size_t bytesLength) const
{
   const fs::path parentFolderPath = filePath.parent_path();
   fs::create_directories(parentFolderPath);
   const shared_ptr<FILE> binaryOrTextFileOpenInWriteMode = OpenFile(filePath, "wb");
#if defined __linux__
   const size_t numberOfBytesWritten = fwrite(bytes, 1, bytesLength, binaryOrTextFileOpenInWriteMode.get());
#elif defined _WIN32
   const size_t numberOfBytesWritten = _fwrite_nolock(bytes, 1, bytesLength, binaryOrTextFileOpenInWriteMode.get());
#endif
   release_assert(numberOfBytesWritten == bytesLength);
}

fs::path FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) const
{
   const bool filePathExists = _caller_FileSystem_Exists->CallConstMemberFunction(this, &FileSystem::FileOrDirectoryExists, filePath);
   if (!filePathExists)
   {
      const string exceptionMessage = String::ConcatStrings(
         "FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) error: filePath does not exist: ", filePath.string());
      throw runtime_error(exceptionMessage);
   }
   const fs::path sourceFolderPath = filePath.parent_path();
   fs::path renamedFilePath = sourceFolderPath / newFileName;
   const bool destinationFilePathExists = _caller_FileSystem_Exists->CallConstMemberFunction(this, &FileSystem::FileOrDirectoryExists, renamedFilePath);
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
   const fs::path directoryPathMinusLeafDirectory = [&]() // LCOV_EXCL_LINE
   {
      return fs::path(directoryPath).remove_filename();
   }();
   fs::path renamedFolderPath = directoryPathMinusLeafDirectory / newDirectoryName;
   error_code renameErrorCode;
   _call_fs_rename_with_error_code(directoryPath, renamedFolderPath, renameErrorCode);
   const int renameErrorCodeValue = renameErrorCode.value();
   if (renameErrorCodeValue != 0)
   {
      const FileSystemException fileSystemException =
         _fileSystemExceptionMaker->MakeFileSystemExceptionForFailedToRenameDirectory(directoryPath, renamedFolderPath, renameErrorCode);
      throw fileSystemException;
   }
   return renamedFolderPath;
}

// Deletes

void FileSystem::DeleteTopLevelFilesAndEmptyDirectoriesInDirectory(
   const fs::path& directoryPath, bool skipFilesInUse, bool dryRun, bool quietMode) const
{
   _caller_DeleteFileOrDirectory->CallConstMemberFunction(
      this, &FileSystem::RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows, directoryPath, dryRun);

   const vector<fs::path> topLevelFolderPaths = _caller_GetFileOrFolderPathsInDirectory->CallConstMemberFunction(
      this, &FileSystem::GetFolderPathsInDirectory, directoryPath, false);
   const vector<fs::path> topLevelFilePaths = _caller_GetFileOrFolderPathsInDirectory->CallConstMemberFunction(
      this, &FileSystem::GetFilePathsInDirectory, directoryPath, false);

   _foreacher_DeleteFileOrDirectory->CallConstMemberFunctionWithEachElement(
      topLevelFolderPaths, this, &FileSystem::DeleteFileOrDirectory, skipFilesInUse, dryRun, quietMode);
   _foreacher_DeleteFileOrDirectory->CallConstMemberFunctionWithEachElement(
      topLevelFilePaths, this, &FileSystem::DeleteFileOrDirectory, skipFilesInUse, dryRun, quietMode);
}

void FileSystem::RemoveFile(const char* filePath, bool ignoreFileDeleteError) const
{
#if defined __linux__
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

void FileSystem::DeleteFileOrDirectory(const fs::path& fileOrFolderPath, bool ignoreFileDeleteError, bool dryRun, bool quietMode) const
{
   if (dryRun)
   {
      const string wouldDeleteMessage = "DryRun: Would delete " + fileOrFolderPath.string();
      _console->ProgramNameThreadIdWriteLine(wouldDeleteMessage);
   }
   else
   {
      try
      {
         _caller_DoDeleteFileOrDirectory->CallConstMemberFunction(this, &FileSystem::DoDeleteFileOrDirectory, fileOrFolderPath);
         if (!quietMode)
         {
            const string deletedMessage = "Deleted " + fileOrFolderPath.string();
            _console->ProgramNameThreadIdWriteLine(deletedMessage);
         }
      }
      catch (const exception& ex)
      {
         if (ignoreFileDeleteError)
         {
            const string exceptionClassNameAndMessage = Type::GetExceptionClassNameAndMessage(&ex);
            const string message = "Ignoring exception because --skip-files-in-use: " + exceptionClassNameAndMessage;
            _console->ProgramNameThreadIdWriteLineColor(message, Color::Yellow);
         }
         else
         {
            throw;
         }
      }
   }
}

// Readonly Flags

void FileSystem::RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows(
   [[maybe_unused]]const fs::path& directoryPath, [[maybe_unused]]bool dryRun) const
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

void FileSystem::DoDeleteFileOrDirectory(const fs::path& fileOrFolderPath) const
{
   const bool isDirectory = _call_fs_is_directory(fileOrFolderPath);
   if (isDirectory)
   {
      _call_fs_remove_all(fileOrFolderPath);
   }
   else
   {
      _call_fs_remove(fileOrFolderPath);
   }
}

size_t FileSystem::GetFileSize(ifstream& fileStream) const
{
   fileStream.seekg(0, ios::end);
   const streampos fileSizeAsStreamPos = fileStream.tellg();
   fileStream.seekg(0, ios::beg);
   const size_t fileSizeAsSizeT = static_cast<size_t>(fileSizeAsStreamPos);
   return fileSizeAsSizeT;
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
