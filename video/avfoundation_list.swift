#!/bin/bash
# Example of how to list video input devices on macos using the Swift REPL
# This script is not actually used by the C++ frank::video
echo 'import AVFoundation;let devices = AVCaptureDevice.devices(for: .video);for device in devices{print(device.localizedName)}'|swift -
