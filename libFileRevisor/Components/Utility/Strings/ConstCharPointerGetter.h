#pragma once

class ConstCharPointerGetter
{
public:
   virtual const char* GetStringConstCharPointer(const std::string& str) const;
   virtual ~ConstCharPointerGetter() = default;
};
