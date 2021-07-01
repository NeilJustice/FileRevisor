#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FCloseDeleter.h"

FCloseDeleter::FCloseDeleter()
   // Function Pointers
   : _call_fclose(fclose)
{
}

void FCloseDeleter::operator()(FILE* filePointer) const
{
#if defined __linux__ || defined __APPLE__
   if (filePointer->_fileno == 0)
   {
      return;
   }
#elif defined _WIN32
   if (filePointer->_Placeholder == nullptr)
   {
      return;
   }
#endif
   const int fcloseReturnValue = _call_fclose(filePointer);
   if (fcloseReturnValue != 0)
   {
      const string exceptionMessage = String::ConcatValues("fclose(filePointer) returned ", fcloseReturnValue);
      throw runtime_error(exceptionMessage);
   }
}
