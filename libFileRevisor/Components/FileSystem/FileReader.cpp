#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileReader.h"

FileReader::FileReader()
{
}

FileReader::~FileReader()
{
}

pair<size_t, array<char, 64>> FileReader::ReadFirst64Bytes(FILE* /*rawFilePointer*/) const
{
//   array<char, 64> first64BytesInFile{};
//#if defined __linux__|| defined __APPLE__
//   const size_t numberOfBytesRead = _call_fread(
//      first64BytesInFile.data(), 1, first64BytesInFile.size(), fileOpenInReadBinaryMode);
//#else
//   const size_t numberOfBytesRead = _call_fread_nolock_s(
//      first64BytesInFile.data(), first64BytesInFile.size(), 1, first64BytesInFile.size(), fileOpenInReadBinaryMode);
//#endif
//   _fileOpenerCloser->CloseFile(fileOpenInReadBinaryMode, filePath);
   return {};
}
