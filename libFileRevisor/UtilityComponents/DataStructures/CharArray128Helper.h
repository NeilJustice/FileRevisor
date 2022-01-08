#pragma once

class CharArray128Helper
{
public:
   virtual ~CharArray128Helper() = default;
   virtual bool ArrayContains0(const array<char, 128>& chars, size_t maximumNumberOfElementsToCompare) const;
};
