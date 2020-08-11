#include "config.h"

#if defined(UNIX) && !defined(APPLE) && !defined(MINGW) && !defined(MSYS) &&   \
    !defined(CYGWIN) && !defined(WIN32)

WARNINGS_OFF
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <gsl/gsl_util>
WARNINGS_ON

using namespace gsl;

#include "linux_list_devices.h"

// Implementation of video/audio device enumeration on Linux

namespace fs = std::filesystem;

namespace frank::video {

// Linux listing of video input devices using video4linux
std::vector<std::string> linux_list_device_names(void *) {
  // On Linux: list all files called
  //   /sys/class/video4linux/video0/name
  //   /sys/class/video4linux/video1/name
  //   ...
  std::cout << "Video4linux input devices\n";
  const fs::path video4linux{"/sys/class/video4linux"};
  if (!fs::exists(video4linux))
    return std::vector<std::string>();

  std::vector<std::string> new_devices{};
  for (const auto &entry : fs::directory_iterator(video4linux)) {
    const auto video_device = entry.path().filename().string();
    if (!entry.is_directory())
      continue;

    std::cout << "file: " << video_device << '\n';
    fs::path index;
    index += entry.path();
    index /= "index";
    if (!fs::exists(index))
      continue;

    std::ifstream index_file(index, std::ifstream::in);
    if (index_file.is_open()) {
      auto _ = finally([&index_file] { index_file.close(); });
      auto file_contents = index_file.rdbuf();
      auto contents_size =
          file_contents->pubseekoff(0, index_file.end, index_file.in);
      file_contents->pubseekpos(0, index_file.in);
      auto contents = new char[contents_size];
      file_contents->sgetn(contents, contents_size);
      std::string video_device_index{contents};
      if (video_device_index.compare(0, 1, "0") != 0)
        continue;
    }

    fs::path name;
    name += entry.path();
    name /= "name";
    if (!fs::exists(name))
      continue;

    std::ifstream name_file(name, std::ifstream::in);
    if (!name_file.is_open())
      continue;

    auto _ = finally([&name_file] { name_file.close(); });
    auto file_contents = name_file.rdbuf();
    auto contents_size =
        file_contents->pubseekoff(0, name_file.end, name_file.in);
    file_contents->pubseekpos(0, name_file.in);
    auto contents = new char[contents_size];
    file_contents->sgetn(contents, contents_size);
    std::cout << "name: " << contents << '\n';
    std::string video_device_name{contents};
    std::string new_device{video_device_name};
    new_devices.push_back(new_device);
  }

  return new_devices;
}

std::vector<std::string> linux_list_devices(void *mock_data) {
  auto device_names = linux_list_device_names(mock_data);
  return device_names;
}

} // namespace frank::video

#endif
