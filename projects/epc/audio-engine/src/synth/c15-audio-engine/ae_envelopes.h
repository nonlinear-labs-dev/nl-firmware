#pragma once

/******************************************************************************/
/** @file       ae_envelopes.h
    @date       2022-08-11
    @version    22-32
    @author     M. Seeber
    @brief      provide envelope generators
    @note       generic provider for different envelope types and features:
                - generic polyphony (mono/poly) support
                - generic curve (linear, finite/infinite exponential,
polynomial) support
                - generic retrigger (soft/hard/variable) support
                - generic segment support (arbitrary: attack, ...)
                - almost generic multichannel (magnitude/timbre) support (env
a/b)
                - almost generic loop support (env c)
*******************************************************************************/

#include "engine.h"
#include "nltoolbox.h"

namespace Engine {

class Envelopes final {

  // class as a namespace wrapper, exposing only required contents
  // (internal implementation details are hidden)

  Envelopes() = delete;

  // RenderMin: prevents exponential rendering from reaching denormals
  static constexpr ScalarValue sRenderMin = 1.e-9f;

  using Unsigned = uint8_t;
  using Signed = int8_t;

public: // exposed types for configuration
  enum class RetriggerType : Unsigned {
    Soft,    // the attack segment starts at arbitrary signal
    Hard,    // the attack segment starts at predefined position
    Variable // the attack segment starts at crossfaded mix
  };
  enum class CurveType : Signed {
    None,        // idle, no curve to render
    Linear,      // linear progression
    Exponential, // exponential progression
    Polynomial,  // polynomial progression, curvature applicable
    InfiniteExponential = -Exponential // decay 2 requires true infinite segment
  };
  enum ChannelType : Unsigned {
    Singular, // only one channel (magnitude)
    Elevate   // elevateable channels (magnitude, timbre)
  };

private: // internal trait declarations
  // trait Retrigger
  template <RetriggerType>
  static inline ScalarValue applyRetrigger(const ScalarValue &_signal,
                                           const ScalarValue &_from,
                                           const ScalarValue &_fade);

  // trait Curve
  // PolynomialExponent: integral curvature parameter (x^(2^Exponent))
  static constexpr Unsigned sPolynomialExponent = 4;
  template <Unsigned Exponent = 1>
  static inline ScalarValue applyCurvature(const ScalarValue &_signal,
                                           const ScalarValue &_curvature) {
    if constexpr (Exponent == 0)
      return _signal;
    ScalarValue ret = _signal;
    for (Unsigned i = 0; i < Exponent; i++)
      ret *= (1.0f + (_curvature * (std::abs(ret) - 1.0f)));
    return ret;
  }
  static inline constexpr bool isFiniteCurve(const CurveType &_curve) {
    return _curve > CurveType::None;
  };
  // determineCurveType: reduce type to basic renderable type
  // (None, Linear, Exponential, Polynomial)
  static inline constexpr CurveType
  determineCurveType(const bool &_finite, const CurveType &_curve) {
    return _finite ? _curve : (CurveType) - (Signed)_curve;
  };
  template <CurveType>
  static inline ScalarValue renderCurve(ScalarValue &_x, ScalarValue &_y,
                                        const ScalarValue &_dx,
                                        const ScalarValue &_curvature = 0.0f);
  template <CurveType>
  static inline bool isTransitionIncomplete(const ScalarValue &_x,
                                            const ScalarValue &_y);

  // trait Channel
  struct Channel {
    template <ChannelType> struct Id;
    template <typename, ChannelType> struct Data;
  };

  // trait Segment
  // Initializer: provides initial segment transition
  struct Initializer {
    inline constexpr Initializer(const ScalarValue &_dx = {},
                                 const ScalarValue &_dest = {},
                                 const ScalarValue &_start = {})
        : mDx{_dx}, mDest{_dest}, mStart{_start} {}
    const ScalarValue mDx;
    const ScalarValue mDest;
    const ScalarValue mStart;
  };
  // Segment: provides type for segment sequence definitions
  template <typename Stage, Stage _Start, Stage _Stop = Stage::Idle,
            Stage _LoopStart = Stage::Idle, Stage _LoopBegin = Stage::Idle,
            Stage _LoopEnd = Stage::Idle>
  struct Segment {
    enum Marker : Signed {
      None = -1,
      Idle,
      Start = (Signed)_Start,
      Stop = (Signed)_Stop,
      LoopStart = (Signed)_LoopStart,
      LoopBegin = (Signed)_LoopBegin,
      LoopEnd = (Signed)_LoopEnd
    };
    inline constexpr Segment(const CurveType &_curve = CurveType::None,
                             const Marker &_marker = None,
                             const Stage &_next = Stage::Idle,
                             const Initializer &_init = {})
        : mCurve{_curve}, mMarker{_marker}, mNext{_next}, mInit{_init} {}
    const CurveType mCurve;
    const Marker mMarker;
    const Stage mNext;
    const Initializer mInit;
  };

  // Wrapper (for monophonic: ScalarValue or polyphonic: PolyValue types)
  template <typename> struct Wrapper;

