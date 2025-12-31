project(FileRevisor)
include(${CMAKE_SOURCE_DIR}/CMakeMacros.cmake)

if(UNIX)
   find_package(TBB REQUIRED)
endif()

include_directories(${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE Sources *.h *.cpp)
source_group("" FILES ${Sources})

add_executable(FileRevisor ${Sources})

target_link_libraries(FileRevisor libFileRevisor)
if(UNIX)
   set_property(TARGET FileRevisor PROPERTY LINKER_TYPE LLD)
   target_link_libraries(FileRevisor TBB::tbb)
   set_target_properties(FileRevisor PROPERTIES OUTPUT_NAME filerevisor)
   if(LCovCodeCoverageMode)
     target_link_libraries(FileRevisor CodeCovLibraryInterface)
   endif()
elseif(MSVC)
   replace(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Ob1" "/Ob2")
   append(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Oi /Ot /GS- /arch:AVX2 /fp:fast")
   replace(CMAKE_EXE_LINKER_FLAGS_DEBUG "/debug" "/DEBUG:FASTLINK")
   replace(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "/debug" "/DEBUG:FASTLINK")
endif()

install(TARGETS FileRevisor DESTINATION "bin")
