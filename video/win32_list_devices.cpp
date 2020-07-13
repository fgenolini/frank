#include "config.h"

#if defined(WIN32)
#include <comutil.h>
#include <dshow.h>
#include <windows.h>

#include <string>
#include <vector>

#include <gsl/gsl_util>

using namespace gsl;

#include "input_device.h"
#include "win32_list_devices.h"

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

std::vector<input_device> win32_list_devices() {
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

#endif
