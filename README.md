[![Build Status](https://travis-ci.com/fgenolini/frank.svg?branch=master)](https://travis-ci.com/fgenolini/frank) [![LinuxBuild](https://github.com/fgenolini/frank/workflows/LinuxBuild/badge.svg?branch=master)](https://github.com/fgenolini/frank/actions?query=workflow%3ALinuxBuild) [![WindowsBuildCI](https://github.com/fgenolini/frank/workflows/WindowsBuildCI/badge.svg?branch=master)](https://github.com/fgenolini/frank/actions?query=workflow%3AWindowsBuildCI)

:see_no_evil: :hear_no_evil: :speak_no_evil:
[coverage: func 47%, line 41%, region 33%](coverage.md)

# frank
Frank coding tools for distant pairs and swarms of software engineers, each in isolation, working from home.

Vision:
- transform the collaborating programmer's communication equipment (computer screens, webcams, microphone, speakers...)
into privately configurable augmented information systems, a bit like the blue HUD inside the helmet used by Iron Man
- rethink some of Bret Victor's Dynamicland / Seeing Spaces,
for a more immediate application to the needs of the remotely working software engineer
- make modest alterations to currently available trivial tools,
such as OpenCV, so that, today, coders can augment their people I/O,
initially for up to 4 streams / video images of remotely working collaborators

First trampoline test project:
- video: OpenCV for up to 4 directly connected video inputs (webcams)

## Subdirectories
### video
Computer vision using OpenCV, to detect what is present on the computer's webcam, or any other video input, with optional audio input.
