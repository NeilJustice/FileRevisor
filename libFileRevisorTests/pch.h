#pragma once
#include "libFileRevisor/Compiler/IfWindowsIgnoreTheseWarningsGlobally.h"
#include <filesystem>
#if defined __linux__
   #include <cstddef>
   #include <ios>
   #include <memory>
   #include <mutex>
   #include <string.h>
   #include <thread>
   #ifdef _LIBCPP_VERSION
      #include <algorithm>
      #include <type_traits>
   #endif
#endif
namespace fs = std::filesystem;
using namespace std;

#include "ZenUnitAndMetalMock/MetalMock.h"

// libFileRevisor Constants
#include "libFileRevisor/Constants/ErrnoValue.h"
// libFileRevisor Enums
#include "libFileRevisor/Enums/Color.h"
#include "libFileRevisor/Enums/FileExceptionType.h"
#include "libFileRevisor/Enums/ProgramMode.h"
#include "libFileRevisor/Enums/WindowsColor.h"
// libFileRevisor ValueTypes
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"
#include "libFileRevisor/ValueTypes/RenameResult.h"
// libFileRevisorTests Components
#include "libFileRevisorTests/Components/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisor/docopt/docopt.h"
// libFileRevisor StaticUtilities
#include "libFileRevisor/StaticUtilities/ENUM_STRING_ARRAY.h"
#include "libFileRevisor/StaticUtilities/Map.h"
#include "libFileRevisor/StaticUtilities/ReleaseAssert.h"
#include "libFileRevisor/StaticUtilities/StringUtil.h"
#include "libFileRevisor/StaticUtilities/Type.h"

// libFileRevisorTests Exceptions
#include "libFileRevisorTests/Exceptions/ZenUnit/FileSystemExceptionRandom.h"
// libFileRevisorTests ValueTypes
#include "libFileRevisorTests/ValueTypes/ZenUnit/FileRevisorArgsEqualizerAndRandom.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/RenameResultEqualizerAndRandom.h"
#include "libFileRevisorTests/Components/Docopt/ZenUnit/docoptvalueRandom.h"
