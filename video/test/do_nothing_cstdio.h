#pragma once

#include "config.h"

WARNINGS_OFF
#include <cstdio>
WARNINGS_ON

namespace test::frank {

extern int do_nothing_feof(FILE *pipe, void *mock_data);
extern char *do_nothing_fgets(char *buffer, int buffer_size, FILE *pipe,
                              void *mock_data);
extern FILE *do_nothing_popen(char const *command, char const *mode,
                              void *mock_data);
extern void do_nothing_pclose(FILE *pipe, void *mock_data);

} // namespace test::frank
