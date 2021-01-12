#pragma once
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"

class DirectoryIteratorMock : public Metal::Mock<DirectoryIterator>
{
public:
   METALMOCK_NONVOID1(vector<fs::path>, GetNonEmptyNonIgnoredTextFilePaths, bool)
   METALMOCK_NONVOID0(fs::path, NextNonIgnoredFilePath)
   METALMOCK_NONVOID0(fs::path, NextNonIgnoredDirectoryPath)
   METALMOCK_VOID2(SetDirectoryIterator, const fs::path&, bool)
   METALMOCK_VOID1(SetFileAndDirectoryPathIgnoreSubstrings, const vector<string>&)
};
