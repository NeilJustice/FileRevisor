#include "pch.h"
#include "libFileRevisor/Components/Asserter/Asserter.h"

namespace Utils
{
   void Asserter::ThrowIfBoolsNotEqual(bool expected, bool actual, string_view message) const
   {
      if (actual != expected)
      {
         const string exceptionMessage = String::Concat("Utils::Asserter::ThrowIfBoolsNotEqual() failed.",
            " expected=", expected, ", actual=", actual, ", message=\"", message, "\"");
         throw runtime_error(exceptionMessage);
      }
   }

   void Asserter::ThrowIfSizeTNotEqual(size_t expected, size_t actual, string_view message) const
   {
      if (actual != expected)
      {
         const string exceptionMessage = String::Concat("Utils::Asserter::ThrowIfSizeTNotEqual() failed.",
            " expected=", expected, ", actual=", actual, ", message=\"", message, "\"");
         throw runtime_error(exceptionMessage);
      }
   }

   void Asserter::ThrowIfStringsNotEqual(string_view expected, string_view actual, string_view message) const
   {
      if (actual != expected)
      {
         const string exceptionMessage = String::Concat("Utils::Asserter::ThrowIfStringsNotEqual() failed.",
            " expected=\"", expected, "\", actual=\"", actual, "\", message=\"", message, "\"");
         throw runtime_error(exceptionMessage);
      }
   }
}
