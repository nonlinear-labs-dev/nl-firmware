#include <testing/TestDriver.h>
#include "NoteWithOctaveDimension.h"

Glib::ustring NoteWithOctaveDimension::stringizeNote(int note)
{
  const std::array<const char *, NoteWithOctaveDimension::KeysPerOctave> keyLabels
      = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

  note = std::max(0, std::min(note, NoteWithOctaveDimension::KeyCount));
  auto index = note % keyLabels.size();
  auto base = keyLabels[index];
  auto octave = static_cast<int>(note / keyLabels.size()) + 1;

  return base + std::to_string(octave);
}

Glib::ustring NoteWithOctaveDimension::stringize(const tDisplayValue &displayValue) const
{
  return stringizeNote(static_cast<int>(displayValue * NoteWithOctaveDimension::KeyCount));
}

Glib::ustring NoteWithOctaveDimension::getStingizerJS() const
{
#warning "TODO!"
  return "formatNote(cpValue)";
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
    g_assert(dim.stringizeNote(0) == "C1");
    g_assert(dim.stringizeNote(1) == "C#1");
    g_assert(dim.stringizeNote(60) == "C6");
    g_assert(dim.stringizeNote(30) == "F#3");
    g_assert(dim.stringizeNote(29) == "F3");
  });
}

static TestDriver<NoteWithOctaveDimension> driver;
