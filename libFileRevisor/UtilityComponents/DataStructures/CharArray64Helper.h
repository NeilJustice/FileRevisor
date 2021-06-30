#pragma once

class CharArray64Helper
{
public:
   virtual ~CharArray64Helper() = default;
   virtual bool ArrayContains(const array<char, 64>& /*chars*/, char /*c*/, size_t /*maximumNumberOfElementsToCompare*/) const;
};
