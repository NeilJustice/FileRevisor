#pragma once
#include "libFileRevisor/Compiler/IfWindowsIgnoreTheseWarningsGlobally.h"

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4365) // signed / unsigned mismatch
#endif
#include <execution>
#include <filesystem>
#ifdef _WIN32
#pragma warning(pop)
#endif

#include <assert.h>
#include <regex>
namespace fs = std::filesystem;
using namespace std;

#if defined __linux__
   #include <cstddef>
   #include <ios>
   #include <memory>
   #if defined(__GNUG__) && !defined(__clang__)
   #include <parallel/algorithm>
   #endif
#endif

#include "ZenUnit/ZenUnit.h"
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
