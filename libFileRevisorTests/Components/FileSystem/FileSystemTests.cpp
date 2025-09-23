#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisorTests/Components/Exceptions/MetalMock/FileSystemExceptionMakerMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileOpenerCloserMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/RecursiveFileDeleterMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/MetalMock/NonVoidOneArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/MetalMock/NonVoidTwoArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/MetalMock/VoidOneArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/MetalMock/VoidTwoArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/Iteration/ForEach/MetalMock/FourArgMemberFunctionForEacherMock.h"
#include "libFileRevisorTests/Components/Iteration/ForEach/MetalMock/ParallelThreeArgMemberFunctionForEacherMock.h"
#include "libFileRevisorTests/Components/Strings/MetalMock/ConstCharPointerGetterMock.h"

TESTS(FileSystemTests)
AFACT(DefaultConstructor_SetsFieldsToDefaultValues)
// Deletes
AFACT(DeleteTopLevelFilesAndEmptyDirectoriesInDirectory_IfWindowsRemovesReadOnlyFlagsFromTopLevelFiles_SequentiallyDeletesTopLevelFiles_SequentiallyDeletesTopLevelEmptyDirectories)
AFACT(RecursivelyDeleteAllFilesInDirectory_CallsFileDeleterRecursivelyDeleteAllFilesInDirectory)
AFACT(DeleteFileOrDirectory_DryRunIsTrue_WritesWouldDeleteFilePathMessage_DoesNotDeleteFile)
AFACT(DeleteFileOrDirectory_DryRunIsFalse_QuietModeIsFalse_DeletesFileWhichDoesNotThrow_WritesDeletedFileMessage)
AFACT(DeleteFileOrDirectory_DryRunIsFalse_QuietModeIsTrue_DeletesFileWhichDoesNotThrow_DoesNotWriteDeletedFileMessage)
AFACT(DeleteFileOrDirectory_DryRunIsFalse_IgnoreFileDeleteErrorIsTrue_DeletesFileWhichThrows_WritesExceptionMessage_Returns)
AFACT(DeleteFileOrDirectory_DryRunIsFalse_IgnoreFileDeleteErrorIsFalse_DeletesFileWhichThrows_RethrowsException)
// Open File
AFACT(OpenFile_FOpenReturnsNullFILEPointer_ThrowsFileSystemException)
AFACT(OpenFile_FOpenReturnsNonNullFILEPointer_ReturnsFilePointer)
#if defined __linux__
AFACT(Linux__RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_DoesNothing)
#elif _WIN32
AFACT(Windows__RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_DryRunIsTrue_DoesNothing)
AFACT(Windows__RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_DryRunIsFalse_CallsRemoveReadonlyFlagOnAllTopLevelFilesInDirectory)
#endif
// Queries
#if defined __linux__
AFACT(Linux__GetAbsolutePath_ReturnsNonEmptyPath)
#elif _WIN32
AFACT(Windows__GetAbsolutePath_ReturnsResultOfCallingStdFilesystemAbsolute)
#endif
AFACT(CurrentFolderPath_ReturnsResultOfCallingFilesystemCurrentPath)
AFACT(FileOrDirectoryExists_ReturnsResultOfCallingStdFilesystemExists)
// Writes
AFACT(RenameFile_FilePathDoesNotExist_ThrowsRuntimeError)
AFACT(RenameFile_FilePathExists_DestinationFilePathExists_ThrowsRuntimeError)
FACTS(RenameFile_FilePathExists_DestinationFilePathDoesNotExist_RenamesFile_ThrowsIfRenameReturnsNon0_OtherwiseReturns)
AFACT(RenameDirectory_RenamesDirectory_FilesystemRenameReturns0_ReturnsRenamedFolderPath)
FACTS(RenameDirectory_RenamesDirectory_FilesystemRenameReturnsNot0_ThrowsFileSystemException)
// Private Functions
AFACT(DoDeleteFileOrDirectory_FileOrFolderPathIsFolderPath_CallsRemoveAll)
AFACT(DoDeleteFileOrDirectory_FileOrFolderPathIsFilePath_CallsRemove)
EVIDENCE

