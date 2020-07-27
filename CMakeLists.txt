# frank: C++ project for cooperative programming
# Using
#   OpenCV: computer vision,
#   cvui: GUI for OpenCV,
#   Microsoft GSL: standard C++ guideline support library,
#   portable-file-dialogs: file selection GUI control,
#   Catch 2: C++ unit testing,
#   trompeloeil: C++ mocking for testing,
#   WIX: MSI installer generator for Microsoft Windows,
#
# TO DO:
# Manually install OpenCV and Microsoft GSL before configuring this C++ project
#
# For an example of how to build on Linux
# (Ubuntu 20.04, with OpenCV 4.2.0, and the latest Microsoft GSL from github master),
# or Windows 10 with Visual Studio 2019,
# see the github CI actions on this repository.
# For an example on how to build on Apple Mac OSX, look at the travis YML file.

cmake_minimum_required(VERSION 3.16)

project(frank VERSION 1.1.0)

include(CTest)
enable_testing()

# Computer vision using OpenCV
add_subdirectory(video)
