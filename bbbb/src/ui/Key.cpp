#include <nltools/messaging/Message.h>
#include "Key.h"

Key::Key(int keyPos) : m_keyPos{keyPos}
{
  set_tooltip_text(std::to_string(keyPos));
}

void Key::on_pressed()
{
  nltools::msg::Keyboard::NoteDown msg{m_keyPos};
  nltools::msg::send(nltools::msg::EndPoint::AudioEngine, msg);
}

void Key::on_released()
{
  nltools::msg::Keyboard::NoteUp msg{m_keyPos};
  nltools::msg::send(nltools::msg::EndPoint::AudioEngine, msg);
}
