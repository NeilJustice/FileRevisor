project(FileRevisor)
include(${CMAKE_SOURCE_DIR}/CMakeMacros.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE Sources *.h *.cpp)
source_group("" FILES ${Sources})

add_executable(FileRevisor ${Sources})
set_property(TARGET FileRevisor PROPERTY CXX_STANDARD 20)
if(UNIX)
   set_target_properties(FileRevisor PROPERTIES OUTPUT_NAME filerevisor)
endif()

target_link_libraries(FileRevisor libFileRevisor)
if(UNIX AND NOT APPLE)
   target_link_libraries(FileRevisor stdc++fs)
endif()

install(TARGETS FileRevisor DESTINATION "bin")

if(UNIX AND NOT APPLE)
   if(LCovCodeCoverageMode)
     target_link_libraries(FileRevisor CodeCovLibraryInterface)
   endif()
endif()
