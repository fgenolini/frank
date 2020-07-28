#pragma once

#include "config.h"

#if defined(WIN32)
#pragma warning(push, 0)
#pragma warning(disable : 4365)
#endif

#include <string>

#if defined(WIN32)
#pragma warning(pop)
#endif

namespace frank::video {

class input_device {
public:
  input_device();
  input_device(std::string name);

  std::string identifier() const;
  std::string name() const;
  void set_identifier(std::string new_identifier);
  void set_name(std::string new_name);

private:
  /// <summary>
  /// USB vendor and product identifiers, or device path
  /// </summary>
  std::string identifier_;

  /// <summary>
  /// Localised name for the video input device / webcam
  /// </summary>
  std::string name_;
};

} // namespace frank::video
