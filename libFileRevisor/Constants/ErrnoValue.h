#pragma once

class ErrnoValue
{
public:
   static const int PermissionDenied;
   static const int BrokenPipe;
   ErrnoValue() = delete;
};