FileSystem p_fileSystem;
// Function Pointers
METALMOCK_NONVOID1_STATIC_OR_FREE(int, _call_fclose, FILE*)
METALMOCK_NONVOID2_STATIC_OR_FREE(FILE*, _call_fopen, const char*, const char*)
METALMOCK_NONVOID1_STATIC_OR_FREE(bool, _call_fs_is_directory, const fs::path&)
METALMOCK_NONVOID1_STATIC_OR_FREE(bool, _call_fs_remove, const fs::path&)
METALMOCK_NONVOID1_STATIC_OR_FREE(uintmax_t, _call_fs_remove_all, const fs::path&)
#ifdef _WIN32
METALMOCK_NONVOID1_STATIC_OR_FREE(fs::path, _call_fs_absolute, const fs::path&)
#endif
METALMOCK_NONVOID0_STATIC_OR_FREE(fs::path, _call_fs_current_path)
METALMOCK_NONVOID1_STATIC_OR_FREE(bool, _call_fs_exists, const fs::path&)
METALMOCK_NONVOID2_STATIC_OR_FREE(int, _call_std_rename, const char*, const char*)
METALMOCK_VOID3_STATIC_OR_FREE(_call_fs_rename_with_error_code, const fs::path&, const fs::path&, std::error_code&)
// Function Callers
using _caller_DeleteFileOrDirectoryMockType = VoidTwoArgMemberFunctionCallerMock<FileSystem, const fs::path&, bool>;
_caller_DeleteFileOrDirectoryMockType* _caller_DeleteFileOrDirectoryMock = nullptr;

using _caller_DoDeleteFileOrDirectoryMockType = VoidOneArgMemberFunctionCallerMock<FileSystem, const fs::path&>;
_caller_DoDeleteFileOrDirectoryMockType* _caller_DoDeleteFileOrDirectoryMock = nullptr;

using _caller_FileSystem_ExistsMockType = NonVoidOneArgMemberFunctionCallerMock<bool, FileSystem, const fs::path&>;
_caller_FileSystem_ExistsMockType* _caller_FileSystem_ExistsMock = nullptr;

using _caller_GetFileOrFolderPathsInDirectoryMockType = NonVoidTwoArgMemberFunctionCallerMock<vector<fs::path>, FileSystem, const fs::path&, bool>;
_caller_GetFileOrFolderPathsInDirectoryMockType* _caller_GetFileOrFolderPathsInDirectoryMock = nullptr;

using _foreacher_DeleteFileOrDirectoryMockType = Utils::FourArgMemberFunctionForEacherMock<FileSystem, fs::path, bool, bool, bool>;
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
   p_fileSystem._call_fclose = BIND_1ARG_METALMOCK_OBJECT(_call_fcloseMock);
   p_fileSystem._call_fopen = BIND_2ARG_METALMOCK_OBJECT(_call_fopenMock);
   p_fileSystem._call_fs_is_directory = BIND_1ARG_METALMOCK_OBJECT(_call_fs_is_directoryMock);
   p_fileSystem._call_fs_remove = BIND_1ARG_METALMOCK_OBJECT(_call_fs_removeMock);
   p_fileSystem._call_fs_remove_all = BIND_1ARG_METALMOCK_OBJECT(_call_fs_remove_allMock);
#ifdef _WIN32
   p_fileSystem._call_fs_absolute = BIND_1ARG_METALMOCK_OBJECT(_call_fs_absoluteMock);
