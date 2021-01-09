#pragma once

class FileSystemException : public std::exception
{
private:
   const string _exceptionMessage;
public:
   FileSystemException(const FileSystemException&) noexcept = default;
   FileSystemException(FileExceptionType fileExceptionType, string_view exceptionMessage);
   virtual ~FileSystemException() = default;
   const char* what() const noexcept override;
};
