#pragma once

// volatile needed to keep compiler from optimizing away this string
static volatile char C15_VERSION_STRING_IN_MEMORY[] = "\n\nC15 Version: YYYY-MM-DD\0\n\n";
#warning "make sure version string is up-to date when building a release"

static inline volatile char* GetC15Version(void)
{
  return &(C15_VERSION_STRING_IN_MEMORY[15]);  // extract date only
}
