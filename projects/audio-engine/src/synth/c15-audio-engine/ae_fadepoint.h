#pragma once

/******************************************************************************/
/** @file       ae_fadepoint.h
    @date
    @version    1.7-4
    @author     M. Seeber
    @brief      fadepoint generator
    @todo
*******************************************************************************/

#include <c15_config.h>
#include <vector>
#include <atomic>

// current FadeTable implementation

//enum class FadeEvent
//{
//  None,
//  RecallMute,
//  ToneMute,
//  UnisonMute,
//  MonoMute,
//  Unmute  // unmute is common
//};

//class ae_fade_table
//{
// public:
//  FadeEvent m_event;
//  float m_value;
//  ae_fade_table();
//  void init(const float _samplerate);
//  bool enable(const FadeEvent _event, const uint32_t _in_or_out);
//  bool get_state();
//  void render();
//  void stop();

// private:
//  std::vector<float> m_data;
//  uint32_t m_table_index, m_table_offset, m_trigger_index;
//  bool m_trigger;
//};

// std::atomic variation of FadeTable

enum MuteTask
{
  MuteTask_Trigger_Tone = 1 << 0,
  MuteTask_Trigger_Unison = 1 << 1,
  MuteTask_Trigger_Mono = 1 << 2,
  MuteTask_Recall_Single = 1 << 3,
  MuteTask_Recall_Split = 1 << 4,
  MuteTask_Recall_Layer = 1 << 5
};

class atomic_fade_table
{
 public:
  uint32_t m_currentMuteRampIndex = 0;
  std::atomic<uint32_t> m_muteTasks = 0;
  atomic_fade_table();
  void init(const float _samplerate);
  uint32_t getTargetRampIndex() const;
  void setTask(const MuteTask _task);
  float getValue();

 private:
  uint32_t m_finalMuteRampIndex = 0;
  std::vector<float> m_data;
};

// current Fader implementation

//class ae_fader
//{
// public:
//  uint32_t m_preloaded_layerId;  // context-agnostic (mono and unison) latch for layer focus
//  float m_preloaded_position;    // context-agnostic (mono and unison) latch for changed parameter position
//  ae_fader();
//  void init(float* _fade_table);
//  void pick(const uint32_t _index);
//  float get_value();
//  void stop();

// private:
//  float* m_data[3];
//  float m_zero = 0.0f;
//  float m_one = 1.0f;
//  uint32_t m_index, m_current;
//};
