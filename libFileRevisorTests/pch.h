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

#include "ZenMock.h"

#include "libFileRevisor/Components/DataStructures/Vector.h"
#include "libFileRevisor/Components/Exceptions/Exception.h"
#include "libFileRevisor/Components/Strings/StringUtil.h"
#include "libFileRevisor/Constants/ErrnoValue.h"
#include "libFileRevisor/Enums/ProgramMode.h"
#include "libFileRevisor/Utilities/ReleaseAssert.h"

#include "libFileRevisorTests/Components/Console/ZenMock/ConsoleMock.h"
#include "libFileRevisorTests/Components/Exceptions/ZenMock/TryCatchCallerMock.h"
#include "libFileRevisorTests/Components/FileSystem/ZenMock/FileSystemMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/ZenMock/NonVoidTwoArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/Member/ZenMock/VoidTwoArgMemberFunctionCallerMock.h"
#include "libFileRevisorTests/Components/FunctionCallers/NonMember/ZenMock/VoidFourArgFunctionCallerMock.h"
#include "libFileRevisorTests/Components/Iteration/Counter/ZenMock/PredicateCounterMock.h"
#include "libFileRevisorTests/Components/Iteration/ForEach/ZenMock/OneExtraArgMemberForEacherMock.h"
#include "libFileRevisorTests/Components/Iteration/ForEach/ZenMock/ParallelOneExtraArgMemberForEacherMock.h"
#include "libFileRevisorTests/Components/Iteration/Transform/ZenMock/OneExtraArgMemberFunctionTransformerMock.h"
#include "libFileRevisorTests/Components/Strings/ZenMock/PluralizerMock.h"
#include "libFileRevisorTests/Components/Strings/ZenMock/RegexerMock.h"
#include "libFileRevisorTests/Components/Time/ZenMock/StopwatchMock.h"

#include "libFileRevisorTests/ValueTypes/ZenUnit/FileRevisorArgsEqualizerAndRandom.h"
#include "libFileRevisorTests/Exceptions/ZenUnit/FileSystemExceptionRandom.h"
