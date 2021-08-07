#pragma once
#include "libFileRevisor/Components/FileSystem/FileReader.h"

class FileReaderMock : public Metal::Mock<FileReader>
{
public:
   using ReturnType = pair<size_t, array<char, 64>>;
   METALMOCK_NONVOID1_CONST(ReturnType, ReadFirst64Bytes, FILE*)
};
