#pragma once
#include "libFileRevisor/Compiler/IfWindowsIgnoreTheseWarningsGlobally.h"

#if defined _WIN32
#pragma warning(push)
#pragma warning(disable: 4365) // signed/unsigned mismatch
#endif
#include <execution>
#if defined _WIN32
#pragma warning(pop)
#endif
#include <filesystem>
#include <regex>
#if defined __linux__
#include <cstddef>
#include <ios>
#include <memory>
#include <mutex>
#include <string.h>
#include <thread>
#endif
namespace fs = std::filesystem;
using namespace std;

#include "ZenUnitAndMetalMock/ZenUnit.h"
#include "ZenUnitAndMetalMock/MetalMock.h"

// libFileRevisor Constants
#include "libFileRevisor/Constants/ErrnoValue.h"
#include "libFileRevisor/Enums/Color.h"
#include "libFileRevisor/Enums/FileExceptionType.h"
#include "libFileRevisor/Enums/ProgramMode.h"
#include "libFileRevisor/Enums/WindowsColor.h"
// libFileRevisor ValueTypes
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"
#include "libFileRevisor/ValueTypes/RenameResult.h"
// libFileRevisorTests Components
#include "libFileRevisorTests/Components/Console/MetalMock/ConsoleMock.h"
// libFileRevisor StaticUtilities
#include "libFileRevisor/StaticUtilities/ENUM_STRING_ARRAY.h"
#include "libFileRevisor/StaticUtilities/Map.h"
#include "libFileRevisor/StaticUtilities/ReleaseAssert.h"
#include "libFileRevisor/StaticUtilities/StringUtil.h"
#include "libFileRevisor/StaticUtilities/Type.h"
#include "libFileRevisor/StaticUtilities/Vector.h"

// libFileRevisorTests Exceptions
#include "libFileRevisorTests/Exceptions/ZenUnit/FileSystemExceptionRandom.h"
// libFileRevisorTests ValueTypes
#include "libFileRevisorTests/ValueTypes/ZenUnit/FileRevisorArgsEqualizerAndRandom.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/RenameResultEqualizerAndRandom.h"
