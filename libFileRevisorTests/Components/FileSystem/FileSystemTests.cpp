#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Enums/FileExceptionType.h"
#include "libFileRevisorTests/Components/Exceptions/MetalMock/FileSystemExceptionMakerMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/DirectoryIteratorMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileOpenerCloserMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/RecursiveFileDeleterMock.h"
#include "libFileRevisorTests/Exceptions/ZenUnit/FileSystemExceptionRandom.h"
#include "libFileRevisorTests/UtilityComponents/FunctionCallers/Member/MetalMock/NonVoidOneArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/UtilityComponents/FunctionCallers/Member/MetalMock/NonVoidTwoArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/UtilityComponents/FunctionCallers/Member/MetalMock/VoidTwoArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/UtilityComponents/Iteration/ForEach/MetalMock/ThreeArgMemberFunctionForEacherMock.h"
#include "libFileRevisorTests/UtilityComponents/Iteration/ForEach/MetalMock/ParallelThreeArgMemberFunctionForEacherMock.h"
#include "libFileRevisorTests/UtilityComponents/Strings/MetalMock/ConstCharPointerGetterMock.h"

TESTS(FileSystemTests)
AFACT(DefaultConstructor_NewsComponents_SetsFunctionPointers)
// Deletes
AFACT(DeleteTopLevelFilesAndEmptyDirectoriesInDirectory_IfWindowsRemovesReadOnlyFlagsFromTopLevelFiles_SequentiallyDeletesTopLevelFiles_SequentiallyDeletesTopLevelEmptyDirectories)
AFACT(RecursivelyDeleteAllFilesInDirectory_CallsFileDeleterRecursivelyDeleteAllFilesInDirectory)
AFACT(RemoveFileSystemFileOrDirectory_IgnoreFileDeleteErrorIsTrueOrFalse_DryRunIsTrue_WritesWouldDeleteFilePathMessage_DoesNotDeleteFile)
AFACT(RemoveFileSystemFileOrDirectory_IgnoreFileDeleteErrorIsTrueOrFalse_DryRunIsFalse_DeletesFileWhichDoesNotThrow_Returns)
AFACT(RemoveFileSystemFileOrDirectory_IgnoreFileDeleteErrorIsTrue_DryRunIsFalse_DeletesFileWhichThrows_WritesExceptionMessage_Returns)
AFACT(RemoveFileSystemFileOrDirectory_IgnoreFileDeleteErrorIsFalse_DryRunIsFalse_DeletesFileWhichThrows_RethrowsException)
// Open File
AFACT(OpenFile_FOpenReturnsNullFILEPointer_ThrowsFileSystemException)
AFACT(OpenFile_FOpenReturnsNonNullFILEPointer_ReturnsFilePointer)
#if defined __linux__|| defined __APPLE__
AFACT(Linux__RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_DoesNothing)
#elif _WIN32
AFACT(Windows__RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_DryRunIsTrue_DoesNothing)
AFACT(Windows__RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_DryRunIsFalse_CallsRemoveReadonlyFlagOnAllTopLevelFilesInDirectory)
#endif
// Queries
#if defined __linux__|| defined __APPLE__
AFACT(Linux__GetAbsolutePath_ReturnsNonEmptyPath)
#elif _WIN32
AFACT(Windows__GetAbsolutePath_ReturnsResultOfCallingStdFilesystemAbsolute)
#endif
AFACT(CurrentDirectoryPath_ReturnsResultOfCallingFilesystemCurrentPath)
AFACT(FileOrDirectoryExists_ReturnsResultOfCallingStdFilesystemExists)
// Writes
AFACT(RenameFile_FilePathDoesNotExist_ThrowsRuntimeError)
AFACT(RenameFile_FilePathExists_DestinationFilePathAlreadyExists_ThrowsRuntimeError)
FACTS(RenameFile_FilePathExists_DestinationFilePathDoesNotExist_RenamesFile_ThrowsIfRenameReturnsNon0_OtherwiseReturns)
AFACT(RenameDirectory_RenamesDirectory_FilesystemRenameReturns0_ReturnsRenamedDirectoryPath)
FACTS(RenameDirectory_RenamesDirectory_FilesystemRenameReturnsNot0_ThrowsFileSystemException)
EVIDENCE

