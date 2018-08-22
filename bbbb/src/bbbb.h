#pragma once

#include <glib.h>
#include <glibmm.h>
#include <giomm.h>
#include <sigc++/sigc++.h>
#include <sigc++/trackable.h>
#include <memory>
#include <atomic>
#include <stdio.h>
#include <iostream>

#define TRACE_LOC_AND_ARGS(loc, arg) std::cout << loc << ": " << arg << std::endl
#define TRACE(arg) TRACE_LOC_AND_ARGS(G_STRLOC, arg)
#define TRACE_CALL() TRACE_LOC_AND_ARGS(G_STRLOC, "called")

enum class Domain
  : uint8_t
  {
    Lpc = 1, Buttons = 2, Oled = 3, Rotary = 4, PanelLed = 5, RibbonLed = 6
};
