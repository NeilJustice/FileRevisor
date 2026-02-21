#pragma once
#include "libFileRevisor/Components/FileSystem/FileSystemExceptions.h"

FileSystemException TestableRandomFileSystemException(const ZenUnit::RandomGenerator* randomGenerator);

namespace ZenUnit
{
   template<>
   FileSystemException Random();
}
