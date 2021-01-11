#include "pch.h"
#include "libFileRevisor/Components/Console/Console.h"
#include "libFileRevisor/Components/Exceptions/FileSystemExceptionMaker.h"
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/FileSystem/RecursiveFileDeleter.h"
#include "libFileRevisor/Components/FunctionCallers/Member/NonVoidOneArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/Strings/ConstCharPointerGetter.h"
#include "libFileRevisor/Exceptions/FileSystemException.h"

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
   , _call_std_filesystem_rename_as_assignable_function_pointer(fs::rename)
   // Function Callers
   , _caller_Exists(make_unique<NonVoidOneArgMemberFunctionCaller<bool, FileSystem, const fs::path&>>())
   , _caller_CloseFile(make_unique<VoidTwoArgMemberFunctionCaller<FileSystem, const fs::path&, FILE*>>())
   // Constant Components
   , _constCharPointerGetter(make_unique<ConstCharPointerGetter>())
   , _fileSystemExceptionMaker(make_unique<FileSystemExceptionMaker>())
   , _recursiveFileDeleter(make_unique<RecursiveFileDeleter>())
{
#ifdef _WIN32
   _call_std_filesystem_absolute = _call_std_filesystem_absolute_as_assignable_function_pointer;
#endif
   _call_std_filesystem_current_path = _call_std_filesystem_current_path_as_assignable_function_pointer;
   _call_std_filesystem_exists = _call_std_filesystem_exists_as_assignable_function_pointer;
   _call_std_filesystem_rename = _call_std_filesystem_rename_as_assignable_function_pointer;
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

vector<fs::path> FileSystem::GetNonEmptyNonGitTextFilePathsInDirectory(const fs::path& directoryPath, bool recurse) const
{
   vector<fs::path> textFilePaths;
   DirectoryIterator directoryIterator;
#if defined __linux__ || defined __APPLE__
   static const vector<string> fileAndDirectoryPathIgnoreSubstrings = { "/.git/" };
#elif defined _WIN32
   static const vector<string> fileAndDirectoryPathIgnoreSubstrings = { "\\.git\\" };
#endif
   directoryIterator.SetFileAndDirectoryPathIgnoreSubstrings(fileAndDirectoryPathIgnoreSubstrings);
   directoryIterator.SetDirectoryIterator(directoryPath, recurse);
   while (true)
   {
      fs::path nextNonIgnoredFilePath = directoryIterator.NextNonIgnoredFilePath();
      static const fs::path endIterationMarker{};
      if (nextNonIgnoredFilePath == endIterationMarker)
      {
         break;
      }
      const bool fileIsEmptyOrBinaryOrNonAnsi = IsFileEmptyOrBinaryOrNonAnsi(nextNonIgnoredFilePath);
      if (fileIsEmptyOrBinaryOrNonAnsi)
      {
         continue;
      }
      textFilePaths.emplace_back(std::move(nextNonIgnoredFilePath));
   }
   return textFilePaths;
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
      const FileSystemException fileSystemException = _fileSystemExceptionMaker->
         MakeFileSystemExceptionForFailedToOpenFileWithFStream(textFilePath);
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

bool FileSystem::IsFileEmptyOrBinaryOrNonAnsi(const fs::path& filePath) const
{
   FILE* const fileOpenInReadBinaryMode = OpenFile(filePath, "rb");
   char first1KBytesInFile[1024];
#if defined __linux__|| defined __APPLE__
   const size_t numberOfBytesRead = fread(
      first1KBytesInFile, 1, sizeof(first1KBytesInFile), fileOpenInReadBinaryMode);
#else
   const size_t numberOfBytesRead = _fread_nolock_s(
      first1KBytesInFile, sizeof(first1KBytesInFile), 1, sizeof(first1KBytesInFile), fileOpenInReadBinaryMode);
#endif
   _caller_CloseFile->ConstCall(this, &FileSystem::CloseFile, filePath, fileOpenInReadBinaryMode);
   if (numberOfBytesRead == 0)
   {
      return true;
   }
   for (size_t i = 0; i < numberOfBytesRead; ++i)
   {
      const char b = first1KBytesInFile[i];
      if (b == 0)
      {
         return true;
      }
   }
   return false;
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
      const string exceptionMessage = "FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) error: filePath does not exist: " + filePath.string();
      throw runtime_error(exceptionMessage);
   }
   const fs::path sourceDirectoryPath = filePath.parent_path();
   fs::path renamedFilePath = sourceDirectoryPath / newFileName;
   const bool destinationFilePathExists = _caller_Exists->ConstCall(
      this, &FileSystem::FileOrDirectoryExists, renamedFilePath);
   if (destinationFilePathExists)
   {
      throw runtime_error("FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) error: renamedFilePath already exists: " + renamedFilePath.string());
   }
   const string filePathString = filePath.string();
   const char* const filePathStringCCP = _constCharPointerGetter->GetStringConstCharPointer(filePathString);
   const string renamedFilePathString = renamedFilePath.string();
   const char* const renamedFilePathStringCCP =
      _constCharPointerGetter->GetStringConstCharPointer(renamedFilePathString);
   const int renameReturnValue = _call_std_rename(filePathStringCCP, renamedFilePathStringCCP);
   if (renameReturnValue != 0)
   {
      const string exceptionMessage = String::Concat("FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) error: std::rename(\"",
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
   _call_std_filesystem_rename(directoryPath, renamedDirectoryPath, renameErrorCode);
   const int renameErrorCodeValue = renameErrorCode.value();
   if (renameErrorCodeValue != 0)
   {
      const FileSystemException fileSystemException =
         _fileSystemExceptionMaker->MakeFileSystemExceptionForFailedToRenameDirectory(
            directoryPath, renamedDirectoryPath, renameErrorCode);
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
      const FileSystemException fileSystemException =
         _fileSystemExceptionMaker->MakeFileSystemExceptionForFailedToDeleteFile(filePath);
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

void FileSystem::CloseFile(const fs::path& filePath, FILE* filePointer) const
{
   const int fcloseReturnValue = _call_fclose(filePointer);
   if (fcloseReturnValue != 0)
   {
      const FileSystemException fileSystemException =
         _fileSystemExceptionMaker->MakeFileSystemExceptionForFailedToCloseFile(filePath);
      throw fileSystemException;
   }
}

FILE* FileSystem::OpenFile(const fs::path& filePath, const char* fileOpenMode) const
{
   FILE* const filePointer = _call_fopen(filePath.string().c_str(), fileOpenMode);
   if (filePointer == nullptr)
   {
      const FileSystemException fileSystemException = _fileSystemExceptionMaker->
         MakeFileSystemExceptionForFailedToOpenFileWithFOpen(filePath, fileOpenMode);
      throw fileSystemException;
   }
   return filePointer;
}

void FileSystem::CreateBinaryOrTextFile(
   const fs::path& filePath, bool trueBinaryFalseText, const char* bytes, size_t bytesLength) const
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
   _caller_CloseFile->ConstCall(this, &FileSystem::CloseFile, filePath, binaryOrTextFileOpenInWriteMode);
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
