#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FCloseDeleter.h"

FCloseDeleter::FCloseDeleter()
   // Function Pointers
   : _call_fclose(fclose)
{
}

void FCloseDeleter::operator()(FILE* rawFilePointer) const
{
   if (rawFilePointer != nullptr)
   {
      const int fcloseReturnValue = _call_fclose(rawFilePointer);
      if (fcloseReturnValue != 0)
      {
         const string exceptionMessage = String::ConcatValues(
            "fclose(rawFilePointer) in FCloseDeleter::operator(FILE* rawFilePointer) failed with return value ", fcloseReturnValue);
         throw runtime_error(exceptionMessage);
      }
   }
}
