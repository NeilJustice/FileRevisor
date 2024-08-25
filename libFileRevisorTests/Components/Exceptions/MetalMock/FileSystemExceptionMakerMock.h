#pragma once
#include "libFileRevisor/Components/Exceptions/FileSystemExceptionMaker.h"

class FileSystemExceptionMakerMock : public Metal::Mock<FileSystemExceptionMaker>
{
public:
   METALMOCK_NONVOID0_CONST(int, GetErrnoValue)

   using PairIntStringType = pair<int, string>;
   METALMOCK_NONVOID0_CONST(PairIntStringType, GetErrnoWithDescription)

   METALMOCK_NONVOID2_CONST(FileSystemException,
      MakeFileSystemExceptionForFailedToOpenFileWithFOpen, const fs::path&, const char*)

   METALMOCK_NONVOID3_CONST(FileSystemException,
      MakeFileSystemExceptionForFailedToRenameDirectory, const fs::path&, const fs::path&, const error_code&)

   METALMOCK_NONVOID1_CONST(FileSystemException,
      MakeFileSystemExceptionForFailedToCloseFile, const fs::path&)

   METALMOCK_NONVOID2_CONST(FileSystemException,
      MakeFileSystemExceptionForRemoveAllFailedToDeleteDirectory, const fs::path&, int)

   METALMOCK_NONVOID1_CONST(FileSystemException,
      MakeFileSystemExceptionForFailedToDeleteFile, const char*)

#ifdef _WIN32
   METALMOCK_NONVOID1_CONST(FileSystemException,
      MakeFileSystemExceptionForFindFirstFileExHavingReturnedInvalidHandleValue, const char*)

   METALMOCK_NONVOID2_CONST(FileSystemException,
      MakeFileSystemExceptionForFailedToSetFileAttribute, const char*, DWORD)
#endif
};