#endif
   p_fileSystem._call_std_rename = BIND_2ARG_METALMOCK_OBJECT(_call_std_renameMock);
   p_fileSystem._call_fs_rename_with_error_code = BIND_3ARG_METALMOCK_OBJECT(_call_fs_rename_with_error_codeMock);
   p_fileSystem._call_fs_current_path = BIND_0ARG_METALMOCK_OBJECT(_call_fs_current_pathMock);
   p_fileSystem._call_fs_exists = BIND_1ARG_METALMOCK_OBJECT(_call_fs_existsMock);
   // Function Callers
   p_fileSystem._caller_DeleteFileOrDirectory.reset(_caller_DeleteFileOrDirectoryMock = new _caller_DeleteFileOrDirectoryMockType);
   p_fileSystem._caller_DoDeleteFileOrDirectory.reset(_caller_DoDeleteFileOrDirectoryMock = new _caller_DoDeleteFileOrDirectoryMockType);
   p_fileSystem._caller_FileSystem_Exists.reset(_caller_FileSystem_ExistsMock = new _caller_FileSystem_ExistsMockType);
   p_fileSystem._caller_GetFileOrFolderPathsInDirectory.reset(_caller_GetFileOrFolderPathsInDirectoryMock = new _caller_GetFileOrFolderPathsInDirectoryMockType);
   p_fileSystem._foreacher_DeleteFileOrDirectory.reset(_foreacher_DeleteFileOrDirectoryMock = new _foreacher_DeleteFileOrDirectoryMockType);
   // Constant Components
   p_fileSystem._console.reset(_consoleMock = new ConsoleMock);
   p_fileSystem._constCharPointerGetter.reset(_constCharPointerGetterMock = new ConstCharPointerGetterMock);
   p_fileSystem._fileOpenerCloser.reset(_fileOpenerCloserMock = new FileOpenerCloserMock);
   p_fileSystem._fileSystemExceptionMaker.reset(_fileSystemExceptionMakerMock = new FileSystemExceptionMakerMock);
   p_fileSystem._recursiveFileDeleter.reset(_recursiveFileDeleterMock = new RecursiveFileDeleterMock);
}

TEST(DefaultConstructor_SetsFieldsToDefaultValues)
{
   const FileSystem fileSystem;
   // Function Pointers
   STD_FUNCTION_TARGETS(fopen, fileSystem._call_fopen);
   STD_FUNCTION_TARGETS(fclose, fileSystem._call_fclose);

   STD_FUNCTION_TARGETS(std::rename, fileSystem._call_std_rename);
#ifdef _WIN32
   using fs_is_directory_FunctionType = bool(*)(const fs::path&);
   STD_FUNCTION_TARGETS_OVERLOAD(fs_is_directory_FunctionType, fs::is_directory, fileSystem._call_fs_is_directory);

   using fs_remove_all_FunctionType = uintmax_t(*)(const fs::path&);
   STD_FUNCTION_TARGETS_OVERLOAD(fs_remove_all_FunctionType, fs::remove_all, fileSystem._call_fs_remove_all);

   using StdFilesystemAbsoluteFunctionType = fs::path(*)(const fs::path&);
   STD_FUNCTION_TARGETS_OVERLOAD(StdFilesystemAbsoluteFunctionType, fs::absolute, fileSystem._call_fs_absolute);

   using StdFilesystemCurrentPathFunctionType = fs::path(*)();
   STD_FUNCTION_TARGETS_OVERLOAD(StdFilesystemCurrentPathFunctionType, fs::current_path, fileSystem._call_fs_current_path);

   using StdFilesystemExistsFunctionType = bool(*)(const fs::path&);
   STD_FUNCTION_TARGETS_OVERLOAD(StdFilesystemExistsFunctionType, fs::exists, fileSystem._call_fs_exists);
#endif
}

// Deletes

