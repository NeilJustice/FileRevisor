#pragma once

class FileReader
{
   friend class FileReaderTests;
private:
   // Function Pointers
#if defined __linux__ || defined __APPLE__
   function<size_t(void*, size_t, size_t, FILE*)> _call_fread;
#elif defined _WIN32
   function<size_t(void*, size_t, size_t, size_t, FILE*)> _call_fread_nolock_s;
#endif
public:
   FileReader();
   virtual ~FileReader();
   virtual pair<size_t, array<char, 256>> ReadFirst256Bytes(FILE* rawFilePointer) const;
};
