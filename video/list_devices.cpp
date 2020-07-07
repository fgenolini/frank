#include "config.h"

#if defined(HAVE_WINDOWS_H)
#include <dshow.h>
#include <windows.h>

#include <gsl/gsl_util>

#include "list_devices.h"

using namespace gsl;

#pragma comment(lib, "strmiids")

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

void device_information(IEnumMoniker *devices) {
  IMoniker *moniker{};
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

    if (SUCCEEDED(get_description)) {
      printf("%S\n", property.bstrVal);
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
    }

    VariantClear(&property);
  }
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

bool video_devices() {
  printf("Video input devices:\n");
  IEnumMoniker *video_devices{};
  auto enumerate =
      devices_from_category(CLSID_VideoInputDeviceCategory, &video_devices);
  if (FAILED(enumerate)) {
    printf("Could not list video input devices\n");
    return false;
  }

  auto _ = finally([video_devices] { video_devices->Release(); });
  device_information(video_devices);
  return true;
}

bool list_input_devices() {
  auto com = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
  if (FAILED(com)) {
    printf("Could not initialise COM\n");
    return false;
  }

  auto _ = finally([] { CoUninitialize(); });
  auto video = video_devices();
  printf("\n");
  audio_devices();
  return video;
}

} // namespace frank::video

#else

// Non-Windows implementations of video/audio device enumeration
// POSIX, Linux, Mac OSX...

#include <filesystem>
#include <iostream>
#include <fstream>

#include "list_devices.h"

namespace fs = std::filesystem;

namespace frank::video {

bool list_input_devices() {
  std::cerr << "Video4linux input devices\n";

  // On Linux: list all files called
  //   /sys/class/video4linux/video0/name
  //   /sys/class/video4linux/video1/name
  //   ...
  const fs::path video4linux { "/sys/class/video4linux" };
  if (!fs::exists(video4linux)) {
    std::cerr << "No video4linux\n";
    return false;
  }

  int device_count{};
  for (const auto& entry : fs::directory_iterator(video4linux)) {
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

    std::ifstream f(name, std::ifstream::in);
    if (f.is_open()) {
      std::cout << "name: " << f.rdbuf() << '\n';
    }

    ++device_count;
  }

  if (device_count < 1) {
    std::cerr << "No video4linux input device\n";
    return false;
  }

  std::cout << device_count << " video input devices" << '\n';
  return true;
}

} // namespace frank::video

#endif

