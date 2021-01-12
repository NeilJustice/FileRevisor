#pragma once
#include "libFileRevisor/Compiler/IfWindowsIgnoreTheseWarningsGlobally.h"

#if defined __linux__ || defined __APPLE__
   #include <assert.h>
   #include <cstddef>
   #include <filesystem>
   #include <ios>
   #include <memory>
   #include <regex>
   #include <string.h>
   #include <unordered_map>
   #include <unordered_set>
   #if defined(__GNUG__) && !defined(__clang__)
   #include <parallel/algorithm>
   #endif
#elif defined _WIN32
   #pragma warning(push)
   #pragma warning(disable: 4365) // signed / unsigned mismatch
   #include <assert.h>
   #include <execution>
   #include <filesystem>
   #include <regex>
   #include <unordered_map>
   #include <unordered_set>
   #pragma warning(pop)
#endif

namespace fs = std::filesystem;
using namespace std;

#include "ZenUnitAndMetalMock/ZenUnit.h"
#include "ZenUnitAndMetalMock/MetalMock.h"

// libFileRevisor Constants
#include "libFileRevisor/Constants/ErrnoValue.h"

// libFileRevisorTests Components Docopt
#include "libFileRevisor/Components/Docopt/docopt.h"

// libFileRevisorTests Components MetalMock
#include "libFileRevisorTests/Components/Console/MetalMock/ConsoleMock.h"

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
