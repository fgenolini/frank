#pragma once

#include "config.h"

WARNINGS_OFF
#include <cstdio>
WARNINGS_ON

namespace frank::video {

extern int feof(FILE *pipe, void *mock_data = nullptr);
extern char *fgets(char *buffer, int buffer_size, FILE *pipe,
                   void *mock_data = nullptr);
extern void pclose(FILE *pipe, void *mock_data = nullptr);
extern FILE *popen(char const *command, char const *mode,
                   void *mock_data = nullptr);

} // namespace frank::video