TEST(DeleteTopLevelFilesAndEmptyDirectoriesInDirectory_IfWindowsRemovesReadOnlyFlagsFromTopLevelFiles_SequentiallyDeletesTopLevelFiles_SequentiallyDeletesTopLevelEmptyDirectories)
{
   _caller_DeleteFileOrDirectoryMock->CallConstMemberFunctionMock.Expect();

   const vector<fs::path> topLevelFolderPaths = ZenUnit::RandomVector<fs::path>();
   const vector<fs::path> topLevelFilePaths = ZenUnit::RandomVector<fs::path>();
   _caller_GetFileOrFolderPathsInDirectoryMock->CallConstMemberFunctionMock.ReturnValues(topLevelFolderPaths, topLevelFilePaths);

   _foreacher_DeleteFileOrDirectoryMock->CallConstMemberFunctionWithEachElementMock.Expect();

   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   const bool skipFilesInUse = ZenUnit::Random<bool>();
   const bool dryRun = ZenUnit::Random<bool>();
   const bool quietMode = ZenUnit::Random<bool>();
   //
   p_fileSystem.DeleteTopLevelFilesAndEmptyDirectoriesInDirectory(directoryPath, skipFilesInUse, dryRun, quietMode);
   //
   METALMOCK(_caller_GetFileOrFolderPathsInDirectoryMock->CallConstMemberFunctionMock.CalledNTimes(2));
   METALMOCK(_foreacher_DeleteFileOrDirectoryMock->CallConstMemberFunctionWithEachElementMock.CalledNTimes(2));

   METALMOCKTHEN(_caller_DeleteFileOrDirectoryMock->CallConstMemberFunctionMock.CalledOnceWith(&p_fileSystem, &FileSystem::RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows, directoryPath, dryRun)).Then(

   METALMOCKTHEN(_caller_GetFileOrFolderPathsInDirectoryMock->CallConstMemberFunctionMock.CalledWith(
      &p_fileSystem, &FileSystem::GetFolderPathsInDirectory, directoryPath, false))).Then(

   METALMOCKTHEN(_caller_GetFileOrFolderPathsInDirectoryMock->CallConstMemberFunctionMock.CalledWith(
      &p_fileSystem, &FileSystem::GetFilePathsInDirectory, directoryPath, false))).Then(

   METALMOCKTHEN(_foreacher_DeleteFileOrDirectoryMock->CallConstMemberFunctionWithEachElementMock.CalledWith(
      topLevelFolderPaths, &p_fileSystem, &FileSystem::DeleteFileOrDirectory, skipFilesInUse, dryRun, quietMode))).Then(

   METALMOCKTHEN(_foreacher_DeleteFileOrDirectoryMock->CallConstMemberFunctionWithEachElementMock.CalledWith(
      topLevelFilePaths, &p_fileSystem, &FileSystem::DeleteFileOrDirectory, skipFilesInUse, dryRun, quietMode)));
}

TEST(RecursivelyDeleteAllFilesInDirectory_CallsFileDeleterRecursivelyDeleteAllFilesInDirectory)
{
   _recursiveFileDeleterMock->RecursivelyDeleteAllFilesInDirectoryMock.Expect();
   const string directoryPath = ZenUnit::Random<string>();
   const FileRevisorArgs args = ZenUnit::Random<FileRevisorArgs>();
   //
   p_fileSystem.RecursivelyDeleteAllFilesInDirectory(directoryPath, args);
   //
   METALMOCK(_recursiveFileDeleterMock->RecursivelyDeleteAllFilesInDirectoryMock.CalledOnceWith(directoryPath.c_str(), args));
}

TEST(DeleteFileOrDirectory_DryRunIsTrue_WritesWouldDeleteFilePathMessage_DoesNotDeleteFile)
{
   _consoleMock->ProgramNameThreadIdWriteLineMock.Expect();
   const fs::path fileOrFolderPath = ZenUnit::Random<fs::path>();
   const bool ignoreFileDeleteError = ZenUnit::Random<bool>();
   const bool dryRun = true;
   const bool quietMode = ZenUnit::Random<bool>();
   //
   p_fileSystem.DeleteFileOrDirectory(fileOrFolderPath, ignoreFileDeleteError, dryRun, quietMode);
   //
   const string expectedWouldDeleteMessage = "DryRun: Would delete " + fileOrFolderPath.string();
   METALMOCK(_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(expectedWouldDeleteMessage));
}

TEST(DeleteFileOrDirectory_DryRunIsFalse_QuietModeIsFalse_DeletesFileWhichDoesNotThrow_WritesDeletedFileMessage)
{
   _caller_DoDeleteFileOrDirectoryMock->CallConstMemberFunctionMock.Expect();
   _consoleMock->ProgramNameThreadIdWriteLineMock.Expect();
   const fs::path fileOrFolderPath = ZenUnit::Random<fs::path>();
   const bool ignoreFileDeleteError = ZenUnit::Random<bool>();
   const bool dryRun = false;
   const bool quietMode = false;
   //
   p_fileSystem.DeleteFileOrDirectory(fileOrFolderPath, ignoreFileDeleteError, dryRun, quietMode);
   //
   const string expectedDeletedMessage = "Deleted " + fileOrFolderPath.string();
   METALMOCKTHEN(_caller_DoDeleteFileOrDirectoryMock->CallConstMemberFunctionMock.CalledOnceWith(
      &p_fileSystem, &FileSystem::DoDeleteFileOrDirectory, fileOrFolderPath)).Then(
   METALMOCKTHEN(_consoleMock->ProgramNameThreadIdWriteLineMock.CalledOnceWith(expectedDeletedMessage)));
}

