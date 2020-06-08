#pragma once

// volatile needed to keep compiler from optimizing away this string
static volatile char C15_VERSION_STRING_IN_MEMORY[] = "\n\nC15 Version: YYYY-MM-DD\0\n\n";
#warning "make sure version string is up-to date when building a release"

static volatile char C15_BUILD_STRING_IN_MEMORY[] = "\n\nthis C15 program was built on " __DATE__ " " __TIME__" \0\n\n";

static inline volatile char* GetC15Version(void)
{
  return &(C15_VERSION_STRING_IN_MEMORY[15]);  // extract date only
}

static inline volatile char* GetC15Build(void)
{
  return &(C15_BUILD_STRING_IN_MEMORY[32]);  // extract date only
}
