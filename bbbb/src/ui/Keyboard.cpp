#include "Keyboard.h"

Keyboard::Keyboard()
{
  for(auto i = 0; i < 60; i++)
    m_keys[i] = std::make_unique<Key>(36 + i);

  for(auto &key : m_keys)
    add(*key);
}