FileSystem _fileSystem;
// Function Pointers
METALMOCK_NONVOID1_FREE(int, _call_fclose, FILE*)
METALMOCK_NONVOID2_FREE(FILE*, _call_fopen, const char*, const char*)
METALMOCK_NONVOID1_FREE(bool, _call_fs_remove, const fs::path&)
METALMOCK_NONVOID2_FREE(uintmax_t, _call_fs_remove_all, const fs::path&, error_code&)
#ifdef _WIN32
METALMOCK_NONVOID1_FREE(fs::path, _call_fs_absolute, const fs::path&)
#endif
METALMOCK_NONVOID0_FREE(fs::path, _call_fs_current_path)
METALMOCK_NONVOID1_FREE(bool, _call_fs_exists, const fs::path&)
METALMOCK_NONVOID2_FREE(int, _call_std_rename, const char*, const char*)
METALMOCK_VOID3_FREE(_call_fs_rename_with_error_code, const fs::path&, const fs::path&, std::error_code&)
// Function Callers
NonVoidOneArgMemberFunctionCallerMock<bool, FileSystem, const fs::path&>* _caller_ExistsMock = nullptr;

using _caller_GetFileOrDirectoryPathsInDirectoryMockType = NonVoidTwoArgMemberFunctionCallerMock<vector<fs::path>, FileSystem, const fs::path&, bool>;
_caller_GetFileOrDirectoryPathsInDirectoryMockType* _caller_GetFileOrDirectoryPathsInDirectoryMock = nullptr;

using _caller_RemoveFileSystemFileOrDirectoryMockType = VoidTwoArgMemberFunctionCallerMock<FileSystem, const fs::path&, bool>;
_caller_RemoveFileSystemFileOrDirectoryMockType* _caller_RemoveFileSystemFileOrDirectoryMock = nullptr;

using _foreacher_DeleteFileOrDirectoryMockType = ThreeArgMemberFunctionForEacherMock<FileSystem, fs::path, bool, bool>;
_foreacher_DeleteFileOrDirectoryMockType* _foreacher_DeleteFileOrDirectoryMock = nullptr;

using _parallelForeacher_DeleteFileOrDirectoryMockType = ParallelThreeArgMemberFunctionForEacherMock<FileSystem, fs::path, bool, bool>;
_parallelForeacher_DeleteFileOrDirectoryMockType* _parallelForeacher_DeleteFileOrDirectoryMock = nullptr;
// Constant Components
ConsoleMock* _consoleMock = nullptr;
ConstCharPointerGetterMock* _constCharPointerGetterMock = nullptr;
FileOpenerCloserMock* _fileOpenerCloserMock = nullptr;
FileSystemExceptionMakerMock* _fileSystemExceptionMakerMock = nullptr;
RecursiveFileDeleterMock* _recursiveFileDeleterMock = nullptr;

