# Configure the CPack installer.
# Must be last

include(InstallRequiredSystemLibraries)
include(GNUInstallDirs)
include(CPackComponent)
if (WIN32)
  install(TARGETS ${APP_NAME_WINMAIN} RUNTIME DESTINATION bin COMPONENT Applications)
endif()
install(TARGETS ${APP_LOW_NAME} RUNTIME DESTINATION bin COMPONENT Applications)
foreach(Test_Target ${TESTS})
  install(TARGETS ${Test_Target} RUNTIME DESTINATION bin COMPONENT Tests)
endforeach()

set(CPACK_COMPONENTS_ALL Applications Tests Dependencies)

if (WIN32)
  set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
  set(CPACK_GENERATOR WIX)
  set(CPACK_WIX_UPGRADE_GUID "475EB28F-7E72-497E-8F9D-DD607B91003E")
  if(DEFINED _OpenCV_LIB_PATH)
    file(TO_NATIVE_PATH "${_OpenCV_LIB_PATH}" Installed_OpenCV_PATH)
  else()
    get_filename_component(OpenCV_BIN_PATH "${OpenCV_INSTALL_PATH}/../bin" ABSOLUTE)
    if(NOT EXISTS "${OpenCV_BIN_PATH}")
      get_filename_component(OpenCV_BIN_PATH "${OpenCV_INSTALL_PATH}/bin" ABSOLUTE)
    endif()
    file(TO_NATIVE_PATH "${OpenCV_BIN_PATH}" Installed_OpenCV_PATH)
  endif()
  set(OpenCV_VER "${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}")
  set(ALL_RUNTIMES "")
  foreach(OpenCV_DLL_File
          opencv_core opencv_highgui opencv_features2d opencv_imgcodecs
          opencv_imgproc opencv_objdetect opencv_photo opencv_stitching
          opencv_video opencv_videoio opencv_ml opencv_gapi opencv_flann
          opencv_dnn opencv_calib3d opencv_world)
    set(OPENCV_DLL_FILE "${Installed_OpenCV_PATH}/${OpenCV_DLL_File}${OpenCV_VER}.dll")
    if(EXISTS "${OPENCV_DLL_FILE}")
      string(REPLACE "\\" "/" OPENCV_DLL_FILE_UNIX "${OPENCV_DLL_FILE}")
      list(APPEND ALL_RUNTIMES "${OPENCV_DLL_FILE_UNIX}")
    endif()
  endforeach()

  foreach(Extra_OpenCV_DLL_File
          opencv_videoio_ffmpeg opencv_videoio_msmf)
    set(EXTRA_OPENCV_DLL_FILE "${Installed_OpenCV_PATH}/${Extra_OpenCV_DLL_File}${OpenCV_VER}_64.dll")
    if(EXISTS "${EXTRA_OPENCV_DLL_FILE}")
      string(REPLACE "\\" "/" EXTRA_OPENCV_DLL_FILE_UNIX "${EXTRA_OPENCV_DLL_FILE}")
      list(APPEND ALL_RUNTIMES "${EXTRA_OPENCV_DLL_FILE_UNIX}")
    endif()
  endforeach()

  foreach(OpenCV_DLL_Support
          tbb tbbbind tbbmalloc ngraph clDNNPlugin HeteroPlugin
          inference_engine inference_engine_ir_reader inference_engine_legacy
          inference_engine_lp_transformations inference_engine_transformations
          libhwloc-5 MKLDNNPlugin myriadPlugin)
    set(OPENCV_SUPPORT_FILE "${Installed_OpenCV_PATH}/${OpenCV_DLL_Support}.dll")
    if(EXISTS "${OPENCV_SUPPORT_FILE}")
      string(REPLACE "\\" "/" OPENCV_SUPPORT_FILE_UNIX "${OPENCV_SUPPORT_FILE}")
      list(APPEND ALL_RUNTIMES "${OPENCV_SUPPORT_FILE_UNIX}")
    endif()
  endforeach()
  install(FILES ${ALL_RUNTIMES} DESTINATION bin COMPONENT Dependencies)
  install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
          DESTINATION bin COMPONENT Dependencies)

  set(CPACK_ALL_INSTALL_TYPES User Full)
  set(CPACK_COMPONENTS_APPLICATIONS_INSTALL_TYPES User Full)
  set(CPACK_COMPONENTS_DEPENDENCIES_INSTALL_TYPES User Full)
  set(CPACK_COMPONENTS_TESTS_INSTALL_TYPES Full)
  set(CPACK_WIX_PROGRAM_MENU_FOLDER "${ROOT_PROJECT_NAME} ${APP_NAME}")
  set(CPACK_PACKAGE_EXECUTABLES ${APP_NAME_WINMAIN} "${ROOT_PROJECT_NAME} ${APP_NAME}")
  set_property(INSTALL "bin/${APP_NAME_WINMAIN}.exe"
               PROPERTY CPACK_START_MENU_SHORTCUTS "${ROOT_PROJECT_NAME} ${APP_NAME}")
endif()

set(CPACK_PACKAGE_VENDOR "fgenolini")
set(CPACK_PACKAGE_NAME "${ROOT_PROJECT_LOW_NAME}-${APP_LOW_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "${ROOT_PROJECT_NAME} ${APP_NAME} - viewer for 4 webcams with edge detection")
set(CPACK_PACKAGE_VERSION ${APP_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${ROOT_PROJECT_LOW_NAME}_${APP_LOW_NAME}")
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "GUI applications")
set(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION
    "OpenCV GUI viewer for up to 4 video input devices (webcams), with optional edge detection")
set(CPACK_COMPONENT_DEPENDENCIES_DISPLAY_NAME "System dependencies")
set(CPACK_COMPONENT_DEPENDENCIES_DESCRIPTION
    "DLLs for OpenCV and Microsoft C++")
set(CPACK_COMPONENT_TESTS_DISPLAY_NAME "Unit tests")
set(CPACK_COMPONENT_TESTS_DESCRIPTION
    "Tests and checks for all the ${ROOT_PROJECT_NAME} ${APP_NAME} applications, for software quality assurance")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")

include(CPack)
