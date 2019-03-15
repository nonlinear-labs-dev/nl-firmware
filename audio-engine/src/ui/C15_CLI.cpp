#include "C15_CLI.h"
#include "main.h"
#include "synth/C15Synth.h"
#include <iostream>

C15_CLI::C15_CLI(C15Synth *synth)
    : m_synth(synth)
{
  m_commands['c'] = [=] { synth->printAndResetTcdInputLog(); };
  m_commands['e'] = [=] { synth->resetDSP(); };
  m_commands['t'] = [=] { synth->toggleTestTone(); };
  m_commands['z'] = [=] { synth->selectTestToneFrequency(); };

  m_commands['u'] = [=] {
    //      m_host.m_test_tone.m_focus = 1;
    //      std::cout << "Test Tone: Amplitude:\t" << m_host.m_test_tone.a_amplitude << " dB" << std::endl;
  };

  m_commands['+'] = [=] {
    //    switch(m_host.m_test_tone.m_focus)
    //    {
    //      case 0:
    //        m_host.m_decoder.m_utilityId = 2;
    //        tmp = std::clamp(m_host.m_test_tone.a_frequency + 10.f, 0.f, 1000.f);
    //        m_host.utilityUpdate(tmp);
    //        std::cout << "Test Tone: Frequency:\t" << tmp << " Hz" << std::endl;
    //        break;
    //      case 1:
    //        m_host.m_decoder.m_utilityId = 3;
    //        tmp = std::clamp(m_host.m_test_tone.a_amplitude + 1.f, -60.f, 0.f);
    //        m_host.utilityUpdate(tmp);
    //        std::cout << "Test Tone: Amplitude:\t" << tmp << " dB" << std::endl;
    //        break;
    //    }
  };

  m_commands['-'] = [=] {
    //    switch(m_host.m_test_tone.m_focus)
    //    {
    //      case 0:
    //        m_host.m_decoder.m_utilityId = 2;
    //        tmp = std::clamp(m_host.m_test_tone.a_frequency - 10.f, 0.f, 1000.f);
    //        m_host.utilityUpdate(tmp);
    //        std::cout << "Test Tone: Frequency:\t" << tmp << " Hz" << std::endl;
    //        break;
    //      case 1:
    //        m_host.m_decoder.m_utilityId = 3;
    //        tmp = std::clamp(m_host.m_test_tone.a_amplitude - 1.f, -60.f, 0.f);
    //        m_host.utilityUpdate(tmp);
    //        std::cout << "Test Tone: Amplitude:\t" << tmp << " dB" << std::endl;
    //        break;
    //    }
  };

  m_commands['q'] = [=] {
    std::cout << "Quitting application..." << std::endl;
    quit(0);
  };
}

void C15_CLI::processByte(char c)
{
  auto it = m_commands.find(c);
  if(it != m_commands.end())
    it->second();
}
