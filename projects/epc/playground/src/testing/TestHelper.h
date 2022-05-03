#pragma once
#include <libundo/undo/TransactionCreationScope.h>
#include <memory>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/Parameter.h>
#include <catch.hpp>
#include <libundo/undo/Scope.h>
#include <Application.h>
#include <parameters/ModulateableParameter.h>
#include <nltools/messaging/Message.h>
#include "use-cases/EditBufferUseCases.h"
#include "use-cases/SettingsUseCases.h"
#include "use-cases/PresetManagerUseCases.h"
#include "use-cases/SoundUseCases.h"
#include <glibmm.h>

namespace TestHelper
{
  class ApplicationFixture
  {
   protected:
    Application app;
   public:
    ApplicationFixture() : app(0, nullptr)
    {
    }

    ~ApplicationFixture()
    {
    }
  };

  class MainContextFixture
  {
   protected:
    GMainContext* m_context;
   public:
    MainContextFixture() : m_context(g_main_context_new())
    {
    }

    ~MainContextFixture()
    {
      g_main_context_release(m_context);
      m_context = nullptr;
    }
  };

  struct ScopedMessagingConfiguration
  {
    explicit ScopedMessagingConfiguration(const nltools::msg::Configuration &config)
    {
      m_oldConfig = nltools::msg::getConfig();
      nltools::msg::init(config);
    }

    ~ScopedMessagingConfiguration()
    {
      nltools::msg::init(m_oldConfig);
    }

    nltools::msg::Configuration m_oldConfig;
  };

  inline nltools::msg::Setting::MidiSettingsMessage::tRoutingMappings createFullMappings(bool state)
  {
    typedef nltools::msg::Setting::MidiSettingsMessage::tRoutingMappings tHW;
    tHW ret;
    for(auto& row : ret)
    {
      row.fill(state);
    }
    return ret;
  }

  namespace floating
  {
    template <typename T> inline bool equals(T first, T second)
    {
      constexpr auto epsilon = std::numeric_limits<T>::epsilon();
      return std::abs(first - second) <= epsilon;
    }

    template <typename T> inline bool differs(T first, T second)
    {
      return !equals(first, second);
    }
  }

  inline PresetManager* getPresetManager()
  {
    return Application::get().getPresetManager();
  }

  inline Settings* getSettings()
  {
    return Application::get().getSettings();
  }

  inline AudioEngineProxy& getAudioEngineProxy()
  {
    return *Application::get().getAudioEngineProxy();
  }

  inline EditBuffer* getEditBuffer()
  {
    return getPresetManager()->getEditBuffer();
  }

  inline const Options& getOptions()
  {
    return *Application::get().getOptions();
  }

  inline std::unique_ptr<UNDO::TransactionCreationScope> createTestScope()
  {
    return std::move(getPresetManager()->getUndoScope().startTestTransaction());
  }

  template <SoundType tType> inline void initDualEditBuffer(VoiceGroup currentSelectedVG)
  {
    auto eb = getEditBuffer();
    EditBufferUseCases useCases(*eb);

    useCases.unlockAllGroups();
    useCases.convertToDual(tType, currentSelectedVG);
    useCases.initSound(Defaults::FactoryDefault);
  }

  inline void initSingleEditBuffer()
  {
    auto eb = getEditBuffer();
    EditBufferUseCases useCases(*eb);

    useCases.unlockAllGroups();
    useCases.convertToSingle(VoiceGroup::I);
    useCases.initSound(Defaults::FactoryDefault);
  }

  inline void forceParameterChange(UNDO::Transaction* transaction, Parameter* param)
  {
    auto currentValue = param->getControlPositionValue();

    auto incNext = param->getValue().getNextStepValue(1, {});
    auto decNext = param->getValue().getNextStepValue(-1, {});

    if(floating::differs(incNext, currentValue))
      param->setCPFromHwui(transaction, incNext);
    else if(floating::differs(decNext, currentValue))
      param->setCPFromHwui(transaction, decNext);
    else
      nltools_detailedAssertAlways(false, "Unable to change Parameter Value in either direction");
  }

  inline void forceParameterChange(Parameter* param)
  {
    auto scope = TestHelper::createTestScope();
    forceParameterChange(scope->getTransaction(), param);
  }

  template <typename tCB> inline void forEachParameter(const tCB& cb, EditBuffer* eb)
  {
    for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
      for(auto& g : eb->getParameterGroups(vg))
        for(auto& p : g->getParameters())
          cb(p);
  }

