#include "C15_CLI.h"
#include "synth/C15Synth.h"
#include "io/audio/AudioOutput.h"
#include <nltools/logging/Log.h>
#include <iostream>

C15_CLI::C15_CLI(C15Synth *synth, AudioOutput *audioOut)
{
  using namespace std::chrono_literals;

  m_commands['c'] = [=] { synth->logStatus(); };
  m_commands['e'] = [=] { synth->resetDSP(); };
  m_commands['i'] = [=] {
    // setup settings so tcd and midi are enabled
    nltools::msg::Setting::MidiSettingsMessage msg {};
    msg.receiveChannel = MidiReceiveChannel::Omni;
    for(auto &routing : msg.routings)
      std::fill(routing.begin(), routing.end(), true);
    synth->onMidiSettingsMessage(msg);
    // recall initial Single Preset (with Output Mixer > A - Level at 50%)
    synth->onSettingInitialSinglePreset();
  };
  m_commands['a'] = [=] {
    synth->doTcd({ 0xED, 0, 60 });     // key pos 60
    synth->doTcd({ 0xEE, 127, 127 });  // key down 100% vel
  };
  m_commands['s'] = [=] {
    synth->doTcd({ 0xED, 0, 60 });     // key pos 60
    synth->doTcd({ 0xEF, 127, 127 });  // key up 100% vel
  };

  m_commands['r'] = [=] {
    if(audioOut)
      audioOut->resetPerformance();
  };

  m_commands['q'] = [=] {
    nltools::Log::info("Quitting application...");
    exit(0);
  };
}

void C15_CLI::processByte(char c)
{
  auto it = m_commands.find(c);
  if(it != m_commands.end())
    it->second();
}
