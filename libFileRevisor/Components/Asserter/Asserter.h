#pragma once

namespace Utils
{
   class Asserter
   {
   public:
      virtual void ThrowIfBoolsNotEqual(bool expected, bool actual, string_view message) const;
      virtual void ThrowIfSizeTNotEqual(size_t expected, size_t actual, string_view message) const;
      virtual void ThrowIfStringsNotEqual(string_view expected, string_view actual, string_view message) const;
      virtual ~Asserter() = default;
   };
}
