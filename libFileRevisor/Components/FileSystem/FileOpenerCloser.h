#pragma once
namespace Utils
{
   class RawCFileAPI;
}
class ErrorCodeTranslator;
template<typename ClassType, typename Arg1Type, typename Arg2Type, typename Arg3Type>
class VoidThreeArgMemberFunctionCaller;

class FileOpenerCloser
{
   friend class FileOpenerCloserTests;
private:
   // Function Callers
   using _caller_ThrowFileOpenExceptionIfFileOpenFailedType = VoidThreeArgMemberFunctionCaller<
      FileOpenerCloser,
      const FILE*,
      const fs::path&,
      bool>;
   unique_ptr<const _caller_ThrowFileOpenExceptionIfFileOpenFailedType> _caller_ThrowFileOpenExceptionIfFileOpenFailed;
   // Constant Components
   unique_ptr<const ErrorCodeTranslator> _errorCodeTranslator;
   unique_ptr<const Utils::RawCFileAPI> _rawCFileAPI;
public:
   FileOpenerCloser();
   virtual ~FileOpenerCloser();

   virtual shared_ptr<FILE> OpenReadModeBinaryFile(const fs::path& filePath, bool throwIfFileNotOpenable) const;
   virtual void CloseFile(FILE* rawFilePointer, const fs::path& filePath) const;
private:
   void ThrowFileOpenExceptionIfFileOpenFailed(
      const FILE* rawFilePointer,
      const fs::path& filePath,
      bool throwIfFileNotOpenable) const;
};