STARTUP
{
   // Function Pointers
   _fileSystem._call_fclose = BIND_1ARG_METALMOCK_OBJECT(_call_fcloseMock);
   _fileSystem._call_fopen = BIND_2ARG_METALMOCK_OBJECT(_call_fopenMock);
   _fileSystem._call_fs_remove = BIND_1ARG_METALMOCK_OBJECT(_call_fs_removeMock);
   _fileSystem._call_fs_remove_all = BIND_2ARG_METALMOCK_OBJECT(_call_fs_remove_allMock);
#ifdef _WIN32
   _fileSystem._call_fs_absolute = BIND_1ARG_METALMOCK_OBJECT(_call_fs_absoluteMock);
#endif
   _fileSystem._call_std_rename = BIND_2ARG_METALMOCK_OBJECT(_call_std_renameMock);
   _fileSystem._call_fs_rename_with_error_code = BIND_3ARG_METALMOCK_OBJECT(_call_fs_rename_with_error_codeMock);
   _fileSystem._call_fs_current_path = BIND_0ARG_METALMOCK_OBJECT(_call_fs_current_pathMock);
   _fileSystem._call_fs_exists = BIND_1ARG_METALMOCK_OBJECT(_call_fs_existsMock);
   // Function Callers
   _fileSystem._caller_Exists.reset(_caller_ExistsMock = new NonVoidOneArgMemberFunctionCallerMock<bool, FileSystem, const fs::path&>);
   _fileSystem._caller_GetFileOrDirectoryPathsInDirectory.reset(_caller_GetFileOrDirectoryPathsInDirectoryMock = new _caller_GetFileOrDirectoryPathsInDirectoryMockType);
   _fileSystem._caller_RemoveFileSystemFileOrDirectory.reset(_caller_RemoveFileSystemFileOrDirectoryMock = new _caller_RemoveFileSystemFileOrDirectoryMockType);
   _fileSystem._foreacher_DeleteFileOrDirectory.reset(_foreacher_DeleteFileOrDirectoryMock = new _foreacher_DeleteFileOrDirectoryMockType);
   // Constant Components
   _fileSystem._console.reset(_consoleMock = new ConsoleMock);
   _fileSystem._constCharPointerGetter.reset(_constCharPointerGetterMock = new ConstCharPointerGetterMock);
   _fileSystem._fileOpenerCloser.reset(_fileOpenerCloserMock = new FileOpenerCloserMock);
   _fileSystem._fileSystemExceptionMaker.reset(_fileSystemExceptionMakerMock = new FileSystemExceptionMakerMock);
   _fileSystem._recursiveFileDeleter.reset(_recursiveFileDeleterMock = new RecursiveFileDeleterMock);
}

TEST(DefaultConstructor_NewsComponents_SetsFunctionPointers)
{
   FileSystem fileSystem;
   // Function Pointers
   STD_FUNCTION_TARGETS(::fopen, fileSystem._call_fopen);
   STD_FUNCTION_TARGETS(::fclose, fileSystem._call_fclose);

   STD_FUNCTION_TARGETS(std::rename, fileSystem._call_std_rename);
#ifdef _WIN32
   using fs_remove_all_FunctionType = uintmax_t(*)(const fs::path&, error_code&);
   STD_FUNCTION_TARGETS_OVERLOAD(fs_remove_all_FunctionType, fs::remove_all, fileSystem._call_fs_remove_all);

   using StdFilesystemAbsoluteFunctionType = fs::path(*)(const fs::path&);
   STD_FUNCTION_TARGETS_OVERLOAD(StdFilesystemAbsoluteFunctionType, fs::absolute, fileSystem._call_fs_absolute);

   using StdFilesystemCurrentPathFunctionType = fs::path(*)();
   STD_FUNCTION_TARGETS_OVERLOAD(StdFilesystemCurrentPathFunctionType, fs::current_path, fileSystem._call_fs_current_path);

   using StdFilesystemExistsFunctionType = bool(*)(const fs::path&);
   STD_FUNCTION_TARGETS_OVERLOAD(StdFilesystemExistsFunctionType, fs::exists, fileSystem._call_fs_exists);
#endif
   // Function Callers
   DELETE_TO_ASSERT_NEWED(fileSystem._caller_Exists);
   DELETE_TO_ASSERT_NEWED(fileSystem._caller_GetFileOrDirectoryPathsInDirectory);
   DELETE_TO_ASSERT_NEWED(fileSystem._caller_RemoveFileSystemFileOrDirectory);
   DELETE_TO_ASSERT_NEWED(fileSystem._foreacher_DeleteFileOrDirectory);
   // Constant Components
   DELETE_TO_ASSERT_NEWED(fileSystem._console);
   DELETE_TO_ASSERT_NEWED(fileSystem._constCharPointerGetter);
   DELETE_TO_ASSERT_NEWED(fileSystem._fileOpenerCloser);
   DELETE_TO_ASSERT_NEWED(fileSystem._fileSystemExceptionMaker);
   DELETE_TO_ASSERT_NEWED(fileSystem._recursiveFileDeleter);
}

// Deletes

