#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Enums/FileExceptionType.h"
#include "libFileRevisorTests/Components/Exceptions/MetalMock/FileSystemExceptionMakerMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/DirectoryIteratorMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileOpenerCloserMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/RecursiveFileDeleterMock.h"
#include "libFileRevisorTests/Exceptions/ZenUnit/FileSystemExceptionRandom.h"
#include "libFileRevisorTests/UtilityComponents/FunctionCallers/Member/MetalMock/NonVoidOneArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/UtilityComponents/Strings/MetalMock/ConstCharPointerGetterMock.h"

TESTS(FileSystemTests)
AFACT(DefaultConstructor_NewsComponents_SetsFunctionPointers)
#if defined __linux__|| defined __APPLE__
AFACT(GetAbsolutePath_ReturnsNonEmptyPath)
#elif _WIN32
AFACT(GetAbsolutePath_ReturnsResultOfCallingStdFilesystemAbsolute)
#endif
AFACT(CurrentDirectoryPath_ReturnsResultOfCallingFilesystemCurrentPath)
AFACT(RecursivelyDeleteAllFilesInDirectory_CallsFileDeleterRecursivelyDeleteAllFilesInDirectory)
AFACT(FileOrDirectoryExists_ReturnsResultOfCallingStdFilesystemExists)
AFACT(RenameFile_FilePathDoesNotExist_ThrowsRuntimeError)
AFACT(RenameFile_FilePathExists_DestinationFilePathAlreadyExists_ThrowsRuntimeError)
FACTS(RenameFile_FilePathExists_DestinationFilePathDoesNotExist_RenamesFile_ThrowsIfRenameReturnsNon0_OtherwiseReturns)
AFACT(RemoveAll_FsRemoveAllReturns0ErrorCode_ReturnsNumberOfFilesAndDirectoriesRemoved)
FACTS(RemoveAll_FsRemoveAllReturnsNon0ErrorCode_ThrowsFileSystemException)
AFACT(RenameDirectory_RenamesDirectory_FilesystemRenameReturns0_ReturnsRenamedDirectoryPath)
FACTS(RenameDirectory_RenamesDirectory_FilesystemRenameReturnsNot0_ThrowsFileSystemException)
AFACT(OpenFile_FOpenReturnsNullFILEPointer_ThrowsFileSystemException)
AFACT(OpenFile_FOpenReturnsNonNullFILEPointer_ReturnsFilePointer)
#if defined __linux__|| defined __APPLE__
AFACT(RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_OnLinuxDoesNothing)
#elif _WIN32
AFACT(RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_CallsRemoveReadonlyFlagOnAllTopLevelFilesInDirectory)
#endif
EVIDENCE

FileSystem _fileSystem;
// Function Pointers
#ifdef _WIN32
METALMOCK_NONVOID1_FREE(fs::path, _call_std_filesystem_absolute, const fs::path&)
#endif
METALMOCK_NONVOID0_FREE(fs::path, _call_std_filesystem_current_path)
METALMOCK_NONVOID1_FREE(bool, exists, const fs::path&)
METALMOCK_NONVOID2_FREE(int, _call_std_rename, const char*, const char*)
METALMOCK_VOID3_FREE(_call_std_filesystem_rename_with_error_code, const fs::path&, const fs::path&, std::error_code&)
METALMOCK_NONVOID2_FREE(FILE*, fopen, const char*, const char*)
METALMOCK_NONVOID1_FREE(int, fclose, FILE*)
METALMOCK_NONVOID2_FREE(uintmax_t, _call_fs_remove_all, const fs::path&, error_code&)
// Function Callers
NonVoidOneArgMemberFunctionCallerMock<bool, FileSystem, const fs::path&>* _caller_ExistsMock = nullptr;
// Constant Components
ConstCharPointerGetterMock* _constCharPointerGetterMock = nullptr;
FileOpenerCloserMock* _fileOpenerCloserMock = nullptr;
FileSystemExceptionMakerMock* _fileSystemExceptionMakerMock = nullptr;
RecursiveFileDeleterMock* _recursiveFileDeleterMock = nullptr;

