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

pair<size_t, array<char, 256>> FileReader::ReadFirst256Bytes(FILE* rawFilePointer) const
{
   pair<size_t, array<char, 256>> numberOfBytesReadAndFirst256FileBytes{};
#if defined __linux__|| defined __APPLE__
   numberOfBytesReadAndFirst256FileBytes.first = _call_fread(
      numberOfBytesReadAndFirst256FileBytes.second.data(),
      1,
      numberOfBytesReadAndFirst256FileBytes.second.size(),
      rawFilePointer);
#else
   numberOfBytesReadAndFirst256FileBytes.first = _call_fread_nolock_s(
      numberOfBytesReadAndFirst256FileBytes.second.data(),
      numberOfBytesReadAndFirst256FileBytes.second.size(),
      1,
      numberOfBytesReadAndFirst256FileBytes.second.size(),
      rawFilePointer);
#endif
   return numberOfBytesReadAndFirst256FileBytes;
}