TEST(DeleteTopLevelFilesAndEmptyDirectoriesInDirectory_IfWindowsRemovesReadOnlyFlagsFromTopLevelFiles_SequentiallyDeletesTopLevelFiles_SequentiallyDeletesTopLevelEmptyDirectories)
{
   _caller_RemoveFileSystemFileOrDirectoryMock->CallConstMemberFunctionMock.Expect();

   const vector<fs::path> topLevelDirectoryPaths = ZenUnit::RandomVector<fs::path>();
   const vector<fs::path> topLevelFilePaths = ZenUnit::RandomVector<fs::path>();
   _caller_GetFileOrDirectoryPathsInDirectoryMock->CallConstMemberFunctionMock.ReturnValues(topLevelDirectoryPaths, topLevelFilePaths);

   _foreacher_DeleteFileOrDirectoryMock->CallConstMemberFunctionWithEachElementMock.Expect();

   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   const bool skipFilesInUse = ZenUnit::Random<bool>();
   const bool dryRun = ZenUnit::Random<bool>();
   //
   _fileSystem.DeleteTopLevelFilesAndEmptyDirectoriesInDirectory(directoryPath, skipFilesInUse, dryRun);
   //
   METALMOCK(_caller_RemoveFileSystemFileOrDirectoryMock->CallConstMemberFunctionMock.CalledOnceWith(
      &_fileSystem, &FileSystem::RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows, directoryPath, dryRun));
   METALMOCK(_caller_GetFileOrDirectoryPathsInDirectoryMock->CallConstMemberFunctionMock.CalledAsFollows(
   {
      { &_fileSystem, &FileSystem::GetDirectoryPathsInDirectory, directoryPath, false },
      { &_fileSystem, &FileSystem::GetFilePathsInDirectory, directoryPath, false },
   }));
   METALMOCK(_foreacher_DeleteFileOrDirectoryMock->CallConstMemberFunctionWithEachElementMock.CalledAsFollows(
   {
      { topLevelDirectoryPaths, &_fileSystem, &FileSystem::RemoveFileSystemFileOrDirectory, skipFilesInUse, dryRun },
      { topLevelFilePaths, &_fileSystem, &FileSystem::RemoveFileSystemFileOrDirectory, skipFilesInUse, dryRun }
   }));
}

TEST(RecursivelyDeleteAllFilesInDirectory_CallsFileDeleterRecursivelyDeleteAllFilesInDirectory)
{
   _recursiveFileDeleterMock->RecursivelyDeleteAllFilesInDirectoryMock.Expect();
   const string directoryPath = ZenUnit::Random<string>();
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   _fileSystem.RecursivelyDeleteAllFilesInDirectory(directoryPath, args);
   //
   METALMOCK(_recursiveFileDeleterMock->
      RecursivelyDeleteAllFilesInDirectoryMock.CalledOnceWith(directoryPath.c_str(), args));
}

TEST(RemoveFileSystemFileOrDirectory_IgnoreFileDeleteErrorIsTrueOrFalse_DryRunIsTrue_WritesWouldDeleteFilePathMessage_DoesNotDeleteFile)
{
   _consoleMock->ThreadIdWriteLineMock.Expect();
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const bool ignoreFileDeleteError = ZenUnit::Random<bool>();
   //
   _fileSystem.RemoveFileSystemFileOrDirectory(filePath, ignoreFileDeleteError, true);
   //
   const string expectedWouldDeleteMessage = "DryRun: Would delete " + filePath.string();
   METALMOCK(_consoleMock->ThreadIdWriteLineMock.CalledOnceWith(expectedWouldDeleteMessage));
}

TEST(RemoveFileSystemFileOrDirectory_IgnoreFileDeleteErrorIsTrueOrFalse_DryRunIsFalse_DeletesFileWhichDoesNotThrow_Returns)
{
   _call_fs_removeMock.ReturnRandom();
   _consoleMock->ThreadIdWriteLineMock.Expect();
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const bool ignoreFileDeleteError = ZenUnit::Random<bool>();
   //
   _fileSystem.RemoveFileSystemFileOrDirectory(filePath, ignoreFileDeleteError, false);
   //
   METALMOCK(_call_fs_removeMock.CalledOnceWith(filePath));
   const string expectedDeletedFileMessage = "Deleted " + filePath.string();
   METALMOCK(_consoleMock->ThreadIdWriteLineMock.CalledOnceWith(expectedDeletedFileMessage));
}

