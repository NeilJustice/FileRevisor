#pragma once
#include "libFileRevisor/Components/FileSystem/FileReader.h"

class FileReaderMock : public Metal::Mock<FileReader>
{
public:
   using ReturnType = pair<size_t, array<char, 256>>;
   METALMOCK_NONVOID1_CONST(ReturnType, ReadFirst256Bytes, FILE*)
};
