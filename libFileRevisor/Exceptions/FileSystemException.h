#pragma once
enum class FileExceptionType;

class FileSystemException : public std::exception
{
private:
   const string _exceptionMessage;
public:
   FileSystemException(FileExceptionType fileExceptionType, string_view exceptionMessage);
   virtual ~FileSystemException() = default;
   const char* what() const noexcept override;
};
