#include "config.h"

WARNINGS_OFF
#include <cstdio>
WARNINGS_ON

#if defined(_DO_NOTHING_STDIO_)
#include "test/do_nothing_cstdio.h"
#endif

#include "testable_cstdio.h"

namespace frank::video {

#if !defined(_DO_NOTHING_STDIO_)

int feof(FILE *pipe, void *) { return ::feof(pipe); }

char *fgets(char *buffer, int buffer_size, FILE *pipe, void *) {
  return ::fgets(buffer, buffer_size, pipe);
}

FILE *popen(char const *command, char const *mode, void *) {
#if defined(WIN32)
  return ::_popen(command, mode);
#else
  return ::popen(command, mode);
#endif
}

void pclose(FILE *pipe, void *) {
#if defined(WIN32)
  ::_pclose(pipe);
#else
  ::pclose(pipe);
#endif
}

#else

int feof(FILE *pipe, void *mock_data) {
  return test::frank::do_nothing_feof(pipe, mock_data);
}

char *fgets(char *buffer, int buffer_size, FILE *pipe, void *mock_data) {
  return test::frank::do_nothing_fgets(buffer, buffer_size, pipe, mock_data);
}

FILE *popen(char const *command, char const *mode, void *mock_data) {
  return test::frank::do_nothing_popen(command, mode, mock_data);
}

void pclose(FILE *pipe, void *mock_data) {
  test::frank::do_nothing_pclose(pipe, mock_data);
}

#endif

} // namespace frank::video
