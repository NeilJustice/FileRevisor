#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Enums/FileExceptionType.h"
#include "libFileRevisorTests/Components/Exceptions/MetalMock/FileSystemExceptionMakerMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/RecursiveFileDeleterMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/MetalMock/NonVoidOneArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/Strings/MetalMock/ConstCharPointerGetterMock.h"
#include "libFileRevisorTests/Exceptions/ZenUnit/FileSystemExceptionRandom.h"

TESTS(FileSystemTests)
AFACT(DefaultConstructor_NewsComponents_SetsFunctionPointers)
#ifdef __linux__
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
AFACT(RemoveAll_FsRemoveAllReturnsWithSettingErrorCodeTo0_ReturnsNumberOfFilesAndDirectoriesRemoved)
FACTS(RemoveAll_FsRemoveAllReturnsWithSettingErrorCodeToNon0_ThrowsFileSystemException)
AFACT(RenameDirectory_RenamesDirectory_FilesystemRenameReturns0_ReturnsRenamedDirectoryPath)
FACTS(RenameDirectory_RenamesDirectory_FilesystemRenameReturnsNot0_ThrowsFileSystemException)
AFACT(OpenFile_FOpenReturnsNullFILEPointer_ThrowsFileSystemException)
AFACT(OpenFile_FOpenReturnsNonNullFILEPointer_ReturnsFilePointer)
FACTS(CloseFile_fcloseReturnValueIsNot0_ThrowsFileCloseException)
AFACT(CloseFile_fcloseReturnValueIs0_Returns)
#ifdef __linux__
AFACT(RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_OnLinuxDoesNothing)
#elif _WIN32
AFACT(RemoveReadOnlyFlagsFromTopLevelFilesInDirectoryIfWindows_CallsRemoveReadonlyFlagOnAllTopLevelFilesInDirectory)
#endif
EVIDENCE

FileSystem _fileSystem;
ConstCharPointerGetterMock* _constCharPointerGetterMock = nullptr;
FileSystemExceptionMakerMock* _fileSystemExceptionThrowerMock = nullptr;
NonVoidOneArgMemberFunctionCallerMock<bool, FileSystem, const fs::path&>* _caller_ExistsMock = nullptr;
RecursiveFileDeleterMock* _recursiveFileDeleterMock = nullptr;
#ifdef _WIN32
METALMOCK_NONVOID1_NAMESPACED_FREE(fs::path, std::filesystem, absolute, const fs::path&)
#endif
METALMOCK_NONVOID0_NAMESPACED_FREE(fs::path, std::filesystem, current_path)
METALMOCK_NONVOID1_FREE(bool, exists, const fs::path&)
METALMOCK_NONVOID2_NAMESPACED_FREE(int, std, rename, const char*, const char*)
METALMOCK_VOID3_NAMESPACED_FREE(std::filesystem, rename, const fs::path&, const fs::path&, std::error_code&, _fs)
METALMOCK_NONVOID2_FREE(FILE*, fopen, const char*, const char*)
METALMOCK_NONVOID1_FREE(int, fclose, FILE*)
METALMOCK_NONVOID2_NAMESPACED_FREE(uintmax_t, fs, remove_all, const fs::path&, error_code&)

STARTUP
{
   _fileSystem._constCharPointerGetter.reset(_constCharPointerGetterMock = new ConstCharPointerGetterMock);
   _fileSystem._fileSystemExceptionThrower.reset(_fileSystemExceptionThrowerMock = new FileSystemExceptionMakerMock);
   _fileSystem._recursiveFileDeleter.reset(_recursiveFileDeleterMock = new RecursiveFileDeleterMock);
   _fileSystem._caller_Exists.reset(_caller_ExistsMock = new NonVoidOneArgMemberFunctionCallerMock<bool, FileSystem, const fs::path&>);
   _fileSystem._call_fopen = BIND_2ARG_METALMOCK_OBJECT(fopenMock);
   _fileSystem._call_fclose = BIND_1ARG_METALMOCK_OBJECT(fcloseMock);
   _fileSystem._call_fs_remove_all = BIND_2ARG_METALMOCK_OBJECT(remove_allMock);
#ifdef _WIN32
   _fileSystem._call_std_filesystem_absolute = BIND_1ARG_METALMOCK_OBJECT(absoluteMock);
#endif
   _fileSystem._call_std_rename = BIND_2ARG_METALMOCK_OBJECT(renameMock);
   _fileSystem._call_std_filesystem_rename = BIND_3ARG_METALMOCK_OBJECT(renameMock_fs);
   _fileSystem._call_std_filesystem_current_path = BIND_0ARG_METALMOCK_OBJECT(current_pathMock);
   _fileSystem._call_std_filesystem_exists = BIND_1ARG_METALMOCK_OBJECT(existsMock);
}

TEST(DefaultConstructor_NewsComponents_SetsFunctionPointers)
{
   FileSystem fileSystem;

   DELETE_TO_ASSERT_NEWED(fileSystem._constCharPointerGetter);
   DELETE_TO_ASSERT_NEWED(fileSystem._fileSystemExceptionThrower);
   DELETE_TO_ASSERT_NEWED(fileSystem._recursiveFileDeleter);

   DELETE_TO_ASSERT_NEWED(fileSystem._caller_Exists);
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
}

