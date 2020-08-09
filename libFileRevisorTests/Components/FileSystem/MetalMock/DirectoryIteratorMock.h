#pragma once
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"

class DirectoryIteratorMock : public Metal::Mock<DirectoryIterator>
{
public:
   METALMOCK_VOID1(SetFileAndDirectoryPathIgnoreSubstrings, const vector<string>&)
	METALMOCK_VOID1(SetDirectoryIterator, const fs::path&)
   METALMOCK_VOID1(SetRecursiveDirectoryIterator, const fs::path&)
   METALMOCK_NONVOID0(fs::path, NextNonIgnoredFilePath)
};
