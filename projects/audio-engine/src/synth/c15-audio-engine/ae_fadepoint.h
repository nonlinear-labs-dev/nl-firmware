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
  std::atomic<uint32_t> m_muteTasks = 0;
  atomic_fade_table();
  void init(const float _samplerate);
  bool getTaskStatus();
  void setTask(const MuteTask _task);
  float getValue();

 private:
  uint32_t m_currentMuteRampIndex = 0, m_finalMuteRampIndex = 0;
  std::vector<float> m_data;
};
