
if (NOT DEFINED ENV{BOOST_PATH})
  message(FATAL_ERROR "BOOST_PATH MUST BE DEFINED!")
endif()

# ---

set(CTEST_PROJECT_NAME "TestingFileSystem1")

set(CTEST_CONFIGURATION_TYPE Release)

if (PLATFORM STREQUAL osx)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE osx.cmake)

elseif (PLATFORM STREQUAL ios)
  set(CTEST_CMAKE_GENERATOR "Xcode")
  set(TOOLCHAIN_FILE ios.xcode.cmake)
# set(CONFIGURE_ARGS "-DIOS_DEPLOYMENT_TARGET=5.1.1 -DIOS_ARCHS=armv7")

elseif (PLATFORM STREQUAL android)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE android.cmake)

elseif (PLATFORM STREQUAL emscripten)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE emscripten.cmake)

elseif (PLATFORM STREQUAL mxe)
  set(CTEST_CMAKE_GENERATOR "Ninja")
  set(TOOLCHAIN_FILE mxe.cmake)

else()
  message(FATAL_ERROR "UNSUPPORTED PLATFORM!")
endif()

set(CONFIGURE_ARGS "${CONFIGURE_ARGS} -DBOOST_ROOT=$ENV{BOOST_PATH}/dist/${PLATFORM}")
