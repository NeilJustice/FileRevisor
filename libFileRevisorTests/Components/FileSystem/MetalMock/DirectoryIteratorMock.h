#pragma once
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"

class DirectoryIteratorMock : public Metal::Mock<DirectoryIterator>
{
public:
   METALMOCK_NONVOID0(vector<fs::path>, GetNonEmptyNonIgnoredTextFilePaths)
   METALMOCK_NONVOID0(fs::path, NextNonIgnoredFilePath)
   METALMOCK_NONVOID0(fs::path, NextNonIgnoredFolderPath)
   METALMOCK_VOID2(SetDirectoryIterator, const fs::path&, bool)
   METALMOCK_VOID1(SetFileAndFolderPathIgnoreSubstrings, const vector<string>&)
};
