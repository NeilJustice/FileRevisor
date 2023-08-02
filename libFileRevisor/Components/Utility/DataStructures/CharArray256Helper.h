#pragma once

class CharArray256Helper
{
public:
   virtual ~CharArray256Helper() = default;
   virtual bool ArrayContains0(const array<char, 256>& chars, size_t maximumNumberOfElementsToCompare) const;
};
