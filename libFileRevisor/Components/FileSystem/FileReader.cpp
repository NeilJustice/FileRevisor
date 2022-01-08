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

pair<size_t, array<char, 128>> FileReader::ReadFirst128Bytes(FILE* rawFilePointer) const
{
   pair<size_t, array<char, 128>> numberOfBytesReadAndFirst128FileBytes{};
#if defined __linux__|| defined __APPLE__
   numberOfBytesReadAndFirst128FileBytes.first = _call_fread(
      numberOfBytesReadAndFirst128FileBytes.second.data(),
      1,
      numberOfBytesReadAndFirst128FileBytes.second.size(),
      rawFilePointer);
#else
   numberOfBytesReadAndFirst128FileBytes.first = _call_fread_nolock_s(
      numberOfBytesReadAndFirst128FileBytes.second.data(),
      numberOfBytesReadAndFirst128FileBytes.second.size(),
      1,
      numberOfBytesReadAndFirst128FileBytes.second.size(),
      rawFilePointer);
#endif
   return numberOfBytesReadAndFirst128FileBytes;
}
