#include "C15_CLI.h"

C15_CLI::C15_CLI(C15Synth *synth)
    : m_synth(synth)
{
  m_commands["a"] = [=] {
    // m_host.examineParameter();
    // jh.cmdBufferResponse->insert(pack<::examine_param>(m_host.m_param_status));
  };

  m_commands["b"] = [=] {
    // m_host.examineSignal();
    // jh.cmdBufferResponse->insert(pack<::examine_signal>(m_host.m_signal_status));
  };

  m_commands["c"] = [=] {
    // jh.cmdBufferResponse->insert(pack<::examine_tcd_input_log>(m_host.m_tcd_input_log));
    // m_host.m_tcd_input_log.reset();
  };

  m_commands["d"] = [=] {
    // jh.cmdBufferResponse->insert(pack<SharedStopWatchHandle>(sw));
  };

  m_commands["e"] = [=] {
    // m_host.resetDSP();
    // jh.cmdBufferResponse->insert(pack<std::string>("Reset executed"));
  };

  m_commands["m"] = [=] {
    // if(handle.inMidiBuffer)
    // {
    //unsigned long rxBytes, txBytes;
    //handle.inMidiBuffer->getStat(&rxBytes, &txBytes);
    //std::cout << "Midi: Input Statistics:" << std::endl << "  rx=" << rxBytes << "  txBytes=" << txBytes << std::endl;
    //}
  };

  m_commands["m"] = [=] {
    // if(handle.inMidiBuffer)
    // {
    //unsigned long rxBytes, txBytes;
    //handle.inMidiBuffer->getStat(&rxBytes, &txBytes);
    //std::cout << "Midi: Input Statistics:" << std::endl << "  rx=" << rxBytes << "  txBytes=" << txBytes << std::endl;
    //}
  };

  m_commands["n"] = [=] {
    //    if(handle.audioOutput)
    //    {
    //      std::cout << "Audio: Output Statistics:" << std::endl << handle.audioOutput->getStats() << std::endl;
    //    }
    //    if(handle.audioInput)
    //    {
    //      std::cout << "Audio: Input Statistics:" << std::endl << handle.audioInput->getStats() << std::endl;
    //    }
  };

  m_commands["t"] = [=] {
    //    tmp = static_cast<float>(1 - m_host.m_test_tone.m_state);
    //    m_host.m_decoder.m_utilityId = 4;
    //    m_host.utilityUpdate(tmp);
    //    std::cout << "Test Tone toggled(" << m_host.m_test_tone.m_state << ")" << std::endl;
  };

  m_commands["z"] = [=] {
    //    m_host.m_test_tone.m_focus = 0;
    //    std::cout << "Test Tone: Frequency:\t" << m_host.m_test_tone.a_frequency << " Hz" << std::endl;
  };

  m_commands["u"] = [=] {
    //      m_host.m_test_tone.m_focus = 1;
    //      std::cout << "Test Tone: Amplitude:\t" << m_host.m_test_tone.a_amplitude << " dB" << std::endl;
  };

  m_commands["+"] = [=] {
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

  m_commands["-"] = [=] {
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
}

void C15_CLI::processLine(const std::string &line)
{
  auto it = m_commands.find(line);
  if(it != m_commands.end())
    it->second();
}
