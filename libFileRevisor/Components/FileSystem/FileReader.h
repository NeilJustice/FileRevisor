#pragma once

class FileReader
{
public:
   FileReader();
   virtual ~FileReader();
   virtual pair<size_t, array<char, 64>> ReadFirst64Bytes(FILE* rawFilePointer) const;
};
