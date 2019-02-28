#include "MidiInput.h"

MidiInput::MidiInput(Callback cb)
    : m_cb(cb)
{
}

MidiInput::~MidiInput() = default;
