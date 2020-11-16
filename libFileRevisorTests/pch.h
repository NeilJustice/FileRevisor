#pragma once
#include "libFileRevisor/Compiler/IfMSVCIgnoreTheseWarnings.h"

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

#include <assert.h>
#include <regex>
using namespace std;

#include "ZenUnit/MetalMock.h"

// libFileRevisor Constants
#include "libFileRevisor/Constants/ErrnoValue.h"

// libFileRevisor Docopt
#include "libFileRevisor/Components/Docopt/docopt.h"

// libFileRevisor StaticUtilities
#include "libFileRevisor/StaticUtilities/Map.h"
#include "libFileRevisor/StaticUtilities/ReleaseAssert.h"
#include "libFileRevisor/StaticUtilities/STRING_ENUM_MAP.h"
#include "libFileRevisor/StaticUtilities/StringUtil.h"
#include "libFileRevisor/StaticUtilities/Vector.h"

// libFileRevisor Enums
#include "libFileRevisor/Enums/ProgramMode.h"

// libFileRevisor ValueTypes
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"
#include "libFileRevisor/ValueTypes/RenameResult.h"

// libFileRevisorTests ZenUnit Equalizers And Randoms
#include "libFileRevisorTests/ValueTypes/ZenUnit/FileRevisorArgsEqualizerAndRandom.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/RenameResultZenUnitEqualizerAndRandom.h"
#include "libFileRevisorTests/Exceptions/ZenUnit/FileSystemExceptionRandom.h"
