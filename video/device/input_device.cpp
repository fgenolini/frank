#include "config.h"

WARNINGS_OFF
#include <string>
WARNINGS_ON

#include "input_device.h"

namespace frank::video {

constexpr auto NO_IDENTIFIER = "none";
constexpr auto NO_VIDEO = "no video";

input_device::input_device() : identifier_(NO_IDENTIFIER), name_(NO_VIDEO) {}

input_device::input_device(std::string name)
    : identifier_(NO_IDENTIFIER), name_(name) {}

std::string input_device::identifier() const { return identifier_; }

std::string input_device::name() const { return name_; }

void input_device::set_identifier(std::string new_identifier) {
  identifier_ = new_identifier;
}

void input_device::set_name(std::string new_name) {
  name_ = std::string(new_name);
}

} // namespace frank::video
