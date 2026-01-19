#pragma once

class FileSystemException : public std::exception
{
private:
   // Constant Fields
   const string _exceptionMessage;
public:
   FileSystemException(FileExceptionType fileExceptionType, string_view exceptionMessage);
   virtual ~FileSystemException() override;

   const char* what() const noexcept override;
};
