#pragma once

#include <nltools/Types.h>

namespace C15
{

  namespace Placeholder
  {

    using Text = char const *const;

    static constexpr Text Qualifier = "@";
    static constexpr Text None = "";

    class Replacer
    {
     protected:
      class VoiceGroupReplacer
      {
        static constexpr size_t s_size = 2;  // probably temporary: I, II
        using Replacement = Text[s_size];
        const Replacement m_replacement;

       public:
        inline constexpr VoiceGroupReplacer(const Text &_both)
            : m_replacement{ _both, _both }
        {
        }
        inline constexpr VoiceGroupReplacer(const Text &_I, const Text &_II)
            : m_replacement{ _I, _II }
        {
        }
        inline const Text &getReplacement(const VoiceGroup &_vg) const
        {
          if((size_t) _vg < s_size)
            return m_replacement[(bool) _vg];
          return None;
        }
      };
      class SoundTypeReplacer
      {
        static constexpr size_t s_size = 3;  // probably temporary: single, split, layer
        using Replacement = VoiceGroupReplacer[s_size];
        const Replacement m_replacement;

       public:
        inline constexpr SoundTypeReplacer(const VoiceGroupReplacer &_single, const VoiceGroupReplacer &_split,
                                           const VoiceGroupReplacer &_layer)
            : m_replacement{ _single, _split, _layer }
        {
        }
        inline const Text &getReplacement(const SoundType &_st, const VoiceGroup &_vg) const
        {
          if((size_t) _st < s_size)
            return m_replacement[(size_t) _st].getReplacement(_vg);
          return None;
        }
      };

     public:
      const Text m_qualifier;
      static constexpr VoiceGroupReplacer createVoiceGroupReplacer(const VoiceGroupReplacer &_vg)
      {
        return _vg;
      }
    };

    class GlobalReplacer : public Replacer
    {
     public:
      static constexpr bool s_isGlobal = true;
      inline const Text &getReplacement() const
      {
        return m_replacement;
      }
      inline constexpr GlobalReplacer(const Text &_qualifier, const Text &_replacement)
          : Replacer{ _qualifier }
          , m_replacement{ _replacement }
      {
      }

     private:
      const Text m_replacement;
    };

    class LocalReplacer : public Replacer
    {
     public:
      static constexpr bool s_isGlobal = false;
      inline const Text &getReplacement(const SoundType &_st, const VoiceGroup &_vg) const
      {
        return m_replacement.getReplacement(_st, _vg);
      }
      inline constexpr LocalReplacer(const Text &_qualifier, const VoiceGroupReplacer &_all)
          : Replacer{ _qualifier }
          , m_replacement{ _all, _all, _all }
      {
      }
      inline constexpr LocalReplacer(const Text &_qualifier, const VoiceGroupReplacer &_single,
                                     const VoiceGroupReplacer &_dual)
          : Replacer{ _qualifier }
          , m_replacement{ _single, _dual, _dual }
      {
      }
      inline constexpr LocalReplacer(const Text &_qualifier, const VoiceGroupReplacer &_single,
                                     const VoiceGroupReplacer &_split, const VoiceGroupReplacer &_layer)
          : Replacer{ _qualifier }
          , m_replacement{ _single, _split, _layer }
      {
      }

     private:
      const SoundTypeReplacer m_replacement;
    };

  }  // namespace C15::Placeholder

}  // namespace C15
