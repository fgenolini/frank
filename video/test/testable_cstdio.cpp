#include "config.h"

WARNINGS_OFF
#include <cstdio>
WARNINGS_ON

#include "testable_cstdio.h"

namespace frank::video {

standard_io::~standard_io() {}

#if !defined(_DO_NOTHING_STDIO_)
int standard_io::feof(FILE *pipe) { return ::feof(pipe); }
#else
int standard_io::feof(FILE *) { return 0; }
#endif

#if !defined(_DO_NOTHING_STDIO_)
char *standard_io::fgets(char *buffer, int buffer_size, FILE *pipe) {
  return ::fgets(buffer, buffer_size, pipe);
}
#else
char *standard_io::fgets(char *, int, FILE *) { return nullptr; }
#endif

#if !defined(_DO_NOTHING_STDIO_)
FILE *standard_io::popen(char const *command, char const *mode) {
#if defined(WIN32)
  return ::_popen(command, mode);
#else
  return ::popen(command, mode);
#endif
}
#else
FILE *standard_io::popen(char const *, char const *) { return nullptr; }
#endif

#if !defined(_DO_NOTHING_STDIO_)
void standard_io::pclose(FILE *pipe) {
#if defined(WIN32)
  ::_pclose(pipe);
#else
  ::pclose(pipe);
#endif
}
#else
void standard_io::pclose(FILE *) {}
#endif

} // namespace frank::video