TEST(DeleteFileOrDirectory_DryRunIsFalse_QuietModeIsTrue_DeletesFileWhichDoesNotThrow_DoesNotWriteDeletedFileMessage)
{
   _caller_DoDeleteFileOrDirectoryMock->CallConstMemberFunctionMock.Expect();
   const fs::path fileOrFolderPath = ZenUnit::Random<fs::path>();
   const bool ignoreFileDeleteError = ZenUnit::Random<bool>();
   const bool dryRun = false;
   const bool quietMode = true;
   //
   p_fileSystem.DeleteFileOrDirectory(fileOrFolderPath, ignoreFileDeleteError, dryRun, quietMode);
   //
   METALMOCKTHEN(_caller_DoDeleteFileOrDirectoryMock->CallConstMemberFunctionMock.CalledOnceWith(
      &p_fileSystem, &FileSystem::DoDeleteFileOrDirectory, fileOrFolderPath));
}

TEST(DeleteFileOrDirectory_DryRunIsFalse_IgnoreFileDeleteErrorIsTrue_DeletesFileWhichThrows_WritesExceptionMessage_Returns)
{
   const string exceptionMessage = ZenUnit::Random<string>();
   _caller_DoDeleteFileOrDirectoryMock->CallConstMemberFunctionMock.ThrowExceptionWhenCalled<runtime_error>(exceptionMessage);
   _consoleMock->ProgramNameThreadIdWriteLineColorMock.Expect();
   const fs::path fileOrFolderPath = ZenUnit::Random<fs::path>();
   const bool ignoreFileDeleteError = true;
   const bool dryRun = false;
   const bool quietMode = ZenUnit::Random<bool>();
   //
   p_fileSystem.DeleteFileOrDirectory(fileOrFolderPath, ignoreFileDeleteError, dryRun, quietMode);
   //
   const string expectedExceptionClassNameAndMessage = "std::runtime_error: " + exceptionMessage;
   const string expectedIgnoringExceptionMessage = "Ignoring exception because --skip-files-in-use: " + expectedExceptionClassNameAndMessage;
   METALMOCKTHEN(_caller_DoDeleteFileOrDirectoryMock->CallConstMemberFunctionMock.CalledOnceWith(
      &p_fileSystem, &FileSystem::DoDeleteFileOrDirectory, fileOrFolderPath)).Then(
   METALMOCKTHEN(_consoleMock->ProgramNameThreadIdWriteLineColorMock.CalledOnceWith(expectedIgnoringExceptionMessage, Color::Yellow)));
}