TEST(RemoveFileSystemFileOrDirectory_IgnoreFileDeleteErrorIsTrue_DryRunIsFalse_DeletesFileWhichThrows_WritesExceptionMessage_Returns)
{
   const string exceptionMessage = ZenUnit::Random<string>();
   _call_fs_removeMock.ThrowExceptionWhenCalled<runtime_error>(exceptionMessage);
   _consoleMock->ThreadIdWriteLineColorMock.Expect();
   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   _fileSystem.RemoveFileSystemFileOrDirectory(filePath, true, false);
   //
   METALMOCK(_call_fs_removeMock.CalledOnceWith(filePath));
   const string expectedExceptionClassNameAndMessage = "std::runtime_error: " + exceptionMessage;
   const string expectedIgnoringExceptionMessage = "Ignoring exception because --skip-files-in-use: " + expectedExceptionClassNameAndMessage;
   METALMOCK(_consoleMock->ThreadIdWriteLineColorMock.CalledOnceWith(expectedIgnoringExceptionMessage, Color::Yellow));
}

TEST(RemoveFileSystemFileOrDirectory_IgnoreFileDeleteErrorIsFalse_DryRunIsFalse_DeletesFileWhichThrows_RethrowsException)
{
   const string exceptionMessage = ZenUnit::Random<string>();
   _call_fs_removeMock.ThrowExceptionWhenCalled<runtime_error>(exceptionMessage);
   const fs::path filePath = ZenUnit::Random<fs::path>();
   //
   THROWS_EXCEPTION(_fileSystem.RemoveFileSystemFileOrDirectory(filePath, false, false),
      runtime_error, exceptionMessage);
   //
   METALMOCK(_call_fs_removeMock.CalledOnceWith(filePath));
}

// Open File

TEST(OpenFile_FOpenReturnsNullFILEPointer_ThrowsFileSystemException)
{
   _fopen_ReturnValue = nullptr;
   _call_fopenMock.CallInstead(std::bind(&FileSystemTests::fopen_CallInstead, this, placeholders::_1, placeholders::_2));

   const FileSystemException fileSystemException =
      _fileSystemExceptionMakerMock->MakeFileSystemExceptionForFailedToOpenFileWithFOpenMock.ReturnRandom();

   const fs::path filePath = ZenUnit::Random<fs::path>();
   const string fileOpenMode = ZenUnit::Random<string>();
   //
   THROWS_EXCEPTION(_fileSystem.OpenFile(filePath, fileOpenMode.c_str()),
      FileSystemException, fileSystemException.what());
   //
   const vector<pair<string, string>> expected_fopen_Arguments = { { filePath.string(), fileOpenMode } };
   ARE_EQUAL(expected_fopen_Arguments, _fopen_Arguments);
   METALMOCK(_fileSystemExceptionMakerMock->MakeFileSystemExceptionForFailedToOpenFileWithFOpenMock.
      CalledOnceWith(filePath, fileOpenMode.c_str()));
}

TEST(OpenFile_FOpenReturnsNonNullFILEPointer_ReturnsFilePointer)
{
   FILE* const rawFilePointer = tmpfile();
   _fopen_ReturnValue = rawFilePointer;
   _call_fopenMock.CallInstead(std::bind(&FileSystemTests::fopen_CallInstead, this, placeholders::_1, placeholders::_2));

   const fs::path filePath = ZenUnit::Random<fs::path>();
   const string fileOpenMode = ZenUnit::Random<string>();
   //
   const shared_ptr<FILE> filePointer = _fileSystem.OpenFile(filePath, fileOpenMode.c_str());
   //
   const vector<pair<string, string>> expected_fopen_Arguments = { { filePath.string(), fileOpenMode } };
   ARE_EQUAL(expected_fopen_Arguments, _fopen_Arguments);
   ARE_EQUAL(rawFilePointer, filePointer.get());
}

#if defined __linux__|| defined __APPLE__

TEST(Linux__RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_DoesNothing)
{
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   const bool dryRun = ZenUnit::Random<bool>();
   _fileSystem.RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows(directoryPath, dryRun);
}

#elif _WIN32

TEST(Windows__RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_DryRunIsTrue_DoesNothing)
{
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   _fileSystem.RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows(directoryPath, true);
}

