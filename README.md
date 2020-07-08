[![Build Status](https://travis-ci.com/fgenolini/frank.svg?branch=master)](https://travis-ci.com/fgenolini/frank)

![LinuxBuild](https://github.com/fgenolini/frank/workflows/LinuxBuild/badge.svg?branch=master)

# frank
video: OpenCV for up to 4 directly connected video inputs (webcams)

Frank coding experiments for distant pairs and swarms of software engineers, each in isolation, working from home

Ask to join this repo as a contributor

## Put your suggestions as a card
In the Projects there is a Kanban called "getting involved": add a card

### Adding a Kanban card
Create an issue, then go to Projects and add a card by selecting the card of interest and drag and drop it to the relevant column

## Subdirectories
### video
Computer vision using OpenCV, to detect what is present on the computer's webcam, or any other video input, with optional audio input.

## CMake
The CMake tool is used to build the frank C++ projects.
Look into the Github build actions and the travis builds for examples of how to build on Linux.

You will first need to get OpenCV and GSL.

Clone OpenCV and Microsoft GSL from GitHub, build, and install.

If you use VS Code, edit your user settings to configure CMake:
```
    "cmake.configureSettings": {
        "Microsoft.GSL_DIR" : "/projects/GSL/share/cmake/Microsoft.GSL",
        "OpenCV_DIR": "/projects/OpenCV"
    }
```