  // Envelope base (generic unit capable of rendering arbitrary configurations)
  template <typename, typename, typename, ChannelType, RetriggerType>
  class Envelope;

public: // exposed usable envelope types
        // (template parameters: mono/poly type, [retrigger type])
  template <typename> class Decay;
  template <typename> class Gate;
  template <typename, RetriggerType> class ElevatingADBDSR;
  template <typename, RetriggerType> class LoopableADBDSR;
};

// implementation details

// impl Retrigger
template <>
inline ScalarValue Envelopes::applyRetrigger<Envelopes::RetriggerType::Soft>(
    const ScalarValue &_signal, const ScalarValue &, const ScalarValue &) {
  // soft: result is current signal
  return _signal;
}
template <>
inline ScalarValue Envelopes::applyRetrigger<Envelopes::RetriggerType::Hard>(
    const ScalarValue &, const ScalarValue &_from, const ScalarValue &) {
  // hard: result is predefined position
  return _from;
}
template <>
inline ScalarValue Envelopes::applyRetrigger<Envelopes::RetriggerType::Variable>(
    const ScalarValue &_signal, const ScalarValue &_from,
    const ScalarValue &_fade) {
  // variable: result is crossfade between signal and predefined position
  return NlToolbox::Crossfades::unipolarCrossFade(_signal, _from, _fade);
}

// impl Curve
template <>
inline ScalarValue Envelopes::renderCurve<Envelopes::CurveType::Linear>(
    ScalarValue &_x, ScalarValue &, const ScalarValue &_dx,
    const ScalarValue &) {
  const ScalarValue ret = _x;
  _x += _dx;
  return ret;
}
template <>
inline ScalarValue Envelopes::renderCurve<Envelopes::CurveType::Exponential>(
    ScalarValue &, ScalarValue &_y, const ScalarValue &_dx,
    const ScalarValue &) {
  const ScalarValue ret = 1.0f - _y;
  _y *= 1.0f - _dx;
  return ret;
}
template <>
inline ScalarValue Envelopes::renderCurve<Envelopes::CurveType::Polynomial>(
    ScalarValue &_x, ScalarValue &, const ScalarValue &_dx,
    const ScalarValue &_curvature) {
  const ScalarValue ret = applyCurvature<sPolynomialExponent>(_x, _curvature);
  _x += _dx;
  return ret;
}
template <>
inline bool Envelopes::isTransitionIncomplete<Envelopes::CurveType::Linear>(
    const ScalarValue &_x, const ScalarValue &) {
  return 1.0f > _x;
};
template <>
inline bool Envelopes::isTransitionIncomplete<Envelopes::CurveType::Exponential>(
    const ScalarValue &, const ScalarValue &_y) {
  return _y > sRenderMin;
};
template <>
inline bool Envelopes::isTransitionIncomplete<Envelopes::CurveType::Polynomial>(
    const ScalarValue &_x, const ScalarValue &) {
  return 1.0f > _x;
};

// impl Channel
template <> struct Envelopes::Channel::Id<Envelopes::ChannelType::Singular> {
  enum ChannelId : Unsigned { Magnitude, _Length_ };
};
template <> struct Envelopes::Channel::Id<Envelopes::ChannelType::Elevate> {
  enum ChannelId : Unsigned { Magnitude, Timbre, _Length_ };
};
template <typename T, Envelopes::ChannelType C>
struct Envelopes::Channel::Data {
  // Channel::Data: holds monophonic or polyphonic values within array,
  // acccessible via index enum (ChannelId)
  using ChannelId = typename Id<C>::ChannelId;
  // a specific voice can be transformed per channel via Scalar type and
  // callbacks
  using TransformCb = std::function<ScalarValue(const Unsigned &)>;
  struct Scalar {
    ScalarValue mData[ChannelId::_Length_] = {};
    inline void apply(const VoiceId &_voiceId,
                      const TransformCb &_transformCb) {
      for (Unsigned c = 0; c < ChannelId::_Length_; c++)
        mData[c] = _transformCb(c);
    }
    inline ScalarValue &operator[](const Unsigned &_channelId) {
      return mData[_channelId];
    }

