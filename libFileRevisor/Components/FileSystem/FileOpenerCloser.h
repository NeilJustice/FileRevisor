#pragma once
class ErrorCodeTranslator;

class FileOpenerCloser
{
   friend class FileOpenerCloserTests;
private:
   // Function Pointers
   function<int(FILE*)> _call_fclose;
#if defined __linux__
   std::function<FILE* (const char*, const char*)> _call_fopen;
#elif _WIN32
   std::function<FILE* (const wchar_t*, const wchar_t*, int)> _call_wfsopen;
#endif
   // Constant Components
   unique_ptr<const ErrorCodeTranslator> _errorCodeTranslator;
public:
   FileOpenerCloser();
   virtual ~FileOpenerCloser();
   virtual shared_ptr<FILE> OpenReadModeBinaryFile(const fs::path& filePath, bool throwIfFileNotOpenable) const;
   virtual void CloseFile(FILE* rawFilePointer, const fs::path& filePath) const;
private:
#if defined __linux__
   shared_ptr<FILE> OpenFileOnLinux(const fs::path& filePath, const char* fileOpenMode, bool throwIfFileNotOpenable) const;
#elif _WIN32
   shared_ptr<FILE> OpenFileOnWindows(const fs::path& filePath, const wchar_t* fileOpenMode, bool throwIfFileNotOpenable) const;
#endif
   void ThrowFileOpenExceptionIfFileOpenFailed(const FILE* rawFilePointer, const fs::path& filePath, bool throwIfFileNotOpenable) const;
};
