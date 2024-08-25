#pragma once
#include "libFileRevisor/Exceptions/FileSystemException.h"

FileSystemException TestableRandomFileSystemException(const ZenUnit::RandomGenerator* randomGenerator);

namespace ZenUnit
{
   template<>
   FileSystemException Random();
}
