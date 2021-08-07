#pragma once

class CharArray64Helper
{
public:
   virtual ~CharArray64Helper() = default;
   virtual bool ArrayContains0(const array<char, 64>& chars, size_t maximumNumberOfElementsToCompare) const;
};