TEST(DeleteFileOrDirectory_DryRunIsFalse_IgnoreFileDeleteErrorIsFalse_DeletesFileWhichThrows_RethrowsException)
{
   const string exceptionMessage = ZenUnit::Random<string>();
   _caller_DoDeleteFileOrDirectoryMock->CallConstMemberFunctionMock.ThrowExceptionWhenCalled<runtime_error>(exceptionMessage);
   const fs::path fileOrFolderPath = ZenUnit::Random<fs::path>();
   const bool quietMode = ZenUnit::Random<bool>();
   //
   THROWS_EXCEPTION(p_fileSystem.DeleteFileOrDirectory(fileOrFolderPath, false, false, quietMode),
      runtime_error, exceptionMessage);
   //
   METALMOCKTHEN(_caller_DoDeleteFileOrDirectoryMock->CallConstMemberFunctionMock.CalledOnceWith(
      &p_fileSystem, &FileSystem::DoDeleteFileOrDirectory, fileOrFolderPath));
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
   THROWS_EXCEPTION(p_fileSystem.OpenFile(filePath, fileOpenMode.c_str()),
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
   const shared_ptr<FILE> filePointer = p_fileSystem.OpenFile(filePath, fileOpenMode.c_str());
   //
   const vector<pair<string, string>> expected_fopen_Arguments = { { filePath.string(), fileOpenMode } };
   ARE_EQUAL(expected_fopen_Arguments, _fopen_Arguments);
   ARE_EQUAL(rawFilePointer, filePointer.get());
}

#if defined __linux__

TEST(Linux__RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_DoesNothing)
{
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   const bool dryRun = ZenUnit::Random<bool>();
   p_fileSystem.RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows(directoryPath, dryRun);
}

#elif _WIN32

TEST(Windows__RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_DryRunIsTrue_DoesNothing)
{
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   p_fileSystem.RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows(directoryPath, true);
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
   METALMOCK(fileSystemSelfMocked.recursiveFileDeleterMock->RemoveReadonlyFlagFromFileSystemFilePathMock.CalledNTimes(2));
   METALMOCKTHEN(fileSystemSelfMocked.GetFilePathsInDirectoryMock.CalledOnceWith(directoryPath, false)).Then(
   METALMOCKTHEN(fileSystemSelfMocked.recursiveFileDeleterMock->RemoveReadonlyFlagFromFileSystemFilePathMock.CalledWith(
      topLevelFilePathsInDirectory[0]))).Then(
   METALMOCKTHEN(fileSystemSelfMocked.recursiveFileDeleterMock->RemoveReadonlyFlagFromFileSystemFilePathMock.CalledWith(
      topLevelFilePathsInDirectory[1])));
}

#endif

// Queries

#if defined __linux__
TEST(Linux__GetAbsolutePath_ReturnsNonEmptyPath)
{
   const fs::path relativeFileOrFolderPath = ZenUnit::Random<fs::path>();
   p_fileSystem.GetAbsolutePath(relativeFileOrFolderPath);

   p_fileSystem.GetAbsolutePath(".");
}
#elif _WIN32
TEST(Windows__GetAbsolutePath_ReturnsResultOfCallingStdFilesystemAbsolute)
{
   const fs::path absoluteFileOrFolderPath = _call_fs_absoluteMock.ReturnRandom();
   const fs::path relativeFileOrFolderPath = ZenUnit::Random<fs::path>();
   //
   const fs::path returnedAbsoluteFileOrFolderPath = p_fileSystem.GetAbsolutePath(relativeFileOrFolderPath);
   //
   METALMOCK(_call_fs_absoluteMock.CalledOnceWith(relativeFileOrFolderPath));
}
#endif

TEST(CurrentFolderPath_ReturnsResultOfCallingFilesystemCurrentPath)
{
   const fs::path currentPathReturnValue = _call_fs_current_pathMock.ReturnRandom();
   //
   const fs::path currentFolderPath = p_fileSystem.CurrentFolderPath();
   //
   METALMOCK(_call_fs_current_pathMock.CalledOnce());
   ARE_EQUAL(currentPathReturnValue, currentFolderPath);
}

TEST(FileOrDirectoryExists_ReturnsResultOfCallingStdFilesystemExists)
{
   const bool existsReturnValue = _call_fs_existsMock.ReturnRandom();
   const fs::path fileOrFolderPath = ZenUnit::Random<fs::path>();
   //
   const bool fileOrFolderPathExists = p_fileSystem.FileOrDirectoryExists(fileOrFolderPath);
   //
   METALMOCK(_call_fs_existsMock.CalledOnceWith(fileOrFolderPath));
   ARE_EQUAL(existsReturnValue, fileOrFolderPathExists);
}

// Writes

TEST(RenameFile_FilePathDoesNotExist_ThrowsRuntimeError)
{
   _caller_FileSystem_ExistsMock->CallConstMemberFunctionMock.Return(false);
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const string newFileName = ZenUnit::Random<string>();
   //
   const string expectedExceptionMessage =
      "FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) error: filePath does not exist: " + filePath.string();
   THROWS_EXCEPTION(const fs::path renamedFilePath = p_fileSystem.RenameFile(filePath, newFileName),
      runtime_error, expectedExceptionMessage);
   //
   METALMOCK(_caller_FileSystem_ExistsMock->CallConstMemberFunctionMock.CalledOnceWith(
      &p_fileSystem, &FileSystem::FileOrDirectoryExists, filePath));
}

TEST(RenameFile_FilePathExists_DestinationFilePathExists_ThrowsRuntimeError)
{
   _caller_FileSystem_ExistsMock->CallConstMemberFunctionMock.ReturnValues(true, true);
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const string newFileName = ZenUnit::Random<string>();
   //
   const fs::path expectedSourceFolderPath = filePath.parent_path();
   const fs::path expectedDestinationFilePath = expectedSourceFolderPath / newFileName;
   const string expectedExceptionMessage =
      "FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) error: renamedFilePath already exists: " + expectedDestinationFilePath.string();
   THROWS_EXCEPTION(const fs::path renamedFilePath = p_fileSystem.RenameFile(filePath, newFileName),
      runtime_error, expectedExceptionMessage);
   //
   METALMOCK(_caller_FileSystem_ExistsMock->CallConstMemberFunctionMock.CalledNTimes(2));
   METALMOCKTHEN(_caller_FileSystem_ExistsMock->CallConstMemberFunctionMock.CalledWith(
      &p_fileSystem, &FileSystem::FileOrDirectoryExists, filePath)).Then(
   METALMOCKTHEN(_caller_FileSystem_ExistsMock->CallConstMemberFunctionMock.CalledWith(
      &p_fileSystem, &FileSystem::FileOrDirectoryExists, expectedDestinationFilePath)));
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

   _caller_FileSystem_ExistsMock->CallConstMemberFunctionMock.ReturnValues(true, false);

   const string filePathString = filePath.string();
   const char* const filePathStringCCP = filePathString.c_str();
   const string destinationFilePathString = ZenUnit::Random<string>();
   const char* const destinationFilePathStringCCP = destinationFilePathString.c_str();
   _constCharPointerGetterMock->GetStringConstCharPointerMock.ReturnValues(filePathStringCCP, destinationFilePathStringCCP);

   _call_std_renameMock.Return(renameReturnValue);
   //
   const fs::path expectedFolderPath = filePath.parent_path();
   const fs::path expectedRenamedFilePath = expectedFolderPath / newFileName;
   const string expectedFilePathString = filePath.string();
   const string expectedRenamedFilePathString = expectedRenamedFilePath.string();
   if (expectThrow)
   {
      const string expectedExceptionMessage = String::ConcatValues(
         "FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) error: std::rename(\"",
         filePathString, "\", \"", expectedRenamedFilePathString, "\") returned a non-0 value: ", renameReturnValue);
      THROWS_EXCEPTION(const fs::path renamedFilePath = p_fileSystem.RenameFile(filePath, newFileName), runtime_error, expectedExceptionMessage);
   }
   else
   {
      const fs::path renamedFilePath = p_fileSystem.RenameFile(filePath, newFileName);
      ARE_EQUAL(expectedRenamedFilePath, renamedFilePath);
   }
   //
   METALMOCK(_caller_FileSystem_ExistsMock->CallConstMemberFunctionMock.CalledNTimes(2));
   METALMOCK(_constCharPointerGetterMock->GetStringConstCharPointerMock.CalledNTimes(2));
   METALMOCKTHEN(_caller_FileSystem_ExistsMock->CallConstMemberFunctionMock.CalledWith(&p_fileSystem, &FileSystem::FileOrDirectoryExists, filePath)).Then(
   METALMOCKTHEN(_caller_FileSystem_ExistsMock->CallConstMemberFunctionMock.CalledWith(&p_fileSystem, &FileSystem::FileOrDirectoryExists, expectedRenamedFilePath))).Then(
   METALMOCKTHEN(_constCharPointerGetterMock->GetStringConstCharPointerMock.CalledWith(expectedFilePathString))).Then(
   METALMOCKTHEN(_constCharPointerGetterMock->GetStringConstCharPointerMock.CalledWith(expectedRenamedFilePathString))).Then(
   METALMOCKTHEN(_call_std_renameMock.CalledOnceWith(filePathStringCCP, destinationFilePathStringCCP)));
}

int _stdFileSystemRenameErrorCodeValue = 0;

void StdFileSystemRenameWithSettableErrorCode(
   const fs::path& /*directoryPath*/, const fs::path& /*destinationFolderPath*/, error_code& outErrorCode) const
{
   error_code errorCode(_stdFileSystemRenameErrorCodeValue, std::generic_category());
   outErrorCode = errorCode;
}

TEST(RenameDirectory_RenamesDirectory_FilesystemRenameReturns0_ReturnsRenamedFolderPath)
{
   _stdFileSystemRenameErrorCodeValue = 0;
   _call_fs_rename_with_error_codeMock.CallInstead(std::bind(&FileSystemTests::StdFileSystemRenameWithSettableErrorCode,
      this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   const string newDirectoryName = ZenUnit::Random<string>();
   //
   const fs::path renamedFolderPath = p_fileSystem.RenameDirectory(directoryPath, newDirectoryName);
   //
   const fs::path expectedFolderPathMinusLeafDirectory = [&]() // LCOV_EXCL_LINE
   {
      return fs::path(directoryPath).remove_filename();
   }();
   const fs::path expectedRenamedFolderPath = expectedFolderPathMinusLeafDirectory / newDirectoryName;
   error_code expectedRenameErrorCodeArgument;
   METALMOCK(_call_fs_rename_with_error_codeMock.CalledOnceWith(directoryPath, expectedRenamedFolderPath, expectedRenameErrorCodeArgument));
   ARE_EQUAL(expectedRenamedFolderPath, renamedFolderPath);
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
   const fs::path expectedFolderPathMinusLeafDirectory = [&]() // LCOV_EXCL_LINE
   {
      return fs::path(directoryPath).remove_filename();
   }();
   const fs::path expectedRenamedFolderPath = expectedFolderPathMinusLeafDirectory / newDirectoryName;
   //
   THROWS_EXCEPTION(const fs::path renamedFolderPath = p_fileSystem.RenameDirectory(directoryPath, newDirectoryName),
      FileSystemException, fileSystemException.what());
   //
   error_code expectedRenameArgumentErrorCode;
   METALMOCK(_call_fs_rename_with_error_codeMock.CalledOnceWith(directoryPath, expectedRenamedFolderPath, expectedRenameArgumentErrorCode));

   error_code expectedErrorCode(_stdFileSystemRenameErrorCodeValue, std::generic_category());
   METALMOCK(_fileSystemExceptionMakerMock->MakeFileSystemExceptionForFailedToRenameDirectoryMock.CalledOnceWith(
      directoryPath, expectedRenamedFolderPath, expectedErrorCode));
}

FILE* _fopen_ReturnValue = nullptr;
vector<pair<string, string>> _fopen_Arguments;
FILE* fopen_CallInstead(const char* filePath, const char* fileOpenMode)
{
   _fopen_Arguments.emplace_back(filePath, fileOpenMode);
   return _fopen_ReturnValue;
}

// Private Functions

TEST(DoDeleteFileOrDirectory_FileOrFolderPathIsFolderPath_CallsRemoveAll)
{
   _call_fs_is_directoryMock.Return(true);
   _call_fs_remove_allMock.ReturnRandom();
   const fs::path fileOrFolderPath = ZenUnit::Random<fs::path>();
   //
   p_fileSystem.DoDeleteFileOrDirectory(fileOrFolderPath);
   //
   METALMOCK(_call_fs_is_directoryMock.CalledOnceWith(fileOrFolderPath));
   METALMOCK(_call_fs_remove_allMock.CalledOnceWith(fileOrFolderPath));
}

TEST(DoDeleteFileOrDirectory_FileOrFolderPathIsFilePath_CallsRemove)
{
   _call_fs_is_directoryMock.Return(false);
   _call_fs_removeMock.ReturnRandom();
   const fs::path fileOrFolderPath = ZenUnit::Random<fs::path>();
   //
   p_fileSystem.DoDeleteFileOrDirectory(fileOrFolderPath);
   //
   METALMOCK(_call_fs_is_directoryMock.CalledOnceWith(fileOrFolderPath));
   METALMOCK(_call_fs_removeMock.CalledOnceWith(fileOrFolderPath));
}

RUN_TESTS(FileSystemTests)