    inline const ScalarValue &operator[](const Unsigned &_channelId) const {
      return mData[_channelId];
    }
  };
  using Value = T[ChannelId::_Length_];
  Value mData = {};
  inline void apply(const VoiceId &_voiceId, const TransformCb &_transformCb) {
    for (Unsigned c = 0; c < ChannelId::_Length_; c++)
      mData[c][_voiceId] = _transformCb(c);
  }
};

// impl Wrapper
template <> struct Envelopes::Wrapper<ScalarValue> {
  static constexpr VoiceId sVoices = 1;
  // a monophonic (float) still requires an array wrapper,
  // also allowing for generic VoiceId access
  struct Data {
    inline ScalarValue &operator[](const VoiceId &_voiceId) {
      return m_data[_voiceId];
    }
    inline const ScalarValue &operator[](const VoiceId &_voiceId) const {
      return m_data[_voiceId];
    }

  private:
    ScalarValue m_data[sVoices] = {};
  };
};
template <> struct Envelopes::Wrapper<PolyValue> {
  static constexpr VoiceId sVoices = C15::Config::local_polyphony;
  using Data = PolyValue;
};

// impl Envelope
template <typename T, typename SID, typename SegmentData,
          Envelopes::ChannelType C, Envelopes::RetriggerType R>
class Envelopes::Envelope {
  // gathering required types
  using WrapperType = Wrapper<T>;

protected: // exposing required types for inheritance
  using WrapperData = typename WrapperType::Data;
  using ChannelData = Channel::Data<WrapperData, C>;
  using ChannelScalar = typename ChannelData::Scalar;
  using MarkerId = typename SegmentData::Marker;

public: // exposing requires types and configuration for access
  using ChannelId = typename ChannelData::ChannelId;
  using SegmentId = SID;
  static constexpr Unsigned sVoices = WrapperType::sVoices;
  static constexpr Unsigned sChannels = ChannelId::_Length_;
  static constexpr Unsigned sSegments = (Unsigned)SegmentId::_Length_;
  static constexpr RetriggerType sRetrigger = R;

protected:
  // Transition: parameterized state for each segment
  struct Transition {
    ChannelData mDest = {};
    WrapperData mDx = {};
    ScalarValue mCurvature = {}, mRetriggerHardness = {};
  };
  // State: rendering state
  struct State {
    ChannelData mSignal = {}, mStart = {};
    WrapperData mX = {}, mY = {};
    SegmentId mSegmentId[sVoices] = {};
    WrapperData mPeakLevel = {};
  };
  // Constructor: requires segment definition
  inline Envelope(const SegmentData *const &_segments)
      : mSegments{_segments}, mTransitions{}, mState{} {
    for (Unsigned segmentId = 0; segmentId < sSegments; segmentId++) {
      // initialize transitions by segment definitions
      const SegmentData &segment = mSegments[segmentId];
      Transition &transition = mTransitions[segmentId];
      for (Unsigned voiceId = 0; voiceId < sVoices; voiceId++) {
        transition.mDx[voiceId] = segment.mInit.mDx;
        transition.mDest.apply(voiceId, [&segment](const Unsigned &) {
          return segment.mInit.mDest;
        });
      }
    }
  }
  const SegmentData *const mSegments;
  Transition mTransitions[sSegments];
  State mState;
  // determines the current difference (per channel) between start and dest
  // positions
  inline ChannelScalar determineDifference(const VoiceId &_voiceId,
                                           const Unsigned &_segmentId) const {
    ChannelScalar ret;
    ret.apply(
        _voiceId, [this, &_voiceId, &_segmentId](const Unsigned &_channelId) {
          return mTransitions[_segmentId].mDest.mData[_channelId][_voiceId] -
                 mState.mStart.mData[_channelId][_voiceId];
        });
    return ret;
  }
  // core rendering algorithm
  inline bool render(const VoiceId &_voiceId, const Unsigned &_segmentId,
                     const ChannelScalar &_difference,
                     const ScalarValue &_mute) {
    Transition &transition = mTransitions[_segmentId];
    const SegmentData &segment = mSegments[_segmentId];
    const bool isFinite = isFiniteCurve(segment.mCurve);
    const CurveType curve = determineCurveType(isFinite, segment.mCurve);
    ScalarValue rendered = 1.0f;
    bool isIncomplete = true;
    switch (curve) {
    case CurveType::Linear:
      if ((isIncomplete = isTransitionIncomplete<CurveType::Linear>(
               mState.mX[_voiceId], mState.mY[_voiceId]))) {
        rendered = renderCurve<CurveType::Linear>(
            mState.mX[_voiceId], mState.mY[_voiceId], transition.mDx[_voiceId]);
      }
      break;
    case CurveType::Exponential:
      if ((isIncomplete = isTransitionIncomplete<CurveType::Exponential>(
               mState.mX[_voiceId], mState.mY[_voiceId]))) {
        rendered = renderCurve<CurveType::Exponential>(
            mState.mX[_voiceId], mState.mY[_voiceId], transition.mDx[_voiceId]);
        if (segment.mMarker == MarkerId::Stop) {
          // during release, glitch suppression event will accelerate the
          // release rendering, ensuring that the envelope signal is zero at
          // fade-out
          mState.mY[_voiceId] *= _mute;
        }
      }
      break;
    case CurveType::Polynomial:
      if ((isIncomplete = isTransitionIncomplete<CurveType::Polynomial>(
               mState.mX[_voiceId], mState.mY[_voiceId]))) {
        rendered = renderCurve<CurveType::Polynomial>(
            mState.mX[_voiceId], mState.mY[_voiceId], transition.mDx[_voiceId],
            transition.mCurvature);
      }
      break;
    default: // idle
      break;
    }
    // form signal (per channel, signal = start + (difference * rendered))
    mState.mSignal.apply(_voiceId, [this, &_voiceId, &rendered,
                                    &_difference](const Unsigned &_channelId) {
      return mState.mStart.mData[_channelId][_voiceId] +
             (rendered * _difference.mData[_channelId]);
    });
    // determine if transition has finished
    return !isIncomplete && isFinite;
  }
  // set segment destination per channel
  inline void setSegmentDest(const SegmentId &_segmentId,
                             const VoiceId &_voiceId,
                             const ChannelScalar &_dest) {
    const ScalarValue &peakLevel = mState.mPeakLevel[_voiceId];
    mTransitions[(Unsigned)_segmentId].mDest.apply(
        _voiceId, [&_dest, &peakLevel](const Unsigned &_channelId) {
          return peakLevel * _dest[_channelId];
        });
  }
  // progress to next transition/segment
  inline void nextTransition(const VoiceId &_voiceId,
                             const Unsigned &_segmentId) {
    startTransition<RetriggerType::Soft>(_voiceId, _segmentId);
  }

private:
  // templated retrigger parameter means that this method cannot be exposed for
  // inheritance
  template <RetriggerType Retr>
  inline void startTransition(const VoiceId &_voiceId,
                              const Unsigned &_segmentId) {
    mState.mSegmentId[_voiceId] = (SegmentId)_segmentId;
    mState.mStart.apply(_voiceId, [this, &_voiceId,
                                   &_segmentId](const Unsigned &_channelId) {
      return applyRetrigger<Retr>(mState.mSignal.mData[_channelId][_voiceId],
                                  mSegments[_segmentId].mInit.mStart,
                                  mTransitions[_segmentId].mRetriggerHardness);
    });
    const float dx = mTransitions[_segmentId].mDx[_voiceId];
    mState.mX[_voiceId] = dx;
    mState.mY[_voiceId] = 1.0f - dx;
  }

public:
  // render function
  inline void tick(const VoiceId &_voiceId, const ScalarValue &_mute) {
    const Unsigned segmentId = (Unsigned)mState.mSegmentId[_voiceId];
    const ChannelScalar difference = determineDifference(_voiceId, segmentId);
    const bool isComplete = render(_voiceId, segmentId, difference, _mute);
    if (isComplete)
      nextTransition(_voiceId, (Unsigned)mSegments[segmentId].mNext);
  }
  // start/stop (usually attack/release)
  inline void start(const VoiceId &_voiceId,
                    const MarkerId &_markerId = MarkerId::Start) {
    startTransition<R>(_voiceId, (Unsigned)_markerId);
    tick(_voiceId, 1.0f);
  }
  inline void stop(const VoiceId &_voiceId,
                   const MarkerId &_markerId = MarkerId::Stop) {
    startTransition<RetriggerType::Soft>(_voiceId, (Unsigned)_markerId);
  }
  // update segment parameters
  inline void setSegmentDx(const SegmentId &_segmentId, const VoiceId &_voiceId,
                           const ScalarValue &_dx) {
    mTransitions[(Unsigned)_segmentId].mDx[_voiceId] = _dx;
  }
  inline void setSegmentDest(const SegmentId &_segmentId,
                             const VoiceId &_voiceId,
                             const ScalarValue &_dest) {
    const ScalarValue &peakLevel = mState.mPeakLevel[_voiceId];
    mTransitions[(Unsigned)_segmentId].mDest.apply(
        _voiceId, [&_dest, &peakLevel](const Unsigned &_channelId) {
          return peakLevel * _dest;
        });
  }
  inline void setSegmentCurvature(const SegmentId &_segmentId,
                                  const ScalarValue &_curvature) {
    mTransitions[(Unsigned)_segmentId].mCurvature = _curvature;
  }
  // update start segment retrigger hardness (usually attack)
  inline void setRetriggerHardness(const ScalarValue &_retriggerHardness) {
    mTransitions[MarkerId::Start].mRetriggerHardness = _retriggerHardness;
  }
  inline void setPeakLevel(const VoiceId &_voiceId,
                           const ScalarValue &_peakLevel) {
    mState.mPeakLevel[_voiceId] = _peakLevel;
  }
  inline void reset() {
    mState.mX = mState.mY = 0.0f;
    const auto cb = [](const Unsigned &) { return 0.0f; };
    for (Unsigned voiceId = 0; voiceId < sVoices; voiceId++) {
      mState.mPeakLevel[voiceId] = 0.0f;
      mState.mSegmentId[voiceId] = SegmentId::Idle;
      mState.mSignal.apply(voiceId, cb);
      mState.mStart.apply(voiceId, cb);
    }
  }
  // signal retrieval
  inline WrapperData &getSignal(const ChannelId &_channelId) {
    return mState.mSignal.mData[_channelId];
  }
  inline const WrapperData &getSignal(const ChannelId &_channelId) const {
    return mState.mSignal.mData[_channelId];
  }
  inline ScalarValue &getSignal(const ChannelId &_channelId,
                                const VoiceId &_voiceId) {
    return mState.mSignal.mData[_channelId][_voiceId];
  }
  inline const ScalarValue &getSignal(const ChannelId &_channelId,
                                      const VoiceId &_voiceId) const {
    return mState.mSignal.mData[_channelId][_voiceId];
  }
};

// usable envelopes

// impl Decay Envelope
template <typename T> class Envelopes::Decay {
  Decay() = delete;
  enum class Stage : Unsigned { Idle, Trigger, Decay, _Length_ };
  using Segment = Envelopes::Segment<Stage, Stage::Trigger, Stage::Idle>;
  using Super =
      Envelope<T, Stage, Segment, ChannelType::Singular, RetriggerType::Soft>;
  static constexpr Segment sSegments[] = {
      {},
      {CurveType::Linear, Segment::Start, Stage::Decay, {1.0f, 1.0f}},
      {CurveType::Exponential, Segment::None, Stage::Idle}};

public:
  class Impl : public Super {
  public:
    inline Impl() : Super{sSegments} {}
  };
};

// impl Gate Envelope
template <typename T> class Envelopes::Gate {
  Gate() = delete;
  enum class Stage : Unsigned { Idle, Hold, Release, _Length_ };
  using Segment = Envelopes::Segment<Stage, Stage::Hold, Stage::Release>;
  using Super =
      Envelope<T, Stage, Segment, ChannelType::Singular, RetriggerType::Hard>;
  static constexpr Segment sSegments[] = {
      {},
      {CurveType::None, Segment::Start, Stage::Idle, {0.0f, 1.0f, 1.0f}},
      {CurveType::Exponential, Segment::Stop, Stage::Idle}};

public:
  class Impl : public Super {
  public:
    inline Impl() : Super{sSegments} {}
  };
};

// impl ElevatingADBDSR Envelope
template <typename T, Envelopes::RetriggerType R>
class Envelopes::ElevatingADBDSR {
  ElevatingADBDSR() = delete;
  enum class Stage : Unsigned {
    Idle,
    Attack,
    Decay1,
    Decay2,
    Release,
    _Length_
  };
  using Segment = Envelopes::Segment<Stage, Stage::Attack, Stage::Release>;
  using Super = Envelope<T, Stage, Segment, ChannelType::Elevate, R>;
  using ChannelId = typename Super::ChannelId;
  static constexpr Segment sSegments[] = {
      {},
      {CurveType::Polynomial, Segment::Start, Stage::Decay1, {0.0f, 1.0f}},
      {CurveType::Linear, Segment::None, Stage::Decay2},
      {CurveType::InfiniteExponential, Segment::None, Stage::Idle},
      {CurveType::Exponential, Segment::Stop, Stage::Idle}};
  static constexpr bool sElevateableSegments[] = {false, false, true, true,
                                                  false};

public:
  class Impl : public Super {
    ScalarValue mElevation[Super::sChannels] = {};