#ifdef __linux__
TEST(GetAbsolutePath_ReturnsNonEmptyPath)
{
   const fs::path relativeFileOrDirectoryPath = ZenUnit::Random<fs::path>();
   _fileSystem.GetAbsolutePath(relativeFileOrDirectoryPath);

   _fileSystem.GetAbsolutePath(".");
}
#elif _WIN32
TEST(GetAbsolutePath_ReturnsResultOfCallingStdFilesystemAbsolute)
{
   const fs::path absoluteFileOrDirectoryPath = absoluteMock.ReturnRandom();
   const fs::path relativeFileOrDirectoryPath = ZenUnit::Random<fs::path>();
   //
   const fs::path returnedAbsoluteFileOrDirectoryPath = _fileSystem.GetAbsolutePath(relativeFileOrDirectoryPath);
   //
   METALMOCK(absoluteMock.CalledOnceWith(relativeFileOrDirectoryPath));
}
#endif

TEST(CurrentDirectoryPath_ReturnsResultOfCallingFilesystemCurrentPath)
{
   const fs::path currentPathReturnValue = current_pathMock.ReturnRandom();
   //
   const fs::path currentDirectoryPath = _fileSystem.CurrentDirectoryPath();
   //
   METALMOCK(current_pathMock.CalledOnce());
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

   renameMock.Return(renameReturnValue);
   //
   const fs::path expectedDirectoryPath = filePath.parent_path();
   const fs::path expectedRenamedFilePath = expectedDirectoryPath / newFileName;
   const string expectedFilePathString = filePath.string();
   const string expectedRenamedFilePathString = expectedRenamedFilePath.string();
   if (expectThrow)
   {
      const string expectedExceptionMessage = String::Concat(
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
   METALMOCK(renameMock.CalledOnceWith(filePathStringCCP, destinationFilePathStringCCP));
}

int _stdFileSystemRenameErrorCodeValue = 0;

void StdFileSystemRenameWithSettableErrorCode(
   const fs::path& /*directoryPath*/, const fs::path& /*destinationDirectoryPath*/, error_code& outErrorCode)
{
   error_code errorCode(_stdFileSystemRenameErrorCodeValue, std::generic_category());
   outErrorCode = errorCode;
}

struct remove_all_FunctionCall
{
   size_t numberOfCalls = 0;
   unsigned long long returnValue = 0;
   fs::path directoryPathArg;
   error_code outErrorCodeArg;
   error_code outErrorCodeReturnValue;
} _remove_all_FunctionCall;

unsigned long long remove_all_CallInstead(const fs::path& directoryPath, error_code& outErrorCode)
{
   ++_remove_all_FunctionCall.numberOfCalls;
   _remove_all_FunctionCall.directoryPathArg = directoryPath;
   _remove_all_FunctionCall.outErrorCodeArg = outErrorCode;
   outErrorCode = _remove_all_FunctionCall.outErrorCodeReturnValue;
   return _remove_all_FunctionCall.returnValue;
}

TEST(RemoveAll_FsRemoveAllReturnsWithSettingErrorCodeTo0_ReturnsNumberOfFilesAndDirectoriesRemoved)
{
   remove_allMock.CallInstead(std::bind(&FileSystemTests::remove_all_CallInstead,
      this, std::placeholders::_1, std::placeholders::_2));
   _remove_all_FunctionCall.returnValue = ZenUnit::Random<unsigned long long>();
   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   //
   const unsigned long long numberOfFilesAndDirectoriesRemoved = _fileSystem.RemoveAll(directoryPath);
   //
   ARE_EQUAL(1, _remove_all_FunctionCall.numberOfCalls);
   ARE_EQUAL(directoryPath, _remove_all_FunctionCall.directoryPathArg);
   ARE_EQUAL(error_code(), _remove_all_FunctionCall.outErrorCodeArg);
   ARE_EQUAL(_remove_all_FunctionCall.returnValue, numberOfFilesAndDirectoriesRemoved);
}

TEST1X1(RemoveAll_FsRemoveAllReturnsWithSettingErrorCodeToNon0_ThrowsFileSystemException,
   int errorCodeValue,
   -1,
   1)
{
   remove_allMock.CallInstead(std::bind(&FileSystemTests::remove_all_CallInstead,
      this, std::placeholders::_1, std::placeholders::_2));
   _remove_all_FunctionCall.returnValue = ZenUnit::Random<unsigned long long>();
   _remove_all_FunctionCall.outErrorCodeReturnValue = error_code(errorCodeValue, std::generic_category());

   const FileSystemException fileSystemException =
      _fileSystemExceptionThrowerMock->MakeFileSystemExceptionForFailedToDeleteDirectoryMock.ReturnRandom();

   const fs::path directoryPath = ZenUnit::Random<fs::path>();
   //
   THROWS_EXCEPTION(_fileSystem.RemoveAll(
      directoryPath), FileSystemException, fileSystemException.what());
   //
   ARE_EQUAL(1, _remove_all_FunctionCall.numberOfCalls);
   ARE_EQUAL(directoryPath, _remove_all_FunctionCall.directoryPathArg);
   ARE_EQUAL(error_code(), _remove_all_FunctionCall.outErrorCodeArg);

   METALMOCK(_fileSystemExceptionThrowerMock->MakeFileSystemExceptionForFailedToDeleteDirectoryMock.CalledOnceWith(
      directoryPath, _remove_all_FunctionCall.returnValue, errorCodeValue));
}

TEST(RenameDirectory_RenamesDirectory_FilesystemRenameReturns0_ReturnsRenamedDirectoryPath)
{
   _stdFileSystemRenameErrorCodeValue = 0;
   renameMock_fs.CallInstead(std::bind(
      &FileSystemTests::StdFileSystemRenameWithSettableErrorCode,
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
   METALMOCK(renameMock_fs.CalledOnceWith(directoryPath, expectedRenamedDirectoryPath, expectedRenameErrorCodeArgument));
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

   renameMock_fs.CallInstead(std::bind(
      &FileSystemTests::StdFileSystemRenameWithSettableErrorCode,
      this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

   const FileSystemException fileSystemException =
      _fileSystemExceptionThrowerMock->MakeFileSystemExceptionForFailedToRenameDirectoryMock.ReturnRandom();

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
   METALMOCK(renameMock_fs.CalledOnceWith(directoryPath, expectedRenamedDirectoryPath, expectedRenameArgumentErrorCode));

   error_code expectedErrorCode(_stdFileSystemRenameErrorCodeValue, std::generic_category());
   METALMOCK(_fileSystemExceptionThrowerMock->MakeFileSystemExceptionForFailedToRenameDirectoryMock.CalledOnceWith(
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
   fopenMock.CallInstead(std::bind(
      &FileSystemTests::fopen_CallInstead, this, placeholders::_1, placeholders::_2));

   const FileSystemException fileSystemException = _fileSystemExceptionThrowerMock->
      MakeFileSystemExceptionForFailedToOpenFileWithFOpenMock.ReturnRandom();

   const fs::path filePath = ZenUnit::Random<fs::path>();
   const string fileOpenMode = ZenUnit::Random<string>();
   //
   THROWS_EXCEPTION(_fileSystem.OpenFile(filePath, fileOpenMode.c_str()),
      FileSystemException, fileSystemException.what());
   //
   const vector<pair<string, string>> expected_fopen_Arguments = { { filePath.string(), fileOpenMode } };
   ARE_EQUAL(expected_fopen_Arguments, _fopen_Arguments);
   METALMOCK(_fileSystemExceptionThrowerMock->MakeFileSystemExceptionForFailedToOpenFileWithFOpenMock.
      CalledOnceWith(filePath, fileOpenMode.c_str()));
}

TEST(OpenFile_FOpenReturnsNonNullFILEPointer_ReturnsFilePointer)
{
   FILE nonNullFOpenReturnValue{};
   _fopen_ReturnValue = &nonNullFOpenReturnValue;
   fopenMock.CallInstead(std::bind(
      &FileSystemTests::fopen_CallInstead, this, placeholders::_1, placeholders::_2));

   const fs::path filePath = ZenUnit::Random<fs::path>();
   const string fileOpenMode = ZenUnit::Random<string>();
   //
   FILE* const filePointer = _fileSystem.OpenFile(filePath, fileOpenMode.c_str());
   //
   const vector<pair<string, string>> expected_fopen_Arguments = { { filePath.string(), fileOpenMode } };
   ARE_EQUAL(expected_fopen_Arguments, _fopen_Arguments);
   ARE_EQUAL(&nonNullFOpenReturnValue, filePointer);
}

TEST1X1(CloseFile_fcloseReturnValueIsNot0_ThrowsFileCloseException,
   int fcloseReturnValue,
   -2,
   -1,
   1,
   2)
{
   fcloseMock.Return(fcloseReturnValue);

   const FileSystemException fileSystemException = ZenUnit::Random<FileSystemException>();
   _fileSystemExceptionThrowerMock->MakeFileSystemExceptionForFailedToCloseFileMock.Return(fileSystemException);

   const fs::path filePath = ZenUnit::Random<fs::path>();
   FILE* const file = nullptr;
   //
   THROWS_EXCEPTION(_fileSystem.CloseFile(filePath, file),
      FileSystemException, fileSystemException.what());
   //
   METALMOCK(fcloseMock.CalledOnceWith(file));
   METALMOCK(_fileSystemExceptionThrowerMock->MakeFileSystemExceptionForFailedToCloseFileMock.CalledOnceWith(filePath));
}

TEST(CloseFile_fcloseReturnValueIs0_Returns)
{
   fcloseMock.Return(0);
   const fs::path filePath = ZenUnit::Random<fs::path>();
   FILE* const file = nullptr;
   //
   _fileSystem.CloseFile(filePath, file);
   //
   METALMOCK(fcloseMock.CalledOnceWith(file));
}

#ifdef __linux__

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