TEST(Windows__RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_DryRunIsFalse_CallsRemoveReadonlyFlagOnAllTopLevelFilesInDirectory)
{
   class FileSystemSelfMocked : public Metal::Mock<FileSystem>
   {
   public:
      RecursiveFileDeleterMock* recursiveFileDeleterMock = nullptr;
      FileSystemSelfMocked()
      {
         _recursiveFileDeleter.reset(recursiveFileDeleterMock = new RecursiveFileDeleterMock);
      }
      METALMOCK_NONVOID2_CONST(vector<fs::path>, GetFilePathsInDirectory, const fs::path&, bool)
   } fileSystemSelfMocked;

   const vector<fs::path> topLevelFilePathsInDirectory =
   {
      ZenUnit::Random<fs::path>(),
      ZenUnit::Random<fs::path>()
   };
   fileSystemSelfMocked.GetFilePathsInDirectoryMock.Return(topLevelFilePathsInDirectory);

   fileSystemSelfMocked.recursiveFileDeleterMock->RemoveReadonlyFlagFromFileSystemFilePathMock.Expect();

   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   //
   fileSystemSelfMocked.RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows(directoryPath, false);
   //
   METALMOCK(fileSystemSelfMocked.GetFilePathsInDirectoryMock.CalledOnceWith(directoryPath, false));
   METALMOCK(fileSystemSelfMocked.recursiveFileDeleterMock->RemoveReadonlyFlagFromFileSystemFilePathMock.CalledAsFollows(
   {
      topLevelFilePathsInDirectory[0],
      topLevelFilePathsInDirectory[1]
   }));
}

#endif

// Queries

#if defined __linux__|| defined __APPLE__
TEST(Linux__GetAbsolutePath_ReturnsNonEmptyPath)
{
   const fs::path relativeFileOrDirectoryPath = ZenUnit::Random<fs::path>();
   _fileSystem.GetAbsolutePath(relativeFileOrDirectoryPath);

   _fileSystem.GetAbsolutePath(".");
}
#elif _WIN32
TEST(Windows__GetAbsolutePath_ReturnsResultOfCallingStdFilesystemAbsolute)
{
   const fs::path absoluteFileOrDirectoryPath = _call_fs_absoluteMock.ReturnRandom();
   const fs::path relativeFileOrDirectoryPath = ZenUnit::Random<fs::path>();
   //
   const fs::path returnedAbsoluteFileOrDirectoryPath = _fileSystem.GetAbsolutePath(relativeFileOrDirectoryPath);
   //
   METALMOCK(_call_fs_absoluteMock.CalledOnceWith(relativeFileOrDirectoryPath));
}
#endif

TEST(CurrentDirectoryPath_ReturnsResultOfCallingFilesystemCurrentPath)
{
   const fs::path currentPathReturnValue = _call_fs_current_pathMock.ReturnRandom();
   //
   const fs::path currentDirectoryPath = _fileSystem.CurrentDirectoryPath();
   //
   METALMOCK(_call_fs_current_pathMock.CalledOnce());
   ARE_EQUAL(currentPathReturnValue, currentDirectoryPath);
}

TEST(FileOrDirectoryExists_ReturnsResultOfCallingStdFilesystemExists)
{
   const bool existsReturnValue = _call_fs_existsMock.ReturnRandom();
   const fs::path fileOrDirectoryPath = ZenUnit::Random<fs::path>();
   //
   const bool fileOrDirectoryPathExists = _fileSystem.FileOrDirectoryExists(fileOrDirectoryPath);
   //
   METALMOCK(_call_fs_existsMock.CalledOnceWith(fileOrDirectoryPath));
   ARE_EQUAL(existsReturnValue, fileOrDirectoryPathExists);
}

// Writes

TEST(RenameFile_FilePathDoesNotExist_ThrowsRuntimeError)
{
   _caller_ExistsMock->CallConstMemberFunctionMock.Return(false);
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const string newFileName = ZenUnit::Random<string>();
   //
   const string expectedExceptionMessage =
      "FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) error: filePath does not exist: " + filePath.string();
   THROWS_EXCEPTION(const fs::path renamedFilePath = _fileSystem.RenameFile(filePath, newFileName),
      runtime_error, expectedExceptionMessage);
   //
   METALMOCK(_caller_ExistsMock->CallConstMemberFunctionMock.CalledOnceWith(
      &_fileSystem, &FileSystem::FileOrDirectoryExists, filePath));
}

