#include "pch.h"
#include "libFileRevisor/Components/Exceptions/FileSystemExceptionMaker.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"
#include "libFileRevisor/Components/FileSystem/FileOpenerCloser.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/FileSystem/RecursiveFileDeleter.h"
#include "libFileRevisor/Exceptions/FileSystemException.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/Member/NonVoidOneArgMemberFunctionCaller.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/UtilityComponents/Strings/ConstCharPointerGetter.h"

FileSystem::FileSystem()
   // Function Pointers
   : _call_fopen(::fopen)
   , _call_fclose(::fclose)
   , _call_fs_remove_all(static_cast<uintmax_t(*)(const fs::path&, error_code&)>(fs::remove_all))
   , _call_std_rename(std::rename)
#ifdef _WIN32
   , _call_std_filesystem_absolute_as_assignable_function_pointer(fs::absolute)
#endif
   , _call_std_filesystem_current_path_as_assignable_function_pointer(fs::current_path)
   , _call_std_filesystem_exists_as_assignable_function_pointer(fs::exists)
   , _call_std_filesystem_rename_with_error_code_as_assignable_function_pointer(fs::rename)
   // Function Callers
   , _caller_Exists(make_unique<NonVoidOneArgMemberFunctionCaller<bool, FileSystem, const fs::path&>>())
   // Constant Components
   , _constCharPointerGetter(make_unique<ConstCharPointerGetter>())
   , _fileOpenerCloser(make_unique<FileOpenerCloser>())
   , _fileSystemExceptionMaker(make_unique<FileSystemExceptionMaker>())
   , _recursiveFileDeleter(make_unique<RecursiveFileDeleter>())
{
#ifdef _WIN32
   _call_std_filesystem_absolute = _call_std_filesystem_absolute_as_assignable_function_pointer;
#endif
   _call_std_filesystem_current_path = _call_std_filesystem_current_path_as_assignable_function_pointer;
   _call_std_filesystem_exists = _call_std_filesystem_exists_as_assignable_function_pointer;
   _call_std_filesystem_rename_with_error_code = _call_std_filesystem_rename_with_error_code_as_assignable_function_pointer;
}

FileSystem::~FileSystem()
{
}

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
   fs::path absoluteFileOrDirectoryPath = _call_std_filesystem_absolute(fileOrDirectoryPath);
   return absoluteFileOrDirectoryPath;
#endif
}

fs::path FileSystem::CurrentDirectoryPath() const
{
   fs::path currentDirectoryPath = _call_std_filesystem_current_path();
   return currentDirectoryPath;
}

void FileSystem::RecursivelyDeleteAllFilesInDirectory(
   const string& directoryPath, const FileRevisorArgs& args) const
{
   _recursiveFileDeleter->RecursivelyDeleteAllFilesInDirectory(directoryPath.c_str(), args);
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

bool FileSystem::FileOrDirectoryExists(const fs::path& fileOrDirectoryPath) const
{
   const bool fileOrDirectoryPathExists = _call_std_filesystem_exists(fileOrDirectoryPath);
   return fileOrDirectoryPathExists;
}

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

size_t FileSystem::GetFileSize(ifstream& fileStream) const
{
   fileStream.seekg(0, ios::end);
   const streampos fileSizeAsStreamPos = fileStream.tellg();
   fileStream.seekg(0, ios::beg);
   const size_t fileSizeAsSizeT = static_cast<size_t>(fileSizeAsStreamPos);
   return fileSizeAsSizeT;
}

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
   const bool filePathExists = _caller_Exists->ConstCall(this, &FileSystem::FileOrDirectoryExists, filePath);
   if (!filePathExists)
   {
      const string exceptionMessage = String::ConcatStrings(
         "FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) error: filePath does not exist: ", filePath.string());
      throw runtime_error(exceptionMessage);
   }
   const fs::path sourceDirectoryPath = filePath.parent_path();
   fs::path renamedFilePath = sourceDirectoryPath / newFileName;
   const bool destinationFilePathExists = _caller_Exists->ConstCall(this, &FileSystem::FileOrDirectoryExists, renamedFilePath);
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
   _call_std_filesystem_rename_with_error_code(directoryPath, renamedDirectoryPath, renameErrorCode);
   const int renameErrorCodeValue = renameErrorCode.value();
   if (renameErrorCodeValue != 0)
   {
      const FileSystemException fileSystemException =
         _fileSystemExceptionMaker->MakeFileSystemExceptionForFailedToRenameDirectory(directoryPath, renamedDirectoryPath, renameErrorCode);
      throw fileSystemException;
   }
   return renamedDirectoryPath;
}

void FileSystem::RemoveFile(const char* filePath) const
{
#if defined __linux__|| defined __APPLE__
   const int unlinkReturnValue = unlink(filePath);
#elif _WIN32
   const int unlinkReturnValue = _unlink(filePath);
#endif
   if (unlinkReturnValue != 0)
   {
      const FileSystemException fileSystemException = _fileSystemExceptionMaker->MakeFileSystemExceptionForFailedToDeleteFile(filePath);
      throw fileSystemException;
   }
}

void FileSystem::RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows([[maybe_unused]]const fs::path& directoryPath) const
{
#ifdef _WIN32
   const vector<fs::path> topLevelFilePathsInDirectory = GetFilePathsInDirectory(directoryPath, false);
   for (const fs::path& filePath : topLevelFilePathsInDirectory)
   {
      _recursiveFileDeleter->RemoveReadonlyFlagFromFileSystemFilePath(filePath);
   }
#endif
}

unsigned long long FileSystem::RemoveAll(const fs::path& directoryPath) const
{
   error_code errorCode;
   unsigned long long numberOfFilesAndDirectoriesRemoved = _call_fs_remove_all(directoryPath, errorCode);
   const int errorCodeValue = errorCode.value();
   if (errorCodeValue != 0)
   {
      const FileSystemException fileSystemException =
         _fileSystemExceptionMaker->MakeFileSystemExceptionForRemoveAllFailedToDeleteDirectory(
            directoryPath, numberOfFilesAndDirectoriesRemoved, errorCodeValue);
      throw fileSystemException;
   }
   return numberOfFilesAndDirectoriesRemoved;
}

FILE* FileSystem::OpenFile(const fs::path& filePath, const char* fileOpenMode) const
{
   FILE* const filePointer = _call_fopen(filePath.string().c_str(), fileOpenMode);
   if (filePointer == nullptr)
   {
      const FileSystemException fileSystemException =
         _fileSystemExceptionMaker->MakeFileSystemExceptionForFailedToOpenFileWithFOpen(filePath, fileOpenMode);
      throw fileSystemException;
   }
   return filePointer;
}

void FileSystem::CreateBinaryOrTextFile(const fs::path& filePath, bool trueBinaryFalseText, const char* bytes, size_t bytesLength) const
{
   const fs::path parentDirectoryPath = filePath.parent_path();
   fs::create_directories(parentDirectoryPath);
   FILE* const binaryOrTextFileOpenInWriteMode = OpenFile(filePath, trueBinaryFalseText ? "wb" : "w");
#ifdef _WIN32
   const size_t numberOfBytesWritten = _fwrite_nolock(bytes, 1, bytesLength, binaryOrTextFileOpenInWriteMode);
#else
   const size_t numberOfBytesWritten = fwrite(bytes, 1, bytesLength, binaryOrTextFileOpenInWriteMode);
#endif
   release_assert(numberOfBytesWritten == bytesLength);
   _fileOpenerCloser->CloseFile(binaryOrTextFileOpenInWriteMode, filePath);
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
