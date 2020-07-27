#pragma once

#include <string>

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
