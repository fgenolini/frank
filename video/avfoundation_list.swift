#!/bin/bash
echo 'import AVFoundation; print("-----"); let devices = AVCaptureDevice.devices(for: .video); for device in devices { print("---"); print(device.localizedName); print(device.modelID); print(device.activeFormat); print("-"); print(device.formats) }' | swift -

