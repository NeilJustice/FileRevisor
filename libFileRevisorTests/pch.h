#pragma once
#include "libFileRevisor/Utilities/MSVCIgnoredWallWarnings.h"

#if defined __linux__
   #include <cstddef>
   #include <experimental/filesystem>
   namespace fs = std::experimental::filesystem;
   #include <ios>
   #include <memory>
   #if defined(__GNUG__) && !defined(__clang__)
   #include <parallel/algorithm>
   #endif
#elif defined _WIN32 || defined __APPLE__
   #include <execution>
   #include <filesystem>
   namespace fs = std::filesystem;
#endif

#include <regex>
using namespace std;

#include "MetalMock.h"

#include "libFileRevisor/Components/Strings/StringUtil.h"
#include "libFileRevisor/Constants/ErrnoValue.h"
#include "libFileRevisor/Enums/ProgramMode.h"
#include "libFileRevisor/Utilities/ReleaseAssert.h"

#include "libFileRevisorTests/ValueTypes/ZenUnit/FileRevisorArgsEqualizerAndRandom.h"
#include "libFileRevisorTests/Exceptions/ZenUnit/FileSystemExceptionRandom.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/RenameResultZenUnitEqualizerAndRandom.h"
