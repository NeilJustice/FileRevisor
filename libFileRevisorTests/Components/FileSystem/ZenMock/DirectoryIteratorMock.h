#pragma once
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"

class DirectoryIteratorMock : public Zen::Mock<DirectoryIterator>
{
public:
   ZENMOCK_VOID1(SetFileAndDirectoryPathIgnoreSubstrings, const vector<string>&)
	ZENMOCK_VOID1(SetDirectoryIterator, const fs::path&)
   ZENMOCK_VOID1(SetRecursiveDirectoryIterator, const fs::path&)
   ZENMOCK_NONVOID0(fs::path, NextNonIgnoredFilePath)
};
