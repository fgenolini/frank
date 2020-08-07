Anybody can volunteer to help.
Ask to join this repo as a contributor.

## Put your suggestions as a card
In the Projects there is a Kanban called "getting involved": add a card

### Adding a Kanban card
Create an issue, then go to Projects and add a card by selecting the card of interest and drag and drop it to the relevant column

## C++ 17
The modern standard C++ 17 programming language is used in this project.
The code is designed to be simple and portable across:
* Microsoft Windows
* Ubuntu Linux (other distributions will probably work too)
* Apple Mac OSX 10.15 (other recent versions should be OK as well)

A number of compilers are supported, as long as they implement std C++ 17:
* Microsoft Visual C++ 16 (Visual Studio 2019)
* gcc
* clang

## Coding standards
All C++ source files are formatted using the clang-format LLVM style,
disabling automated sorting of includes.
Maximum recommended line width is 80 columns in general,
and 120 columns in exceptional circumstances.

Follow Robert C. Martin (Uncle Bob) principle of:
good code does not require many comments (if any at all).

## CMake
The CMake tool is used to build the frank C++ projects.

CMake facilitates the automation of the creation of build systems
on all supported platforms.

Use cmake-gui for a user friendly GUI,
or use the cmake command line tool for continuous integration scripts
(Travis, github actions).

Microsoft Windows with Visual Studio 2019:
* creates a Visual Studio solution with native C++ projects

Linux and Apple Mac OSX:
* creates Makefiles

Look into the Github build actions and the travis builds
for examples of how to build on Linux and Microsoft Windows.

You will first need to get OpenCV and GSL.

Clone OpenCV and Microsoft GSL from GitHub, build, and install.

If you use VS Code, edit your user settings to configure CMake:
```
    "cmake.configureSettings": {
        "Microsoft.GSL_DIR" : "/projects/GSL/share/cmake/Microsoft.GSL",
        "OpenCV_DIR": "/projects/OpenCV",
        "OpenCV_STATIC" : "ON",
        "THIRD_PARTY_SHARED_LIBS" : "OFF",
        "CMAKE_VERBOSE_MAKEFILE" : "ON"
    },
    "cmake.parallelJobs": 1,
    "C_Cpp.clang_format_fallbackStyle": "LLVM",
    "editor.rulers": [
        80, 120
    ]
```

With these settings you can choose to link third party libraries
as dynamic (DLL or SO) or static (LIB or A).
You can also allow verbose builds showing which compiler command line
parameters are used.
You can also reduce the number of parallel build operations, so that any
error in the build output makes more sense.

## Dependencies
* Microsoft Windows, or Linux, or Apple Mac OSX
* C++ compiler, with standard C++ 17 support
* CMake 3.16, or above
* OpenCV 4.2.0, or above, computer vision for C++
* Microsoft GSL, standard C++ Guidelines Support Library
* Portable file dialogs, for C++
* nlohmann/json, JSON for C++
* Catch 2, unit testing for C++
* trompeloeil, mocking for C++

If building on Microsoft Windows, the WIX installer is also required.
