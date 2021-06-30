#pragma once

class CharArray64Helper
{
public:
   virtual ~CharArray64Helper() = default;

   virtual bool ArrayContains(const array<char, 64>& /*chars*/, char /*c*/, size_t /*maximumNumberOfElementsToCompare*/) const
   {
      // for (size_t i = 0; i < numberOfBytesRead; ++i)
      // {
      //    const char ithFileByte = first1KBytesInFile[i];
      //    if (ithFileByte == 0)
      //    {
      //       return true;
      //    }
      // }
      return false;
   }
};
