#pragma once
#include <nltools/Types.h>

class EditBuffer;
class Parameter;

class EditBufferModifierSharedBase
{
 public:
  EditBufferModifierSharedBase(EditBuffer &eb);

 protected:
  VoiceGroup invert(VoiceGroup vg);
  template <typename T> std::vector<T> combine(const std::vector<T> &lhs, const std::vector<T> &rhs)
  {
    std::vector<T> ret;
    for(const auto &v : { lhs, rhs })
      for(auto &l : v)
        ret.emplace_back(l);
    return ret;
  }

  [[nodiscard]] std::vector<Parameter *> getCrossFBParameters(const VoiceGroup &to) const;

  EditBuffer &m_editBuffer;
};