  inline void changeAllParameters(UNDO::Transaction* transaction)
  {
    auto eb = TestHelper::getEditBuffer();
    eb->forEachParameter([&](Parameter* param) { TestHelper::forceParameterChange(transaction, param); });
  };

  void randomizeCrossFBAndToFX(UNDO::Transaction* transaction);

  void randomizeFadeParams(UNDO::Transaction* transaction);

  inline void doMainLoopFor(std::chrono::milliseconds time, GMainContext* ctx = nullptr)
  {
    Expiration exp;
    exp.refresh(time);

    while(exp.isPending())
      g_main_context_iteration(ctx, TRUE);
  }

  inline void doMainLoop(std::chrono::milliseconds minTime, std::chrono::milliseconds timeout,
                         const std::function<bool()>& test, GMainContext* ctx = nullptr)
  {
    if(Application::exists() && ctx == nullptr)
    {
      ctx = Application::get().getMainContext()->gobj();
    }

    Expiration exp;
    exp.refresh(timeout);

    Expiration min;

    if(minTime != std::chrono::milliseconds::zero())
      min.refresh(minTime);

    while((exp.isPending() && !test()) || min.isPending())
      g_main_context_iteration(ctx, TRUE);

    CHECK(test());
  }

  inline void doMainLoopIterationOnContext(Glib::MainContext* c)
  {
    c->iteration(true);
  }

  inline void doMainLoopIteration(GMainContext* ctx = nullptr)
  {
    g_main_context_iteration(ctx, TRUE);
  }

  inline void updateMappings(nltools::msg::Setting::MidiSettingsMessage::tRoutingMappings& array,
                      nltools::msg::Setting::MidiSettingsMessage::RoutingAspect index, bool b)
  {
    for(auto& hw: array) {
      hw[static_cast<int>(index)] = b;
    }
  }

  inline void updateMappingForRow(nltools::msg::Setting::MidiSettingsMessage::tRoutingMappings& array,
                                  nltools::msg::Setting::MidiSettingsMessage::RoutingIndex index,
                                  bool value)
  {
    for(auto& a: array[static_cast<int>(index)])
    {
      a = value;
    }
  }

  inline void updateMappingForHW(nltools::msg::Setting::MidiSettingsMessage::tRoutingMappings& array,
                                 nltools::msg::Setting::MidiSettingsMessage::RoutingIndex index,
                                 nltools::msg::Setting::MidiSettingsMessage::RoutingAspect aspect, bool b)
  {
    array[static_cast<int>(index)][static_cast<int>(aspect)] = b;
  }
}

inline std::pair<double, double> getNextStepValuesFromValue(Parameter* p, double v)
{
  auto scope = UNDO::Scope::startTrashTransaction();
  auto oldCP = p->getControlPositionValue();
  p->setCPFromHwui(scope->getTransaction(), v);
  auto ret
      = std::make_pair<double, double>(p->getValue().getNextStepValue(-1, {}), p->getValue().getNextStepValue(1, {}));
  p->setCPFromHwui(scope->getTransaction(), oldCP);
  return ret;
}

#define CHECK_PARAMETER_CP_EQUALS_FICTION(p, v)                                                                        \
  {                                                                                                                    \
    auto range = getNextStepValuesFromValue(p, v);                                                                     \
    auto inRange = p->getControlPositionValue() >= range.first && p->getControlPositionValue() <= range.second;        \
    if(!inRange)                                                                                                       \
    {                                                                                                                  \
      nltools::Log::error(p->getLongName(), '(', p->getID().toString(), ") got", p->getControlPositionValue(),         \
                          "expected ~", v);                                                                            \
      CHECK(inRange);                                                                                                  \
      return;                                                                                                          \
    }                                                                                                                  \
    CHECK(p->getControlPositionValue() >= range.first);                                                                \
    CHECK(p->getControlPositionValue() <= range.second);                                                               \
  }

#define CHECK_PARAMETER_CP_UNEQUALS_FICTION(p, v)                                                                      \
  {                                                                                                                    \
    auto range = getNextStepValuesFromValue(p, v);                                                                     \
    auto inRange = p->getControlPositionValue() >= range.first && p->getControlPositionValue() <= range.second;        \
    if(inRange)                                                                                                        \
    {                                                                                                                  \
      nltools::Log::error(p->getLongName(), '(', p->getID().toString(), ") got", p->getControlPositionValue(),         \
                          "expected unequals ~", v);                                                                   \
      CHECK(false);                                                                                                    \
      return;                                                                                                          \
    }                                                                                                                  \
    CHECK_FALSE(p->getControlPositionValue() >= range.first);                                                          \
    CHECK_FALSE(p->getControlPositionValue() <= range.second);                                                         \
  }
