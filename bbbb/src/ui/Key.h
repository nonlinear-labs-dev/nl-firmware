#pragma once
#include <gtkmm-3.0/gtkmm.h>

class Key : public Gtk::Button
{
public:
 Key(int keyPos);

    void on_pressed() override;

    void on_released() override;

private:
    int m_keyPos;
};