  public:
    inline Impl() : Super{sSegments} {}
    inline void setElevation(const ScalarValue &_elevation) {
      mElevation[ChannelId::Magnitude] = std::max(0.0f, _elevation);
      mElevation[ChannelId::Timbre] = std::max(0.0f, -_elevation);
    }
    inline void setSegmentDest(const Stage &_segmentId, const VoiceId &_voiceId,
                               const ScalarValue &_dest) {
      const Unsigned segmentId = (Unsigned)_segmentId;
      if (sElevateableSegments[segmentId]) {
        Super::setSegmentDest(
            _segmentId, _voiceId,
            {NlToolbox::Crossfades::unipolarCrossFade(
                 _dest, 1.0f, mElevation[ChannelId::Magnitude]),
             NlToolbox::Crossfades::unipolarCrossFade(
                 _dest, 1.0f, mElevation[ChannelId::Timbre])});
      } else {
        Super::setSegmentDest(_segmentId, _voiceId, _dest);
      }
    }
  };
};

// impl LoopableADBDSR Envelope
template <typename T, Envelopes::RetriggerType R>
class Envelopes::LoopableADBDSR {
  LoopableADBDSR() = delete;
  enum class Stage : Unsigned {
    Idle,
    Attack,
    Decay1,
    Decay2,
    Release,
    LoopAttack,
    LoopDecay1,
    LoopDecay2,
    _Length_
  };
  enum class Loop : Unsigned { None, Hold, Release };
  using SegmentBase = Envelopes::Segment<Stage, Stage::Attack, Stage::Release,
                                         Stage::LoopAttack, Stage::LoopDecay1,
                                         Stage::LoopDecay2>;
  using MarkerId = typename SegmentBase::Marker;
  struct Segment : public SegmentBase {
    inline constexpr Segment(const CurveType &_curve = CurveType::None,
                             const MarkerId &_marker = MarkerId::None,
                             const Stage &_sibling = Stage::Idle,
                             const Stage &_next = Stage::Idle,
                             const Initializer &_init = {})
        : SegmentBase{_curve, _marker, _next, _init}, mSibling{_sibling} {}
    const Stage mSibling;
  };
  using Super = Envelope<T, Stage, Segment, ChannelType::Singular, R>;
  using Sibling = Stage;
  static constexpr Segment sSegments[] = {
      {},
      {CurveType::Polynomial,
       Segment::Start,
       Sibling::LoopAttack,
       Stage::Decay1,
       {0.0f, 1.0f}},
      {CurveType::Linear, Segment::None, Sibling::LoopDecay1, Stage::Decay2},
      {CurveType::InfiniteExponential, Segment::None, Sibling::LoopDecay2,
       Stage::Idle},
      {CurveType::Exponential, Segment::Stop, Sibling::Idle, Stage::Idle},
      {CurveType::Polynomial,
       Segment::LoopStart,
       Sibling::Attack,
       Stage::LoopDecay1,
       {0.0f, 1.0f}},
      {CurveType::Linear, Segment::LoopBegin, Sibling::Decay1,
       Stage::LoopDecay2},
      {CurveType::Linear, Segment::LoopEnd, Sibling::Decay2, Stage::LoopDecay1},
  };

public:
  class Impl : public Super {
    using ChannelScalar = typename Super::ChannelScalar;
    using WrapperData = typename Super::WrapperData;
    struct LoopState {
      WrapperData mFactor = {}, mDecay = {}, mRelease = {};
      Loop mLoop[Super::sVoices] = {};
      ScalarValue mValue = {};
    };
    inline ChannelScalar determineDifference(const VoiceId &_voiceId,
                                             const Unsigned &_segmentId,
                                             const ScalarValue &_mute) const {
      ChannelScalar ret;
      ret.apply(_voiceId, [this, &_voiceId, &_segmentId,
                           &_mute](const Unsigned &_channelId) {
        const ScalarValue mute =
            mLoopState.mLoop[_voiceId] == Loop::Release ? _mute : 1.0f;
        const ScalarValue dest =
            Super::mTransitions[_segmentId].mDest.mData[_channelId][_voiceId] *
            mLoopState.mRelease[_voiceId] * mute;
        const ScalarValue start =
            Super::mState.mStart.mData[_channelId][_voiceId];
        const ScalarValue decay =
            Super::mSegments[_segmentId].mMarker == MarkerId::LoopBegin
                ? mLoopState.mDecay[_voiceId]
                : 1.0f;
        const ScalarValue diff = dest - start;
        return diff * decay;
      });
      return ret;
    }
    inline void applyLoopDecay(ScalarValue &_value,
                               const ScalarValue &_factor) {
      if (_value > sRenderMin) {
        _value *= _factor;
      } else {
        _value = 0.0f;
      }
    }

