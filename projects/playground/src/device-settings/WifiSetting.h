#pragma once

#include "BooleanSetting.h"

class WifiSetting : public BooleanSetting, public sigc::trackable
{
 public:
  typedef BooleanSetting super;
  explicit WifiSetting(UpdateDocumentContributor& settings);

  bool set(tEnum m) override;

 private:
  void pollAccessPointRunning();
  void onPollReturned(GPid pid, int result);
};
