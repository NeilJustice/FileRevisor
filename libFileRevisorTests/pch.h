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

#include "libFileRevisor/Components/DataStructures/Vector.h"
#include "libFileRevisor/Components/Exceptions/Exception.h"
#include "libFileRevisor/Components/Strings/StringUtil.h"
#include "libFileRevisor/Constants/ErrnoValue.h"
#include "libFileRevisor/Enums/ProgramMode.h"
#include "libFileRevisor/Utilities/ReleaseAssert.h"

#include "libFileRevisorTests/Components/Console/MetalMock/ConsoleMock.h"
#include "libFileRevisorTests/Components/Exceptions/MetalMock/TryCatchCallerMock.h"
#include "libFileRevisorTests/Components/FileSystem/MetalMock/FileSystemMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/MetalMock/NonVoidTwoArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/MetalMock/VoidTwoArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/NonMember/MetalMock/VoidFourArgFunctionCallerMock.h"
#include "libFileRevisorTests/Components/Iteration/Counter/MetalMock/PredicateCounterMock.h"
#include "libFileRevisorTests/Components/Iteration/ForEach/MetalMock/OneExtraArgMemberForEacherMock.h"
#include "libFileRevisorTests/Components/Iteration/ForEach/MetalMock/ParallelOneExtraArgMemberForEacherMock.h"
#include "libFileRevisorTests/Components/Iteration/Transform/MetalMock/OneExtraArgMemberFunctionTransformerMock.h"
#include "libFileRevisorTests/Components/Strings/MetalMock/PluralizerMock.h"
#include "libFileRevisorTests/Components/Strings/MetalMock/RegexerMock.h"
#include "libFileRevisorTests/Components/Time/MetalMock/StopwatchMock.h"

#include "libFileRevisorTests/ValueTypes/ZenUnit/FileRevisorArgsEqualizerAndRandom.h"
#include "libFileRevisorTests/Exceptions/ZenUnit/FileSystemExceptionRandom.h"
