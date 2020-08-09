#include "pch.h"
#include "libFileRevisor/Components/Exceptions/Exception.h"
#include "libFileRevisor/Utilities/Type.h"

string Exception::ClassNameAndWhat(const exception* e)
{
   const string classNameAndWhat = *Type::GetName(*e) + ": " + e->what();
   return classNameAndWhat;
}