  public:
    using LoopScaleCb = std::function<ScalarValue(const ScalarValue &)>;
    inline Impl() : Super{sSegments} {}
    inline void setLoopFactor(const ScalarValue &_loopFactor) {
      mLoopState.mValue = _loopFactor;
    }
    inline void tick(const VoiceId &_voiceId, const ScalarValue &_mute) {
      const Unsigned segmentId = (Unsigned)Super::mState.mSegmentId[_voiceId];
      const ChannelScalar difference =
          determineDifference(_voiceId, segmentId, _mute);
      const MarkerId markerId = Super::mSegments[segmentId].mMarker;
      const bool isComplete =
          Super::render(_voiceId, segmentId, difference, _mute);
      if (isComplete) {
        switch (mLoopState.mLoop[_voiceId]) {
        case Loop::Hold:
          if (markerId == MarkerId::LoopBegin)
            applyLoopDecay(mLoopState.mDecay[_voiceId],
                           mLoopState.mFactor[_voiceId]);
          break;
        case Loop::Release:
          if (markerId == MarkerId::LoopEnd)
            applyLoopDecay(mLoopState.mRelease[_voiceId],
                           mLoopState.mFactor[_voiceId]);
          break;
        default:
          break;
        }
        Super::nextTransition(_voiceId,
                              (Unsigned)Super::mSegments[segmentId].mNext);
      }
    }
    inline void start(const VoiceId &_voiceId,
                      const LoopScaleCb &_loopScaleCb) {
      mLoopState.mDecay[_voiceId] = mLoopState.mRelease[_voiceId] = 1.0f;
      if (mLoopState.mValue == 0.0f) {
        mLoopState.mLoop[_voiceId] = Loop::None;
        mLoopState.mFactor[_voiceId] = 1.0f;
        Super::start(_voiceId, MarkerId::Start);
      } else {
        mLoopState.mLoop[_voiceId] = Loop::Hold;
        mLoopState.mFactor[_voiceId] =
            _loopScaleCb(mLoopState.mValue > 1.0f ? 1.0f : mLoopState.mValue);
        Super::start(_voiceId, MarkerId::LoopStart);
      }
    }
    inline void stop(const VoiceId &_voiceId,
                     const LoopScaleCb &_loopScaleCb) {
      if (mLoopState.mValue <= 1.0f) {
        mLoopState.mLoop[_voiceId] = Loop::None;
        mLoopState.mFactor[_voiceId] = 1.0f;
        mLoopState.mDecay[_voiceId] = 1.0f;
        Super::stop(_voiceId, MarkerId::Stop);
      } else {
        mLoopState.mLoop[_voiceId] = Loop::Release;
        mLoopState.mFactor[_voiceId] = _loopScaleCb(mLoopState.mValue - 1.0f);
      }
    }
    inline void setSegmentDx(const Stage &_stage, const VoiceId &_voiceId,
                             const ScalarValue &_dx) {
      Super::setSegmentDx(_stage, _voiceId, _dx);
      const Stage &sibling = Super::mSegments[(Unsigned)_stage].mSibling;
      if (sibling != Stage::Idle)
        Super::setSegmentDx(sibling, _voiceId, _dx);
    }
    inline void setSegmentDest(const Stage &_stage, const VoiceId &_voiceId,
                               const ScalarValue &_dest) {
      Super::setSegmentDest(_stage, _voiceId, _dest);
      const Stage &sibling = Super::mSegments[(Unsigned)_stage].mSibling;
      if (sibling != Stage::Idle)
        Super::setSegmentDest(sibling, _voiceId, _dest);
    }

  private:
    LoopState mLoopState = {};
  };
};

} // namespace Engine

/******************************************************************************/
/** @version    1.7-0
    @author     M. Seeber
    @brief      legacy: provide envelope generators
    @note       separate mono and poly engines!
    @todo
*******************************************************************************/