TEST(RenameFile_FilePathExists_DestinationFilePathAlreadyExists_ThrowsRuntimeError)
{
   _caller_ExistsMock->CallConstMemberFunctionMock.ReturnValues(true, true);
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const string newFileName = ZenUnit::Random<string>();
   //
   const fs::path expectedSourceDirectoryPath = filePath.parent_path();
   const fs::path expectedDestinationFilePath = expectedSourceDirectoryPath / newFileName;
   const string expectedExceptionMessage =
      "FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) error: renamedFilePath already exists: " + expectedDestinationFilePath.string();
   THROWS_EXCEPTION(const fs::path renamedFilePath = _fileSystem.RenameFile(filePath, newFileName),
      runtime_error, expectedExceptionMessage);
   //
   METALMOCK(_caller_ExistsMock->CallConstMemberFunctionMock.CalledAsFollows(
   {
      { &_fileSystem, &FileSystem::FileOrDirectoryExists, filePath },
      { &_fileSystem, &FileSystem::FileOrDirectoryExists, expectedDestinationFilePath }
   }));
}

TEST2X2(RenameFile_FilePathExists_DestinationFilePathDoesNotExist_RenamesFile_ThrowsIfRenameReturnsNon0_OtherwiseReturns,
   int renameReturnValue, bool expectThrow,
   -1, true,
   0, false,
   1, true,
   10, true)
{
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const string newFileName = ZenUnit::Random<string>();

   _caller_ExistsMock->CallConstMemberFunctionMock.ReturnValues(true, false);

   const string filePathString = filePath.string();
   const char* const filePathStringCCP = filePathString.c_str();
   const string destinationFilePathString = ZenUnit::Random<string>();
   const char* const destinationFilePathStringCCP = destinationFilePathString.c_str();
   _constCharPointerGetterMock->GetStringConstCharPointerMock.ReturnValues(filePathStringCCP, destinationFilePathStringCCP);

   _call_std_renameMock.Return(renameReturnValue);
   //
   const fs::path expectedDirectoryPath = filePath.parent_path();
   const fs::path expectedRenamedFilePath = expectedDirectoryPath / newFileName;
   const string expectedFilePathString = filePath.string();
   const string expectedRenamedFilePathString = expectedRenamedFilePath.string();
   if (expectThrow)
   {
      const string expectedExceptionMessage = String::ConcatValues(
         "FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) error: std::rename(\"",
         filePathString, "\", \"", expectedRenamedFilePathString, "\") returned a non-0 value: ", renameReturnValue);
      THROWS_EXCEPTION(const fs::path renamedFilePath = _fileSystem.RenameFile(filePath, newFileName), runtime_error, expectedExceptionMessage);
   }
   else
   {
      const fs::path renamedFilePath = _fileSystem.RenameFile(filePath, newFileName);
      ARE_EQUAL(expectedRenamedFilePath, renamedFilePath);
   }
   //
   METALMOCK(_caller_ExistsMock->CallConstMemberFunctionMock.CalledAsFollows(
   {
      { &_fileSystem, &FileSystem::FileOrDirectoryExists, filePath },
      { &_fileSystem, &FileSystem::FileOrDirectoryExists, expectedRenamedFilePath }
   }));
   METALMOCK(_constCharPointerGetterMock->GetStringConstCharPointerMock.CalledAsFollows(
   {
      expectedFilePathString,
      expectedRenamedFilePathString
   }));
   METALMOCK(_call_std_renameMock.CalledOnceWith(filePathStringCCP, destinationFilePathStringCCP));
}

int _stdFileSystemRenameErrorCodeValue = 0;

void StdFileSystemRenameWithSettableErrorCode(
   const fs::path& /*directoryPath*/, const fs::path& /*destinationDirectoryPath*/, error_code& outErrorCode) const
{
   error_code errorCode(_stdFileSystemRenameErrorCodeValue, std::generic_category());
   outErrorCode = errorCode;
}

