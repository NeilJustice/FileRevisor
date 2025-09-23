#pragma once

class ConstCharPointerGetter
{
public:
   virtual ~ConstCharPointerGetter() = default;
   virtual const char* GetStringConstCharPointer(const std::string& str) const;
};