STARTUP
{
   // Function Pointers
   _fileSystem._call_fopen = BIND_2ARG_METALMOCK_OBJECT(fopenMock);
   _fileSystem._call_fclose = BIND_1ARG_METALMOCK_OBJECT(fcloseMock);
   _fileSystem._call_fs_remove_all = BIND_2ARG_METALMOCK_OBJECT(_call_fs_remove_allMock);
#ifdef _WIN32
   _fileSystem._call_std_filesystem_absolute = BIND_1ARG_METALMOCK_OBJECT(_call_std_filesystem_absoluteMock);
#endif
   _fileSystem._call_std_rename = BIND_2ARG_METALMOCK_OBJECT(_call_std_renameMock);
   _fileSystem._call_std_filesystem_rename_with_error_code = BIND_3ARG_METALMOCK_OBJECT(_call_std_filesystem_rename_with_error_codeMock);
   _fileSystem._call_std_filesystem_current_path = BIND_0ARG_METALMOCK_OBJECT(_call_std_filesystem_current_pathMock);
   _fileSystem._call_std_filesystem_exists = BIND_1ARG_METALMOCK_OBJECT(existsMock);
   // Function Callers
   _fileSystem._caller_Exists.reset(_caller_ExistsMock = new NonVoidOneArgMemberFunctionCallerMock<bool, FileSystem, const fs::path&>);
   // Constant Components
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
   STD_FUNCTION_TARGETS_OVERLOAD(
      fs_remove_all_FunctionType, fs::remove_all, fileSystem._call_fs_remove_all);

   using StdFilesystemAbsoluteFunctionType = fs::path(*)(const fs::path&);
   STD_FUNCTION_TARGETS_OVERLOAD(StdFilesystemAbsoluteFunctionType,
      fs::absolute, fileSystem._call_std_filesystem_absolute);

   using StdFilesystemCurrentPathFunctionType = fs::path(*)();
   STD_FUNCTION_TARGETS_OVERLOAD(StdFilesystemCurrentPathFunctionType,
      fs::current_path, fileSystem._call_std_filesystem_current_path);

   using StdFilesystemExistsFunctionType = bool(*)(const fs::path&);
   STD_FUNCTION_TARGETS_OVERLOAD(StdFilesystemExistsFunctionType,
      fs::exists, fileSystem._call_std_filesystem_exists);
#endif
   // Function Callers
   DELETE_TO_ASSERT_NEWED(fileSystem._caller_Exists);
   // Constant Components
   DELETE_TO_ASSERT_NEWED(fileSystem._constCharPointerGetter);
   DELETE_TO_ASSERT_NEWED(fileSystem._fileOpenerCloser);
   DELETE_TO_ASSERT_NEWED(fileSystem._fileSystemExceptionMaker);
   DELETE_TO_ASSERT_NEWED(fileSystem._recursiveFileDeleter);
}

#if defined __linux__|| defined __APPLE__
TEST(GetAbsolutePath_ReturnsNonEmptyPath)
{
   const fs::path relativeFileOrDirectoryPath = ZenUnit::Random<fs::path>();
   _fileSystem.GetAbsolutePath(relativeFileOrDirectoryPath);

   _fileSystem.GetAbsolutePath(".");
}
#elif _WIN32
TEST(GetAbsolutePath_ReturnsResultOfCallingStdFilesystemAbsolute)
{
   const fs::path absoluteFileOrDirectoryPath = _call_std_filesystem_absoluteMock.ReturnRandom();
   const fs::path relativeFileOrDirectoryPath = ZenUnit::Random<fs::path>();
   //
   const fs::path returnedAbsoluteFileOrDirectoryPath = _fileSystem.GetAbsolutePath(relativeFileOrDirectoryPath);
   //
   METALMOCK(_call_std_filesystem_absoluteMock.CalledOnceWith(relativeFileOrDirectoryPath));
}
#endif

