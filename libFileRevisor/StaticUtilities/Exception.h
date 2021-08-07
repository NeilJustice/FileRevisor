#pragma once

class Exception
{
public:
   static string GetExceptionClassNameAndMessage(const exception* ex);
   Exception() = delete;
};
