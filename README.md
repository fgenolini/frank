# frank
Frank coding experiments for distant pairs and swarms of software engineers, each in isolation, working from home

Ask to join this repo as a contributor

## Put your suggestions as a card
In the Projects there is a Kanban called "getting involved": add a card

### Adding a Kanban card
Create an issue, then go to Projects and add a card by selecting the card of interest and drag and drop it to the relevant column

## CMake
The CMake tool is used to build the frank C++ projects.
You will first need to get OpenCV and GSL.

Clone OpenCV and Microsoft GSL from GitHub, build, and install.

If you use VS Code, edit your user settings to configure CMake:
```
    "cmake.configureSettings": {
        "Microsoft.GSL_DIR" : "/projects/GSL/share/cmake/Microsoft.GSL",
        "OpenCV_DIR": "/projects/OpenCV"
    }
```