TEST(CurrentDirectoryPath_ReturnsResultOfCallingFilesystemCurrentPath)
{
   const fs::path currentPathReturnValue = _call_std_filesystem_current_pathMock.ReturnRandom();
   //
   const fs::path currentDirectoryPath = _fileSystem.CurrentDirectoryPath();
   //
   METALMOCK(_call_std_filesystem_current_pathMock.CalledOnce());
   ARE_EQUAL(currentPathReturnValue, currentDirectoryPath);
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

TEST(FileOrDirectoryExists_ReturnsResultOfCallingStdFilesystemExists)
{
   const bool existsReturnValue = existsMock.ReturnRandom();
   const fs::path fileOrDirectoryPath = ZenUnit::Random<fs::path>();
   //
   const bool fileOrDirectoryPathExists = _fileSystem.FileOrDirectoryExists(fileOrDirectoryPath);
   //
   METALMOCK(existsMock.CalledOnceWith(fileOrDirectoryPath));
   ARE_EQUAL(existsReturnValue, fileOrDirectoryPathExists);
}

TEST(RenameFile_FilePathDoesNotExist_ThrowsRuntimeError)
{
   _caller_ExistsMock->ConstCallMock.Return(false);
   const fs::path filePath = ZenUnit::Random<fs::path>();
   const string newFileName = ZenUnit::Random<string>();
   //
   const string expectedExceptionMessage =
      "FileSystem::RenameFile(const fs::path& filePath, string_view newFileName) error: filePath does not exist: " + filePath.string();
   THROWS_EXCEPTION(const fs::path renamedFilePath = _fileSystem.RenameFile(filePath, newFileName),
      runtime_error, expectedExceptionMessage);
   //
   METALMOCK(_caller_ExistsMock->ConstCallMock.CalledOnceWith(
      &_fileSystem, &FileSystem::FileOrDirectoryExists, filePath));
}

TEST(RenameFile_FilePathExists_DestinationFilePathAlreadyExists_ThrowsRuntimeError)
{
   _caller_ExistsMock->ConstCallMock.ReturnValues(true, true);
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
   METALMOCK(_caller_ExistsMock->ConstCallMock.CalledAsFollows(
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

   _caller_ExistsMock->ConstCallMock.ReturnValues(true, false);

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
   METALMOCK(_caller_ExistsMock->ConstCallMock.CalledAsFollows(
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

void StdFileSystemRenameWithSettableErrorCode(const fs::path& /*directoryPath*/, const fs::path& /*destinationDirectoryPath*/, error_code& outErrorCode) const
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

TEST(RemoveAll_FsRemoveAllReturns0ErrorCode_ReturnsNumberOfFilesAndDirectoriesRemoved)
{
   _call_fs_remove_allMock.CallInstead(std::bind(&FileSystemTests::remove_all_CallInstead,
      this, std::placeholders::_1, std::placeholders::_2));
   _remove_all_CallHistory.returnValue = ZenUnit::Random<unsigned long long>();
   _remove_all_CallHistory.outErrorCodeReturnValue = error_code(0, std::generic_category());
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   //
   const unsigned long long numberOfFilesAndDirectoriesRemoved = _fileSystem.RemoveAll(directoryPath);
   //
   ARE_EQUAL(1, _remove_all_CallHistory.numberOfCalls);
   ARE_EQUAL(directoryPath, _remove_all_CallHistory.directoryPathArg);
   ARE_EQUAL(error_code(), _remove_all_CallHistory.outErrorCodeArg);
   ARE_EQUAL(_remove_all_CallHistory.returnValue, numberOfFilesAndDirectoriesRemoved);
}

TEST1X1(RemoveAll_FsRemoveAllReturnsNon0ErrorCode_ThrowsFileSystemException,
   int errorCodeValue,
   -1,
   1)
{
   _call_fs_remove_allMock.CallInstead(std::bind(&FileSystemTests::remove_all_CallInstead,
      this, std::placeholders::_1, std::placeholders::_2));
   _remove_all_CallHistory.returnValue = ZenUnit::Random<unsigned long long>();
   _remove_all_CallHistory.outErrorCodeReturnValue = error_code(errorCodeValue, std::generic_category());

   const FileSystemException fileSystemException = _fileSystemExceptionMakerMock->
      MakeFileSystemExceptionForRemoveAllFailedToDeleteDirectoryMock.ReturnRandom();

   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   //
   THROWS_EXCEPTION(_fileSystem.RemoveAll(directoryPath),
      FileSystemException, fileSystemException.what());
   //
   ARE_EQUAL(1, _remove_all_CallHistory.numberOfCalls);
   ARE_EQUAL(directoryPath, _remove_all_CallHistory.directoryPathArg);
   ARE_EQUAL(error_code(), _remove_all_CallHistory.outErrorCodeArg);

   METALMOCK(_fileSystemExceptionMakerMock->
      MakeFileSystemExceptionForRemoveAllFailedToDeleteDirectoryMock.CalledOnceWith(
      directoryPath, _remove_all_CallHistory.returnValue, errorCodeValue));
}

TEST(RenameDirectory_RenamesDirectory_FilesystemRenameReturns0_ReturnsRenamedDirectoryPath)
{
   _stdFileSystemRenameErrorCodeValue = 0;
   _call_std_filesystem_rename_with_error_codeMock.CallInstead(std::bind(&FileSystemTests::StdFileSystemRenameWithSettableErrorCode,
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
   METALMOCK(_call_std_filesystem_rename_with_error_codeMock.CalledOnceWith(directoryPath, expectedRenamedDirectoryPath, expectedRenameErrorCodeArgument));
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

   _call_std_filesystem_rename_with_error_codeMock.CallInstead(std::bind(&FileSystemTests::StdFileSystemRenameWithSettableErrorCode,
      this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

   const FileSystemException fileSystemException =
      _fileSystemExceptionMakerMock->MakeFileSystemExceptionForFailedToRenameDirectoryMock.ReturnRandom();

   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   const string newDirectoryName = ZenUnit::Random<string>();
   const fs::path expectedDirectoryPathMinusLeafDirectory = [&]()
   {
      return fs::path(directoryPath).remove_filename();
   }();
   const fs::path expectedRenamedDirectoryPath = expectedDirectoryPathMinusLeafDirectory / newDirectoryName;
   //
   THROWS_EXCEPTION(const fs::path renamedDirectoryPath =
      _fileSystem.RenameDirectory(directoryPath, newDirectoryName),
      FileSystemException, fileSystemException.what());
   //
   error_code expectedRenameArgumentErrorCode;
   METALMOCK(_call_std_filesystem_rename_with_error_codeMock.CalledOnceWith(
      directoryPath, expectedRenamedDirectoryPath, expectedRenameArgumentErrorCode));

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

TEST(OpenFile_FOpenReturnsNullFILEPointer_ThrowsFileSystemException)
{
   _fopen_ReturnValue = nullptr;
   fopenMock.CallInstead(std::bind(&FileSystemTests::fopen_CallInstead, this, placeholders::_1, placeholders::_2));

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
   fopenMock.CallInstead(std::bind(&FileSystemTests::fopen_CallInstead, this, placeholders::_1, placeholders::_2));

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

TEST(RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_OnLinuxDoesNothing)
{
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   _fileSystem.RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows(directoryPath);
}

#elif _WIN32

TEST(RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_CallsRemoveReadonlyFlagOnAllTopLevelFilesInDirectory)
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
   fileSystemSelfMocked.RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows(directoryPath);
   //
   METALMOCK(fileSystemSelfMocked.GetFilePathsInDirectoryMock.CalledOnceWith(directoryPath, false));
   METALMOCK(fileSystemSelfMocked.recursiveFileDeleterMock->RemoveReadonlyFlagFromFileSystemFilePathMock.CalledAsFollows(
   {
      topLevelFilePathsInDirectory[0],
      topLevelFilePathsInDirectory[1]
   }));
}

#endif

RUN_TESTS(FileSystemTests)
