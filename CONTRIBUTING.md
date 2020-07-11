Anybody can volunteer to help.
Ask to join this repo as a contributor.

## Put your suggestions as a card
In the Projects there is a Kanban called "getting involved": add a card

### Adding a Kanban card
Create an issue, then go to Projects and add a card by selecting the card of interest and drag and drop it to the relevant column

## Coding standard
All C++ source files are formatted using the clang-format LLVM style, disabling automated sorting of includes

## CMake
The CMake tool is used to build the frank C++ projects.
Look into the Github build actions and the travis builds for examples of how to build on Linux and Microsoft Windows.

You will first need to get OpenCV and GSL.

Clone OpenCV and Microsoft GSL from GitHub, build, and install.

If you use VS Code, edit your user settings to configure CMake:
```
    "cmake.configureSettings": {
        "Microsoft.GSL_DIR" : "/projects/GSL/share/cmake/Microsoft.GSL",
        "OpenCV_DIR": "/projects/OpenCV"
    }
```
