cmake_minimum_required(VERSION 3.10.0)
get_filename_component(PROJECT_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project(${PROJECT_NAME} LANGUAGES C)
add_definitions(-DFUNCTION="${PROJECT_NAME}")

# version
get_filename_component(VERSION_TAG_PATH "VERSION" ABSOLUTE)

if(EXISTS ${VERSION_TAG_PATH})
  file(READ VERSION VERSION_TAG OFFSET 0)
  string(REPLACE "." ";" VERSION_LIST ${VERSION_TAG})
  list(GET VERSION_LIST 0 PROJECT_VERSION_MAJOR)
  list(GET VERSION_LIST 1 PROJECT_VERSION_MINOR)
  list(GET VERSION_LIST 2 PROJECT_VERSION_PATCH)
else()
  set(PROJECT_VERSION_MAJOR 0)
  set(PROJECT_VERSION_MINOR 0)
  set(PROJECT_VERSION_PATCH 0)
endif()

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
add_definitions(-DVERSION="${PROJECT_VERSION}")
message(STATUS "[${PROJECT_NAME}] Project version: ${PROJECT_VERSION}")

# source
file(GLOB SOURCES "*.c")
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/main.c")
file(GLOB INCLUDES "*.h")
install(FILES ${INCLUDES} DESTINATION include CONFIGURATIONS RELEASE)
install(FILES ${SOURCES}  DESTINATION src CONFIGURATIONS RELEASE)
message(STATUS "[${PROJECT_NAME}] List of source files: ${SOURCES}")
message(STATUS "[${PROJECT_NAME}] List of include files: ${INCLUDES}")

# library
if(UNIX OR MSYS OR MINGW)
  set(LIBNAME ${PROJECT_NAME})
elseif(WIN32)
  set(LIBNAME lib${PROJECT_NAME})
endif()

# build
if(BUILD_SHARED_LIBS)
  add_library(${LIBNAME} SHARED ${SOURCES})

  if(MSVC)
    if(BUILD_SHARED_LIBS)
      add_definitions(-DMSVC_DLL)
      add_definitions(-DMSVC_DLL_EXPORT)
      include(GenerateExportHeader)
      GENERATE_EXPORT_HEADER(${LIBNAME} BASE_NAME ${LIBNAME} STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)
    endif(BUILD_SHARED_LIBS)
  endif(MSVC)
  install(TARGETS ${LIBNAME} CONFIGURATIONS RELEASE LIBRARY DESTINATION bin)
else()
  add_library(${LIBNAME} STATIC ${SOURCES})
  install(TARGETS ${LIBNAME} CONFIGURATIONS RELEASE ARCHIVE DESTINATION lib)
endif(BUILD_SHARED_LIBS)

target_compile_definitions(${LIBNAME} PUBLIC PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR})
target_compile_definitions(${LIBNAME} PUBLIC PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR})
target_compile_definitions(${LIBNAME} PUBLIC PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH})

include_directories("dependencies")

if(UNIX OR MSYS OR MINGW)
  link_directories("dependencies/argtable/build/src")
  link_directories("dependencies/jansson/build/lib")
elseif(WIN32)
  link_directories("dependencies/argtable/build/src/Release")
  link_directories("dependencies/jansson/build/lib/Release")
endif()

# cli executable
set(COMMON_LIBS "argtable3" "jansson")
add_executable(main main.c)
target_link_libraries(main ${LIBNAME} ${COMMON_LIBS})
set_property(TARGET main PROPERTY OUTPUT_NAME ${PROJECT_NAME})
install(TARGETS main CONFIGURATIONS RELEASE RUNTIME DESTINATION bin)

# test
include(CTest)
enable_testing()

# pack
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

if(UNIX OR MSYS OR MINGW)
elseif(WIN32)
  # set(CMAKE_INSTALL_PREFIX "${LOCALAPPDATA_CMAKE}/AppData/Local/SignalRegistry/${PROJECT_NAME}")
  # file(TO_CMAKE_PATH "$ENV{LOCALAPPDATA}" LOCALAPPDATA_CMAKE)
  # message(STATUS "${LOCALAPPDATA_CMAKE}")
  # set(CPACK_INSTALL_PREFIX "${LOCALAPPDATA_CMAKE}/AppData/Local/SignalRegistry/${PROJECT_NAME}")
  set(CPACK_NSIS_DEFINES "RequestExecutionLevel user") # Force user-level
  set(CPACK_NSIS_MODIFY_PATH ON) # add to path
endif()

include(CPack)
