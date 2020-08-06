
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(APP_VERSION
  "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(ROOT_PROJECT_NAME "Frank")
string(TOLOWER ${ROOT_PROJECT_NAME} ROOT_PROJECT_LOW_NAME)
set(APP_NAME "Video")
string(TOLOWER ${APP_NAME} APP_LOW_NAME)

# Microsoft Windows executable application that has no attached DOS command line window
set(APP_NAME_WINMAIN "${APP_LOW_NAME}w")

# Catch2 test runner application
set(TEST_APP_NAME "${APP_LOW_NAME}_test")

# Library that contains most of the source code for frank video
set(LIB_NAME "${APP_LOW_NAME}_cv")
