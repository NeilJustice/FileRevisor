#pragma once
#include "libFileRevisor/Components/Exceptions/FileSystemExceptionMaker.h"

class FileSystemExceptionMakerMock : public Zen::Mock<FileSystemExceptionMaker>
{
public:
   ZENMOCK_NONVOID0_CONST(int, GetErrnoValue)

   ZENMOCK_NONVOID2_CONST(FileSystemException,
      MakeFileSystemExceptionForFailedToOpenFileWithFOpen, const fs::path&, const char*)

   ZENMOCK_NONVOID3_CONST(FileSystemException,
      MakeFileSystemExceptionForFailedToRenameDirectory, const fs::path&, const fs::path&, const error_code&)

   ZENMOCK_NONVOID1_CONST(FileSystemException,
      MakeFileSystemExceptionForFailedToCloseFile, const fs::path&)

   ZENMOCK_NONVOID3_CONST(FileSystemException,
      MakeFileSystemExceptionForFailedToDeleteDirectory, const fs::path&, long long, int)

   ZENMOCK_NONVOID1_CONST(FileSystemException,
      MakeFileSystemExceptionForFailedToDeleteFile, const char*)

#ifdef _WIN32
   ZENMOCK_NONVOID1_CONST(FileSystemException,
      MakeFileSystemExceptionForFindFirstFileExHavingReturnedInvalidHandleValue, const char*)

   ZENMOCK_NONVOID2_CONST(FileSystemException,
      MakeFileSystemExceptionForFailedToSetFileAttribute, const char*, DWORD)
#endif
};
