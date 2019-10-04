#include <testing/TestDriver.h>
#include "NoteWithOctaveDimension.h"

ustring NoteWithOctaveDimension::stringize(const tDisplayValue &displayValue) const
{
  int key = static_cast<int>(displayValue);
  key = std::max(36, std::min(key, 96));
  const auto foo = std::vector<const char *>{ "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
  auto index = (key - 36) % foo.size();

  auto base = foo[index];
  auto octave = static_cast<int>((key - 36) / foo.size()) + 1;

  return base + std::to_string(octave);
}

const NoteWithOctaveDimension &NoteWithOctaveDimension::get()
{
  static NoteWithOctaveDimension dim;
  return dim;
}

void NoteWithOctaveDimension::registerTests()
{
  g_test_add_func("/NoteWithOctaveDimension/stringize", []() {
    auto dim = get();
    g_assert(dim.stringize(59) == "B2");
    g_assert(dim.stringize(54) == "F#2");
    g_assert(dim.stringize(36) == "C1");
    g_assert(dim.stringize(66) == "F#3");
    g_assert(dim.stringize(96) == "C6");
  });
}

static TestDriver<NoteWithOctaveDimension> driver;