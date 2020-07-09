#include <string>
#include <vector>

#include "config.h"
#include "input_device.h"
#include "list_devices.h"

#if defined(WIN32)
#include <comutil.h>
#include <dshow.h>
#include <windows.h>

#include <gsl/gsl_util>

using namespace gsl;

#pragma comment(lib, "strmiids")
#pragma comment(lib, "comsuppw.lib")

namespace frank::video {

HRESULT devices_from_category(REFGUID category, IEnumMoniker **devices) {
  ICreateDevEnum *system_device{};
  auto class_creator =
      CoCreateInstance(CLSID_SystemDeviceEnum, nullptr, CLSCTX_INPROC_SERVER,
                       IID_PPV_ARGS(&system_device));
  if (FAILED(class_creator)) {
    printf("Could not create system device enumerator\n");
    return class_creator;
  }

  auto _ = finally([system_device] { system_device->Release(); });

  auto category_enumerator =
      system_device->CreateClassEnumerator(category, devices, 0);
  if (category_enumerator == S_FALSE) {
    printf("Empty category\n");
    category_enumerator = VFW_E_NOT_FOUND;
  }

  return category_enumerator;
}

std::vector<input_device> device_information(IEnumMoniker *devices) {
  IMoniker *moniker{};
  std::vector<input_device> new_devices{};
  while (devices->Next(1, &moniker, nullptr) == S_OK) {
    auto _ = finally([moniker] { moniker->Release(); });
    IPropertyBag *property_bag{};
    auto bind_to_storage =
        moniker->BindToStorage(0, 0, IID_PPV_ARGS(&property_bag));
    if (FAILED(bind_to_storage)) {
      printf("Could not bind to storage\n");
      continue;
    }

    auto _1 = finally([property_bag] { property_bag->Release(); });
    VARIANT property;
    VariantInit(&property);
    auto get_description = property_bag->Read(L"Description", &property, 0);
    if (FAILED(get_description)) {
      get_description = property_bag->Read(L"FriendlyName", &property, 0);
    }

    auto convert_text = [](BSTR unicode_com_text) {
      auto source_text = _bstr_t(unicode_com_text);
      auto length_excluding_terminating_zero = source_text.length();
      auto length_including_zero =
          (rsize_t)length_excluding_terminating_zero + 1;
      auto converted_text = new char[length_including_zero]{};
      strcpy_s(converted_text, length_including_zero, source_text);
      return std::string(converted_text);
    };

    input_device new_device{};
    if (SUCCEEDED(get_description)) {
      printf("%S\n", property.bstrVal);
      new_device.set_name(convert_text(property.bstrVal));
    }

    VariantClear(&property);
    auto get_audio_id = property_bag->Read(L"WaveInID", &property, 0);
    if (SUCCEEDED(get_audio_id)) {
      printf("  WaveIn ID: %d\n", property.lVal);
    }

    VariantClear(&property);
    auto get_device_path = property_bag->Read(L"DevicePath", &property, 0);
    if (SUCCEEDED(get_device_path)) {
      printf("  Device path: %S\n", property.bstrVal);
      new_device.set_identifier(convert_text(property.bstrVal));
    }

    VariantClear(&property);
    new_devices.push_back(new_device);
  }

  return new_devices;
}

void audio_devices() {
  printf("Audio input devices:\n");
  IEnumMoniker *audio_devices{};
  auto enumerate =
      devices_from_category(CLSID_AudioInputDeviceCategory, &audio_devices);
  if (FAILED(enumerate)) {
    printf("Could not list audio input devices\n");
    return;
  }

  auto _ = finally([audio_devices] { audio_devices->Release(); });
  device_information(audio_devices);
}

std::vector<input_device> video_devices() {
  printf("Video input devices:\n");
  IEnumMoniker *video_devices{};
  auto enumerate =
      devices_from_category(CLSID_VideoInputDeviceCategory, &video_devices);
  if (FAILED(enumerate)) {
    printf("Could not list video input devices\n");
    std::vector<input_device> no_device{};
    return no_device;
  }

  auto _ = finally([video_devices] { video_devices->Release(); });
  return device_information(video_devices);
}

std::vector<input_device> list_input_devices() {
  auto com = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
  if (FAILED(com)) {
    printf("Could not initialise COM\n");
    std::vector<input_device> no_device{};
    return no_device;
  }

  auto _ = finally([] { CoUninitialize(); });
  auto devices = video_devices();
  printf("\n");
  audio_devices();
  return devices;
}

} // namespace frank::video

#elif !defined(WIN32)

// Non-Windows implementations of video/audio device enumeration
// POSIX, Linux, Mac OSX...

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace frank::video {

#if defined(UNIX) && !defined(APPLE) && !defined(MINGW) && !defined(MSYS) &&   \
    !defined(CYGWIN)
std::vector<input_device> list_linux_input_devices() {
  std::cerr << "Video4linux input devices\n";

  // On Linux: list all files called
  //   /sys/class/video4linux/video0/name
  //   /sys/class/video4linux/video1/name
  //   ...
  const fs::path video4linux{"/sys/class/video4linux"};
  if (!fs::exists(video4linux)) {
    std::cerr << "No video4linux\n";
    std::vector<input_device> no_device{};
    return no_device;
  }

  std::vector<input_device> new_devices{};
  for (const auto &entry : fs::directory_iterator(video4linux)) {
    const auto video_device = entry.path().filename().string();
    if (!entry.is_directory()) {
      continue;
    }

    std::cout << "file: " << video_device << '\n';
    fs::path name;
    name += entry.path();
    name /= "name";
    if (!fs::exists(name)) {
      continue;
    }

    input_device new_device{};
    new_devices.set_identifier(video_device);
    std::ifstream f(name, std::ifstream::in);
    if (f.is_open()) {
      auto new_device_name = f.rdbuf();
      std::cout << "name: " << new_device_name << '\n';
      new_device.set_name(new_device_name);
    }

    new_devices.push_back(new_device);
  }

  if (new_devices.size() < 1) {
    std::cerr << "No video4linux input device\n";
    return new_devices;
  }

  std::cout << new_devices.size() << " video input devices" << '\n';
  return new_devices;
}
#endif

#if defined(UNIX) && defined(APPLE)
std::vector<input_device> list_mac_osx_input_devices() {
  std::cerr << "AVFoundation input devices\n";
  std::string swift_script{"echo "
                           "'import AVFoundation;"
                           "print(\"-----\");"
                           "let devices = AVCaptureDevice.devices(for: .video);"
                           "for device in devices {"
                           "print(\"---\");"
                           "print(device.localizedName);"
                           "print(device.modelID);"
                           "print(device.activeFormat);"
                           "print(\"-\");"
                           "print(device.formats)"
                           "}'"
                           "|swift -"};
  system(swift_script.c_str());

  // TODO: read stdout from swift script
  std::vector<input_device> no_device{};
  return no_device;
}
#endif

std::vector<input_device> list_input_devices() {
#if defined(UNIX) && !defined(APPLE) && !defined(MINGW) && !defined(MSYS) &&   \
    !defined(CYGWIN)
  return list_linux_input_devices();
#elif defined(UNIX) && defined(APPLE)
  return list_mac_osx_input_devices();
#else
  std::vector<input_device> no_device{};
  return no_device;
#endif
}

} // namespace frank::video

#endif
