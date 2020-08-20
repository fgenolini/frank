#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

#include "device/input_device.h"

namespace frank::video {

constexpr auto NO_VIDEO = "no video";

input_device::input_device() : name_(NO_VIDEO) {}

input_device::input_device(std::string name) : name_(name) {}

bool input_device::operator==(input_device const &other) const {
  return name_.compare(other.name()) == 0;
}

std::string input_device::name() const { return name_; }

} // namespace frank::video