namespace Engine {

namespace LegacyEnvelopes {
namespace Proto {
const uint32_t SegmentType_ADBDSR[4][4] = {{4, 2, 0, 0},
                                           {1, 3, 0, 0},
                                           {2, 0, 0, 0},
                                           {3, 0, 0, 0}},
               SegmentType_Gate[4][4] = {{1, 0, 1, 1},
                                         {2, 0, 0, 0},
                                         {0, 0, 0, 0},
                                         {0, 0, 0, 0}},
               SegmentType_Decay[4][4] = {
                   {1, 2, 1, 1}, {2, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
constexpr uint32_t SegSize = 5;
constexpr float renderMin = 1e-9f;

template <uint32_t Size> struct SingleSegment {
  float m_dx[Size] = {}, m_dest_magnitude[Size] = {}, m_curve = 0.0f;
  uint32_t m_state = 0, m_next = 0;
};

template <uint32_t Size> struct SplitSegment {
  float m_dx[Size] = {}, m_dest_magnitude[Size] = {}, m_dest_timbre[Size] = {},
        m_curve = 0.0f;
  uint32_t m_state = 0, m_next = 0;
};

struct EnvBody_Single {
  float m_x = 0.0f, m_y = 0.0f, m_start_magnitude = 0.0f,
        m_signal_magnitude = 0.0f;
  uint32_t m_state = 0, m_next = 0, m_index = 0;
};

struct EnvBody_Split : EnvBody_Single {
  float m_start_timbre = 0.0f, m_signal_timbre = 0.0f;
};
} // namespace Proto

template <uint32_t Size> struct SplitEnvelope {
  Proto::EnvBody_Split m_body[Size];
  Proto::SplitSegment<Size> m_segment[Proto::SegSize];
  float m_timeFactor[Size][Proto::SegSize - 1] = {}, m_levelFactor[Size] = {},
                                            m_splitValues[2] = {};
  const uint32_t m_startIndex = 1, m_stopIndex = 4;
  inline SplitEnvelope() {
    for (uint32_t s = 1; s < Proto::SegSize; s++) {
      const uint32_t segment = s - 1;
      m_segment[s].m_state = Proto::SegmentType_ADBDSR[segment][0];
      m_segment[s].m_next = Proto::SegmentType_ADBDSR[segment][1];
      for (uint32_t v = 0; v < Size; v++) {
        setSegmentDx(v, s,
                     static_cast<float>(Proto::SegmentType_ADBDSR[segment][2]));
        setSegmentDest(
            v, s, false,
            static_cast<float>(Proto::SegmentType_ADBDSR[segment][3]));
        m_levelFactor[v] = 0.0f;
        m_timeFactor[v][segment] = 1.0f;
      }
    }
  }
  inline void start(const uint32_t _voiceId) {
    auto body = &m_body[_voiceId];
    body->m_x = m_segment[m_startIndex].m_dx[_voiceId];
    body->m_y = 1.0f - m_segment[m_startIndex].m_dx[_voiceId];
    body->m_start_magnitude = body->m_signal_magnitude;
    body->m_start_timbre = body->m_signal_timbre;
    body->m_state = m_segment[m_startIndex].m_state;
    body->m_index = m_startIndex;
    body->m_next = m_segment[m_startIndex].m_next;
    tick(_voiceId, 1.0f);
  }
  inline void stop(const uint32_t _voiceId) {
    auto body = &m_body[_voiceId];
    body->m_x = m_segment[m_stopIndex].m_dx[_voiceId];
    body->m_y = 1.0f - m_segment[m_stopIndex].m_dx[_voiceId];
    body->m_start_magnitude = body->m_signal_magnitude;
    body->m_start_timbre = body->m_signal_timbre;
    body->m_state = m_segment[m_stopIndex].m_state;
    body->m_index = m_stopIndex;
    body->m_next = m_segment[m_stopIndex].m_next;
  }
  inline void tick(const uint32_t _voiceId, const float _mute) {
    auto body = &m_body[_voiceId];
    const uint32_t segment = body->m_index;
    const float diff_magnitude =
        m_segment[segment].m_dest_magnitude[_voiceId] - body->m_start_magnitude;
    const float diff_timbre =
        m_segment[segment].m_dest_timbre[_voiceId] - body->m_start_timbre;
    switch (body->m_state) {
    case 0:
      break;
    case 1:
      if (body->m_x < 1.0f) {

        body->m_signal_magnitude =
            body->m_start_magnitude + (diff_magnitude * body->m_x);
        body->m_signal_timbre =
            body->m_start_timbre + (diff_timbre * body->m_x);
        body->m_x += m_segment[segment].m_dx[_voiceId];
      } else {
        body->m_signal_magnitude =
            m_segment[segment].m_dest_magnitude[_voiceId];
        body->m_signal_timbre = m_segment[segment].m_dest_timbre[_voiceId];
        nextSegment(_voiceId);
      }
      break;
    case 2:
      if (body->m_y > Proto::renderMin) {
        body->m_signal_magnitude =
            body->m_start_magnitude + (diff_magnitude * (1.0f - body->m_y));
        body->m_signal_timbre =
            body->m_start_timbre + (diff_timbre * (1.0f - body->m_y));
        body->m_y *= 1.0f - m_segment[segment].m_dx[_voiceId];
      } else {
        body->m_signal_magnitude = body->m_start_magnitude + diff_magnitude;
        body->m_signal_timbre = body->m_start_timbre + diff_timbre;
      }
      break;

    case 3:
      if (body->m_y > Proto::renderMin) {
        body->m_signal_magnitude =
            body->m_start_magnitude + (diff_magnitude * (1.0f - body->m_y));
        body->m_signal_timbre =
            body->m_start_timbre + (diff_timbre * (1.0f - body->m_y));
        body->m_y *= _mute * (1.0f - m_segment[segment].m_dx[_voiceId]);
      } else {
        body->m_signal_magnitude =
            m_segment[segment].m_dest_magnitude[_voiceId];
        body->m_signal_timbre = m_segment[segment].m_dest_timbre[_voiceId];
        nextSegment(_voiceId);
      }
      break;
    case 4:
      if (body->m_x < 1.0f) {
        float x = NlToolbox::Curves::SquaredCurvature(
            body->m_x, m_segment[segment].m_curve);
        x = NlToolbox::Curves::SquaredCurvature(x, m_segment[segment].m_curve);
        x = NlToolbox::Curves::SquaredCurvature(x, m_segment[segment].m_curve);
        x = NlToolbox::Curves::SquaredCurvature(x, m_segment[segment].m_curve);
        body->m_signal_magnitude =
            body->m_start_magnitude + (diff_magnitude * x);
        body->m_signal_timbre = body->m_start_timbre + (diff_timbre * x);
        body->m_x += m_segment[segment].m_dx[_voiceId];
      } else {
        body->m_signal_magnitude =
            m_segment[segment].m_dest_magnitude[_voiceId];
        body->m_signal_timbre = m_segment[segment].m_dest_timbre[_voiceId];
        nextSegment(_voiceId);
      }
      break;
    }
  }
  inline void nextSegment(const uint32_t _voiceId) {
    auto body = &m_body[_voiceId];
    body->m_x = m_segment[body->m_next].m_dx[_voiceId];
    body->m_y = 1.0f - m_segment[body->m_next].m_dx[_voiceId];
    body->m_start_magnitude = body->m_signal_magnitude;
    body->m_start_timbre = body->m_signal_timbre;
    body->m_state = m_segment[body->m_next].m_state;
    body->m_index = body->m_next;
    body->m_next = m_segment[body->m_index].m_next;
  }
  inline void setSegmentDx(const uint32_t _voiceId, const uint32_t _segmentId,
                           const float _value) {
    m_segment[_segmentId].m_dx[_voiceId] = _value;
  }
  inline void setSegmentDest(const uint32_t _voiceId, const uint32_t _segmentId,
                             const bool _splitMode, const float _value) {
    if (_splitMode) {
      m_segment[_segmentId].m_dest_magnitude[_voiceId] =
          NlToolbox::Crossfades::unipolarCrossFade(
              _value, m_levelFactor[_voiceId], m_splitValues[0]);
      m_segment[_segmentId].m_dest_timbre[_voiceId] =
          NlToolbox::Crossfades::unipolarCrossFade(
              _value, m_levelFactor[_voiceId], m_splitValues[1]);
    } else {
      m_segment[_segmentId].m_dest_magnitude[_voiceId] = _value;
      m_segment[_segmentId].m_dest_timbre[_voiceId] = _value;
    }
  }
  inline void setSplitValue(const float _value) {
    m_splitValues[0] = std::max(0.0f, _value);
    m_splitValues[1] = std::max(0.0f, -_value);
  }
  inline void setAttackCurve(const float _value) {
    m_segment[1].m_curve = _value;
  }
  inline void setPeakLevel(const uint32_t _voiceId, const float _value) {
    m_levelFactor[_voiceId] = _value;
  }
  inline void reset() {
    for (uint32_t v = 0; v < Size; v++) {
      m_body[v].m_index = 0;
      m_body[v].m_x = 0.0f;
      m_body[v].m_signal_timbre = 0.0f;
      m_body[v].m_start_timbre = 0.0f;
      m_body[v].m_signal_magnitude = 0.0f;
      m_body[v].m_start_magnitude = 0.0f;
    }
  }
};

template <uint32_t Size> struct RetriggerEnvelope {
  Proto::EnvBody_Single m_body[Size];
  Proto::SingleSegment<Size> m_segment[Proto::SegSize];
  float m_timeFactor[Size][Proto::SegSize - 1] = {}, m_levelFactor[Size] = {},
                                            m_clipFactor[Size] = {};
  const uint32_t m_startIndex = 1, m_stopIndex = 4;
  float m_retriggerHardness = 0.0f;
  inline RetriggerEnvelope() {
    for (uint32_t s = 1; s < Proto::SegSize; s++) {
      const uint32_t segment = s - 1;
      m_segment[s].m_state = Proto::SegmentType_ADBDSR[segment][0];
      m_segment[s].m_next = Proto::SegmentType_ADBDSR[segment][1];
      for (uint32_t v = 0; v < Size; v++) {
        setSegmentDx(v, s,
                     static_cast<float>(Proto::SegmentType_ADBDSR[segment][2]));
        setSegmentDest(
            v, s, static_cast<float>(Proto::SegmentType_ADBDSR[segment][3]));
        m_levelFactor[v] = 0.0f;
        m_clipFactor[v] = 1.0f;
        m_timeFactor[v][segment] = 1.0f;
      }
    }
  }
  inline void start(const uint32_t _voiceId) {
    auto body = &m_body[_voiceId];
    body->m_x = m_segment[m_startIndex].m_dx[_voiceId];
    body->m_y = 1.0f - m_segment[m_startIndex].m_dx[_voiceId];
    body->m_start_magnitude =
        (1.0f - m_retriggerHardness) * body->m_signal_magnitude;
    body->m_state = m_segment[m_startIndex].m_state;
    body->m_index = m_startIndex;
    body->m_next = m_segment[m_startIndex].m_next;
    tick(_voiceId, 1.0f);
  }
  inline void stop(const uint32_t _voiceId) {
    auto body = &m_body[_voiceId];
    body->m_x = m_segment[m_stopIndex].m_dx[_voiceId];
    body->m_y = 1.0f - m_segment[m_stopIndex].m_dx[_voiceId];
    body->m_start_magnitude = body->m_signal_magnitude;
    body->m_state = m_segment[m_stopIndex].m_state;
    body->m_index = m_stopIndex;
    body->m_next = m_segment[m_stopIndex].m_next;
  }
  inline void tick(const uint32_t _voiceId, const float _mute) {
    auto body = &m_body[_voiceId];
    const uint32_t segment = body->m_index;
    const float diff_magnitude =
        m_segment[segment].m_dest_magnitude[_voiceId] - body->m_start_magnitude;
    switch (body->m_state) {
    case 0:
      break;
    case 1:
      if (body->m_x < 1.0f) {
        body->m_signal_magnitude =
            body->m_start_magnitude + (diff_magnitude * body->m_x);
        body->m_x += m_segment[segment].m_dx[_voiceId];
      } else {
        body->m_signal_magnitude =
            m_segment[segment].m_dest_magnitude[_voiceId];
        nextSegment(_voiceId);
      }
      break;
    case 2:
      if (body->m_y > Proto::renderMin) {
        body->m_signal_magnitude =
            body->m_start_magnitude + (diff_magnitude * (1.0f - body->m_y));
        body->m_y *= 1.0f - m_segment[segment].m_dx[_voiceId];
      } else {
        body->m_signal_magnitude = body->m_start_magnitude + diff_magnitude;
      }
      break;
    case 3:
      if (body->m_y > Proto::renderMin) {
        body->m_signal_magnitude =
            body->m_start_magnitude + (diff_magnitude * (1.0f - body->m_y));
        body->m_y *= _mute * (1.0f - m_segment[segment].m_dx[_voiceId]);
      } else {
        body->m_signal_magnitude =
            m_segment[segment].m_dest_magnitude[_voiceId];
        nextSegment(_voiceId);
      }
      break;
    case 4:
      if (body->m_x < 1.0f) {
        float x = NlToolbox::Curves::SquaredCurvature(
            body->m_x, m_segment[segment].m_curve);
        x = NlToolbox::Curves::SquaredCurvature(x, m_segment[segment].m_curve);
        x = NlToolbox::Curves::SquaredCurvature(x, m_segment[segment].m_curve);
        x = NlToolbox::Curves::SquaredCurvature(x, m_segment[segment].m_curve);
        body->m_signal_magnitude =
            body->m_start_magnitude + (diff_magnitude * x);
        body->m_x += m_segment[segment].m_dx[_voiceId];
      } else {
        body->m_signal_magnitude =
            m_segment[segment].m_dest_magnitude[_voiceId];
        nextSegment(_voiceId);
      }
      break;
    }
  }
  inline void nextSegment(const uint32_t _voiceId) {
    auto body = &m_body[_voiceId];
    body->m_x = m_segment[body->m_next].m_dx[_voiceId];
    body->m_y = 1.0f - m_segment[body->m_next].m_dx[_voiceId];
    body->m_start_magnitude = body->m_signal_magnitude;
    body->m_state = m_segment[body->m_next].m_state;
    body->m_index = body->m_next;
    body->m_next = m_segment[body->m_index].m_next;
  }
  inline void setSegmentDx(const uint32_t _voiceId, const uint32_t _segmentId,
                           const float _value) {
    m_segment[_segmentId].m_dx[_voiceId] = _value;
  }
  inline void setSegmentDest(const uint32_t _voiceId, const uint32_t _segmentId,
                             const float _value) {
    m_segment[_segmentId].m_dest_magnitude[_voiceId] = _value;
  }
  inline void setAttackCurve(const float _value) {
    m_segment[1].m_curve = _value;
  }
  inline void setRetriggerHardness(const float _value) {
    m_retriggerHardness = _value;
  }
  inline void setPeakLevel(const uint32_t _voiceId, const float _value) {
    m_levelFactor[_voiceId] = _value;
  }
  inline void reset() {
    for (uint32_t v = 0; v < Size; v++) {
      m_body[v].m_index = 0;
      m_body[v].m_x = 0.0f;
      m_body[v].m_signal_magnitude = 0.0f;
      m_body[v].m_start_magnitude = 0.0f;
    }
  }
};

template <uint32_t Size> struct GateEnvelope {
  const uint32_t m_startIndex = 1, m_stopIndex = 2;
  Proto::EnvBody_Single m_body[Size];
  Proto::SingleSegment<Size> m_segment[Proto::SegSize];
  inline GateEnvelope() {
    for (uint32_t s = 1; s < Proto::SegSize; s++) {
      const uint32_t segment = s - 1;
      m_segment[s].m_state = Proto::SegmentType_Gate[segment][0];
      m_segment[s].m_next = Proto::SegmentType_Gate[segment][1];
      for (uint32_t v = 0; v < Size; v++) {
        setSegmentDx(v, s,
                     static_cast<float>(Proto::SegmentType_Gate[segment][2]));
        setSegmentDest(v, s,
                       static_cast<float>(Proto::SegmentType_Gate[segment][3]));
      }
    }
  }
  inline void init(const float _release) {
    for (uint32_t v = 0; v < Size; v++) {
      setSegmentDx(v, 2, _release);
    }
  }
  inline void start(const uint32_t _voiceId) {
    auto body = &m_body[_voiceId];
    body->m_x = m_segment[m_startIndex].m_dx[_voiceId];
    body->m_y = 1.0f - m_segment[m_startIndex].m_dx[_voiceId];
    body->m_start_magnitude = body->m_signal_magnitude;
    body->m_state = m_segment[m_startIndex].m_state;
    body->m_index = m_startIndex;
    body->m_next = m_segment[m_startIndex].m_next;
    tick(_voiceId);
  }
  inline void stop(const uint32_t _voiceId) {
    auto body = &m_body[_voiceId];
    body->m_x = m_segment[m_stopIndex].m_dx[_voiceId];
    body->m_y = 1.0f - m_segment[m_stopIndex].m_dx[_voiceId];
    body->m_start_magnitude = body->m_signal_magnitude;
    body->m_state = m_segment[m_stopIndex].m_state;
    body->m_index = m_stopIndex;
    body->m_next = m_segment[m_stopIndex].m_next;
  }
  inline void tick(const uint32_t _voiceId) {
    auto body = &m_body[_voiceId];
    const uint32_t segment = body->m_index;
    const float diff_magnitude =
        m_segment[segment].m_dest_magnitude[_voiceId] - body->m_start_magnitude;
    switch (body->m_state) {
    case 0:
      break;
    case 1:
      if (body->m_x < 1.0f) {
        body->m_signal_magnitude =
            body->m_start_magnitude + (diff_magnitude * body->m_x);
        body->m_x += m_segment[segment].m_dx[_voiceId];
      } else {
        body->m_signal_magnitude =
            m_segment[segment].m_dest_magnitude[_voiceId];
        nextSegment(_voiceId);
      }
      break;
    case 2:
      if (body->m_y > Proto::renderMin) {
        body->m_signal_magnitude =
            body->m_start_magnitude + (diff_magnitude * (1.0f - body->m_y));
        body->m_y *= 1.0f - m_segment[segment].m_dx[_voiceId];
      } else {
        body->m_signal_magnitude =
            m_segment[segment].m_dest_magnitude[_voiceId];
        nextSegment(_voiceId);
      }
      break;
    }
  }
  inline void nextSegment(const uint32_t _voiceId) {
    auto body = &m_body[_voiceId];
    body->m_x = m_segment[body->m_next].m_dx[_voiceId];
    body->m_y = 1.0f - m_segment[body->m_next].m_dx[_voiceId];
    body->m_start_magnitude = body->m_signal_magnitude;
    body->m_state = m_segment[body->m_next].m_state;
    body->m_index = body->m_next;
    body->m_next = m_segment[body->m_index].m_next;
  }
  inline void setSegmentDx(const uint32_t _voiceId, const uint32_t _segmentId,
                           const float _value) {
    m_segment[_segmentId].m_dx[_voiceId] = _value;
  }
  inline void setSegmentDest(const uint32_t _voiceId, const uint32_t _segmentId,
                             const float _value) {
    m_segment[_segmentId].m_dest_magnitude[_voiceId] = _value;
  }
  inline void reset() {
    for (uint32_t v = 0; v < Size; v++) {
      m_body[v].m_index = 0;
      m_body[v].m_x = 0.0f;
      m_body[v].m_signal_magnitude = 0.0f;
      m_body[v].m_start_magnitude = 0.0f;
    }
  }
};

template <uint32_t Size> struct DecayEnvelope {
  const uint32_t m_startIndex = 1;
  Proto::EnvBody_Single m_body[Size];
  Proto::SingleSegment<Size> m_segment[Proto::SegSize];
  inline DecayEnvelope() {
    for (uint32_t s = 1; s < Proto::SegSize; s++) {
      const uint32_t segment = s - 1;
      m_segment[s].m_state = Proto::SegmentType_Decay[segment][0];
      m_segment[s].m_next = Proto::SegmentType_Decay[segment][1];
      for (uint32_t v = 0; v < Size; v++) {
        setSegmentDx(v, s,
                     static_cast<float>(Proto::SegmentType_Decay[segment][2]));
        setSegmentDest(
            v, s, static_cast<float>(Proto::SegmentType_Decay[segment][3]));
      }
    }
  }
  inline void start(const uint32_t _voiceId) {
    auto body = &m_body[_voiceId];
    body->m_x = m_segment[m_startIndex].m_dx[_voiceId];
    body->m_y = 1.0f - m_segment[m_startIndex].m_dx[_voiceId];
    body->m_start_magnitude = body->m_signal_magnitude;
    body->m_state = m_segment[m_startIndex].m_state;
    body->m_index = m_startIndex;
    body->m_next = m_segment[m_startIndex].m_next;
  }
  inline void tick(const uint32_t _voiceId) {
    auto body = &m_body[_voiceId];
    const uint32_t segment = body->m_index;
    const float diff_magnitude =
        m_segment[segment].m_dest_magnitude[_voiceId] - body->m_start_magnitude;
    switch (body->m_state) {
    case 0:
      break;
    case 1:
      if (body->m_x < 1.0f) {
        body->m_signal_magnitude =
            body->m_start_magnitude + (diff_magnitude * body->m_x);
        body->m_x += m_segment[segment].m_dx[_voiceId];
      } else {
        body->m_signal_magnitude =
            m_segment[segment].m_dest_magnitude[_voiceId];
        nextSegment(_voiceId);
      }
      break;
    case 2: /* exponential, quasi-finite rendering (release phase) */
      if (body->m_y > Proto::renderMin) {
        body->m_signal_magnitude =
            body->m_start_magnitude + (diff_magnitude * (1.0f - body->m_y));
        body->m_y *= 1.0f - m_segment[segment].m_dx[_voiceId];
      } else {
        body->m_signal_magnitude =
            m_segment[segment].m_dest_magnitude[_voiceId];
        nextSegment(_voiceId);
      }
      break;
    }
  }
  inline void nextSegment(const uint32_t _voiceId) {
    auto body = &m_body[_voiceId];
    body->m_x = m_segment[body->m_next].m_dx[_voiceId];
    body->m_y = 1.0f - m_segment[body->m_next].m_dx[_voiceId];
    body->m_start_magnitude = body->m_signal_magnitude;
    body->m_state = m_segment[body->m_next].m_state;
    body->m_index = body->m_next;
    body->m_next = m_segment[body->m_index].m_next;
  }
  inline void setSegmentDx(const uint32_t _voiceId, const uint32_t _segmentId,
                           const float _value) {
    m_segment[_segmentId].m_dx[_voiceId] = _value;
  }
  inline void setSegmentDest(const uint32_t _voiceId, const uint32_t _segmentId,
                             const float _value) {
    m_segment[_segmentId].m_dest_magnitude[_voiceId] = _value;
  }
  inline void reset() {
    for (uint32_t v = 0; v < Size; v++) {
      m_body[v].m_index = 0;
      m_body[v].m_x = 0.0f;
      m_body[v].m_signal_magnitude = 0.0f;
      m_body[v].m_start_magnitude = 0.0f;
    }
  }
};
} // namespace LegacyEnvelopes
} // namespace Engine