struct remove_all_CallHistory
{
   size_t numberOfCalls = 0;
   unsigned long long returnValue = 0;
   fs::path directoryPathArg;
   error_code outErrorCodeArg;
   error_code outErrorCodeReturnValue;
} _remove_all_CallHistory;

unsigned long long remove_all_CallInstead(const fs::path& directoryPath, error_code& outErrorCode)
{
   ++_remove_all_CallHistory.numberOfCalls;
   _remove_all_CallHistory.directoryPathArg = directoryPath;
   _remove_all_CallHistory.outErrorCodeArg = outErrorCode;
   outErrorCode = _remove_all_CallHistory.outErrorCodeReturnValue;
   return _remove_all_CallHistory.returnValue;
}

TEST(RenameDirectory_RenamesDirectory_FilesystemRenameReturns0_ReturnsRenamedDirectoryPath)
{
   _stdFileSystemRenameErrorCodeValue = 0;
   _call_fs_rename_with_error_codeMock.CallInstead(std::bind(&FileSystemTests::StdFileSystemRenameWithSettableErrorCode,
      this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   const string newDirectoryName = ZenUnit::Random<string>();
   //
   const fs::path renamedDirectoryPath = _fileSystem.RenameDirectory(directoryPath, newDirectoryName);
   //
   const fs::path expectedDirectoryPathMinusLeafDirectory = [&]()
   {
      return fs::path(directoryPath).remove_filename();
   }();
   const fs::path expectedRenamedDirectoryPath = expectedDirectoryPathMinusLeafDirectory / newDirectoryName;
   error_code expectedRenameErrorCodeArgument;
   METALMOCK(_call_fs_rename_with_error_codeMock.CalledOnceWith(directoryPath, expectedRenamedDirectoryPath, expectedRenameErrorCodeArgument));
   ARE_EQUAL(expectedRenamedDirectoryPath, renamedDirectoryPath);
}

TEST1X1(RenameDirectory_RenamesDirectory_FilesystemRenameReturnsNot0_ThrowsFileSystemException,
   int stdFileSystemRenameErrorCodeValue,
   -2,
   -1,
   1,
   2)
{
   _stdFileSystemRenameErrorCodeValue = stdFileSystemRenameErrorCodeValue;

   _call_fs_rename_with_error_codeMock.CallInstead(std::bind(&FileSystemTests::StdFileSystemRenameWithSettableErrorCode,
      this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

   const FileSystemException fileSystemException = _fileSystemExceptionMakerMock->MakeFileSystemExceptionForFailedToRenameDirectoryMock.ReturnRandom();

   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   const string newDirectoryName = ZenUnit::Random<string>();
   const fs::path expectedDirectoryPathMinusLeafDirectory = [&]()
   {
      return fs::path(directoryPath).remove_filename();
   }();
   const fs::path expectedRenamedDirectoryPath = expectedDirectoryPathMinusLeafDirectory / newDirectoryName;
   //
   THROWS_EXCEPTION(const fs::path renamedDirectoryPath = _fileSystem.RenameDirectory(directoryPath, newDirectoryName),
      FileSystemException, fileSystemException.what());
   //
   error_code expectedRenameArgumentErrorCode;
   METALMOCK(_call_fs_rename_with_error_codeMock.CalledOnceWith(directoryPath, expectedRenamedDirectoryPath, expectedRenameArgumentErrorCode));

   error_code expectedErrorCode(_stdFileSystemRenameErrorCodeValue, std::generic_category());
   METALMOCK(_fileSystemExceptionMakerMock->MakeFileSystemExceptionForFailedToRenameDirectoryMock.CalledOnceWith(
      directoryPath, expectedRenamedDirectoryPath, expectedErrorCode));
}

FILE* _fopen_ReturnValue = nullptr;
vector<pair<string, string>> _fopen_Arguments;
FILE* fopen_CallInstead(const char* filePath, const char* fileOpenMode)
{
   _fopen_Arguments.emplace_back(filePath, fileOpenMode);
   return _fopen_ReturnValue;
}

RUN_TESTS(FileSystemTests)
