#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileReader.h"

FileReader::FileReader()
   // Function Pointers
#if defined __linux__ || defined __APPLE__
   : _call_fread(fread)
#elif defined _WIN32
   : _call_fread_nolock_s(_fread_nolock_s)
#endif
{
}

FileReader::~FileReader()
{
}

pair<size_t, array<char, 64>> FileReader::ReadFirst64Bytes(FILE* rawFilePointer) const
{
   pair<size_t, array<char, 64>> numberOfBytesReadAndFirst64FileBytes{};
#if defined __linux__|| defined __APPLE__
   numberOfBytesReadAndFirst64FileBytes.first = _call_fread(
      numberOfBytesReadAndFirst64FileBytes.second.data(),
      numberOfBytesReadAndFirst64FileBytes.second.size(),
      1,
      rawFilePointer);
#else
   numberOfBytesReadAndFirst64FileBytes.first = _call_fread_nolock_s(
      numberOfBytesReadAndFirst64FileBytes.second.data(),
      numberOfBytesReadAndFirst64FileBytes.second.size(),
      numberOfBytesReadAndFirst64FileBytes.second.size(),
      1,
      rawFilePointer);
#endif
   return numberOfBytesReadAndFirst64FileBytes;
}
