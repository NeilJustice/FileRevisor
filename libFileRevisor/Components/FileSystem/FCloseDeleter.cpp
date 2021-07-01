#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FCloseDeleter.h"

FCloseDeleter::FCloseDeleter()
   // Function Pointers
   : _call_fclose(fclose)
{
}

void FCloseDeleter::operator()(FILE* rawFlePointer) const
{
   const int fcloseReturnValue = _call_fclose(rawFlePointer);
   if (fcloseReturnValue != 0)
   {
      const string exceptionMessage = String::ConcatValues("fclose(rawFlePointer) returned ", fcloseReturnValue);
      throw runtime_error(exceptionMessage);
   }
}
