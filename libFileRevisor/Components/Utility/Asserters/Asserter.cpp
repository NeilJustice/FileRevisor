#include "pch.h"
#include "libFileRevisor/Components/Utility/Asserters/Asserter.h"

void Asserter::ThrowIfIntsNotEqual(int expectedInt, int actualInt, string_view message) const
{
   if (actualInt != expectedInt)
   {
      const string exceptionMessage = String::ConcatValues(
         "Asserter::ThrowIfIntsNotEqual(int expected, int actual, string_view message) failed. ",
         "expectedInt=", expectedInt, ", actualInt=", actualInt, ", message=\"", message, "\"");
      throw runtime_error(exceptionMessage);
   }
}

void Asserter::ThrowIfSizeTValuesNotEqual(size_t expectedSizeT, size_t actualSizeT, string_view message) const
{
   if (actualSizeT != expectedSizeT)
   {
      const string exceptionMessage = String::ConcatValues(
         "Asserter::ThrowIfSizeTValuesNotEqual(size_t expectedSizeT, size_t actualSizeT, string_view message) failed. ",
         "expectedSizeT=", expectedSizeT, ", actualSizeT=", actualSizeT, ", message=\"", message, "\"");
      throw runtime_error(exceptionMessage);
   }
}
