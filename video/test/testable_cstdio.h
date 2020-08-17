#pragma once

#include "config.h"

WARNINGS_OFF
#include <cstdio>
WARNINGS_ON

namespace frank::video {

class standard_io {
public:
  virtual ~standard_io();

  virtual int feof(FILE *pipe);
  virtual char *fgets(char *buffer, int buffer_size, FILE *pipe);
  virtual void pclose(FILE *pipe);
  virtual FILE *popen(char const *command, char const *mode);
};

} // namespace frank::video
