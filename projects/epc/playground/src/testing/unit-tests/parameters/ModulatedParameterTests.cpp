#include <testing/TestHelper.h>
#include <catch.hpp>
#include "parameters/RibbonParameter.h"
#include "parameters/PitchbendParameter.h"
#include "parameters/ModulationRoutingParameter.h"
#include "parameters/MacroControlParameter.h"
#include "parameters/ModulateableParameter.h"
#include "presets/EditBuffer.h"
#include "presets/EditBufferSnapshotMaker.h"
#include "groups/MacroControlMappingGroup.h"

#include "use-cases/PhysicalControlParameterUseCases.h"

#include <vector>
#include <variant>
#include <functional>
#include <iostream>

namespace ModulationTest
{
  ENUM(TestTarget, uint8_t, HWPosition, HWAmount, MacroPosition, ControlPosition, ModAmount, ModRangeLow, ModRangeHi);
  ENUM(Direction, uint8_t, Inc, Dec);
  ENUM(Resolution, uint8_t, Coarse, Fine);

  struct Setup
  {
    int hwParameterNumber;
    tControlPositionValue initialHwPosition;
    tControlPositionValue initialHWAmount;
    tControlPositionValue initialMacroControlPosition;

    int targetParameterNumber;
    tControlPositionValue initialTargetControlPosition;
    tControlPositionValue initialModAmount;
  };

  template <TestTarget T> struct StepTask
  {
    uint numSteps;
    Direction direction;
    Resolution resolution;
  };

  template <TestTarget T> struct SetTask
  {
    tControlPositionValue v;
  };

  template <TestTarget T, typename V> struct Expect
  {
    V value;
  };

  using StepHWPosition = StepTask<TestTarget::HWPosition>;
  using SetHWPosition = SetTask<TestTarget::HWPosition>;
  using StepHWAmount = StepTask<TestTarget::HWAmount>;
  using SetHWAmount = SetTask<TestTarget::HWAmount>;
  using StepMacroPosition = StepTask<TestTarget::MacroPosition>;
  using SetMacroPosition = SetTask<TestTarget::MacroPosition>;
  using StepTargetValue = StepTask<TestTarget::ControlPosition>;
  using SetTargetValue = SetTask<TestTarget::ControlPosition>;
  using StepModAmount = StepTask<TestTarget::ModAmount>;
  using SetModAmount = SetTask<TestTarget::ModAmount>;
  using StepModRangeLow = StepTask<TestTarget::ModRangeLow>;
  using StepModRangeHi = StepTask<TestTarget::ModRangeHi>;

  using ExpectHWPosition = Expect<TestTarget::HWPosition, tControlPositionValue>;
  using ExpectHWAmount = Expect<TestTarget::HWAmount, tControlPositionValue>;
  using ExpectMacroPosition = Expect<TestTarget::MacroPosition, tControlPositionValue>;
  using ExpectControlPosition = Expect<TestTarget::ControlPosition, tControlPositionValue>;
  using ExpectModAmount = Expect<TestTarget::ModAmount, tControlPositionValue>;
  using ExpectModRangeLow = Expect<TestTarget::ModRangeLow, tControlPositionValue>;
  using ExpectModRangeHi = Expect<TestTarget::ModRangeHi, tControlPositionValue>;
  using ExpectHWPositionString = Expect<TestTarget::HWPosition, std::string>;
  using ExpectModAmountString = Expect<TestTarget::ModAmount, std::string>;
  using ExpectMacroPositionString = Expect<TestTarget::MacroPosition, std::string>;
  using ExpectControlPositionString = Expect<TestTarget::ControlPosition, std::string>;

  using Task
      = std::variant<StepHWPosition, SetHWPosition, StepHWAmount, SetHWAmount, StepMacroPosition, SetMacroPosition,
                     StepTargetValue, SetTargetValue, StepModAmount, SetModAmount, StepModRangeLow, StepModRangeHi>;
  using Tasks = std::vector<Task>;

  using Expectation = std::variant<ExpectHWPosition, ExpectHWAmount, ExpectMacroPosition, ExpectControlPosition,
                                   ExpectModAmount, ExpectModRangeLow, ExpectModRangeHi, ExpectHWPositionString,
                                   ExpectModAmountString, ExpectMacroPositionString, ExpectControlPositionString>;
  using Expectations = std::vector<Expectation>;

  using TasksAndExpectations = std::tuple<Tasks, Expectations>;

  void doTask(const StepHWPosition& t, EditBuffer* eb, PhysicalControlParameter* phys,
              ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    PhysicalControlParameterUseCases uc(phys);
    UNSCOPED_INFO("StepHWPosition " << toString(t.direction) << " " << toString(t.resolution));
    for(auto i = 0; i < t.numSteps; i++)
      uc.incDec(t.direction == Direction::Inc ? 1 : -1, t.resolution == Resolution::Fine, false);
  };

  void doTask(const SetHWPosition& t, EditBuffer* eb, PhysicalControlParameter* phys,
              ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    PhysicalControlParameterUseCases uc(phys);
    UNSCOPED_INFO("SetHWPosition " << t.v);
    uc.setControlPosition(t.v);
  };

  void doTask(const StepHWAmount& t, EditBuffer* eb, PhysicalControlParameter* phys, ModulationRoutingParameter* router,
              MacroControlParameter* macro, ModulateableParameter* target)
  {
    ParameterUseCases uc(router);
    UNSCOPED_INFO("StepHWAmount " << toString(t.direction) << " " << toString(t.resolution));
    for(auto i = 0; i < t.numSteps; i++)
      uc.incDec(t.direction == Direction::Inc ? 1 : -1, t.resolution == Resolution::Fine, false);
  };

  void doTask(const SetHWAmount& t, EditBuffer* eb, PhysicalControlParameter* phys, ModulationRoutingParameter* router,
              MacroControlParameter* macro, ModulateableParameter* target)
  {
    ParameterUseCases uc(router);
    UNSCOPED_INFO("SetHWAmount " << t.v);
    uc.setControlPosition(t.v);
  };

  void doTask(const StepMacroPosition& t, EditBuffer* eb, PhysicalControlParameter* phys,
              ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    ParameterUseCases uc(macro);
    UNSCOPED_INFO("StepMacroPosition " << toString(t.direction) << " " << toString(t.resolution));
    for(auto i = 0; i < t.numSteps; i++)
      uc.incDec(t.direction == Direction::Inc ? 1 : -1, t.resolution == Resolution::Fine, false);
  };

  void doTask(const SetMacroPosition& t, EditBuffer* eb, PhysicalControlParameter* phys,
              ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    ParameterUseCases uc(macro);
    UNSCOPED_INFO("SetMacroPosition " << t.v);
    uc.setControlPosition(t.v);
  };

  void doTask(const StepTargetValue& t, EditBuffer* eb, PhysicalControlParameter* phys,
              ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    ParameterUseCases uc(target);
    UNSCOPED_INFO("StepTargetValue " << toString(t.direction) << " " << toString(t.resolution));
    for(auto i = 0; i < t.numSteps; i++)
      uc.incDec(t.direction == Direction::Inc ? 1 : -1, t.resolution == Resolution::Fine, false);
  };

  void doTask(const SetTargetValue& t, EditBuffer* eb, PhysicalControlParameter* phys,
              ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    ParameterUseCases uc(target);
    UNSCOPED_INFO("SetTargetValue " << t.v);
    uc.setControlPosition(t.v);
  };

  void doTask(const StepModAmount& t, EditBuffer* eb, PhysicalControlParameter* phys,
              ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    ModParameterUseCases uc(target);
    UNSCOPED_INFO("StepModAmount " << toString(t.direction) << " " << toString(t.resolution));
    for(auto i = 0; i < t.numSteps; i++)
      uc.incModAmount(t.direction == Direction::Inc ? 1 : -1, t.resolution == Resolution::Fine);
  };

  void doTask(const SetModAmount& t, EditBuffer* eb, PhysicalControlParameter* phys, ModulationRoutingParameter* router,
              MacroControlParameter* macro, ModulateableParameter* target)
  {
    ModParameterUseCases uc(target);
    UNSCOPED_INFO("SetModAmount " << t.v);
    uc.setModulationAmount(t.v);
  };

  void doTask(const StepModRangeLow& t, EditBuffer* eb, PhysicalControlParameter* phys,
              ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    ModParameterUseCases uc(target);
    UNSCOPED_INFO("StepModRangeLow " << toString(t.direction) << " " << toString(t.resolution));
    for(auto i = 0; i < t.numSteps; i++)
      uc.incLowerModulationBound(t.direction == Direction::Inc ? 1 : -1, t.resolution == Resolution::Fine);
  };

  void doTask(const StepModRangeHi& t, EditBuffer* eb, PhysicalControlParameter* phys,
              ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    ModParameterUseCases uc(target);
    UNSCOPED_INFO("StepModRangeHi " << toString(t.direction) << " " << toString(t.resolution));
    for(auto i = 0; i < t.numSteps; i++)
      uc.incUpperModulationBound(t.direction == Direction::Inc ? 1 : -1, t.resolution == Resolution::Fine);
  };

  using GenericTask
      = std::function<void(EditBuffer* eb, PhysicalControlParameter* phys, ModulationRoutingParameter* router,
                           MacroControlParameter* macro, ModulateableParameter* target)>;

  void doTask(const GenericTask& t, EditBuffer* eb, PhysicalControlParameter* phys, ModulationRoutingParameter* router,
              MacroControlParameter* macro, ModulateableParameter* target)
  {
    t(eb, phys, router, macro, target);
  };

  void checkExpectation(const ExpectHWPosition& t, EditBuffer* eb, PhysicalControlParameter* phys,
                        ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    INFO("HWPosition");
    CHECK(t.value == Approx(phys->getControlPositionValue()));
  };

  void checkExpectation(const ExpectHWAmount& t, EditBuffer* eb, PhysicalControlParameter* phys,
                        ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    INFO("HWAmount");
    CHECK(t.value == Approx(router->getControlPositionValue()));
  };

  void checkExpectation(const ExpectMacroPosition& t, EditBuffer* eb, PhysicalControlParameter* phys,
                        ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    INFO("MacroPosition");
    CHECK(t.value == Approx(macro->getControlPositionValue()));
  };

  void checkExpectation(const ExpectControlPosition& t, EditBuffer* eb, PhysicalControlParameter* phys,
                        ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    INFO("ControlPosition");
    CHECK(t.value == Approx(target->getControlPositionValue()));
  };

  void checkExpectation(const ExpectModAmount& t, EditBuffer* eb, PhysicalControlParameter* phys,
                        ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    INFO("ModAmount");
    CHECK(t.value == Approx(target->getModulationAmount()));
  };

  void checkExpectation(const ExpectModRangeLow& t, EditBuffer* eb, PhysicalControlParameter* phys,
                        ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    INFO("ModRangeLow");
    CHECK(t.value == Approx(target->getModulationRange(true).first));
  };

  void checkExpectation(const ExpectModRangeHi& t, EditBuffer* eb, PhysicalControlParameter* phys,
                        ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    INFO("ModRangeHi");
    CHECK(t.value == Approx(target->getModulationRange(true).second));
  };

  void checkExpectation(const ExpectHWPositionString& t, EditBuffer* eb, PhysicalControlParameter* phys,
                        ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    INFO("HWPositionString");
    CHECK(t.value == phys->getDisplayString());
  };

  void checkExpectation(const ExpectModAmountString& t, EditBuffer* eb, PhysicalControlParameter* phys,
                        ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    INFO("ModAmountString");
    CHECK(t.value == target->stringizeModulationAmount(target->getModulationAmount()));
  };

  void checkExpectation(const ExpectMacroPositionString& t, EditBuffer* eb, PhysicalControlParameter* phys,
                        ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    INFO("MacroPositionString");
    CHECK(t.value == macro->getDisplayString());
  };

  void checkExpectation(const ExpectControlPositionString& t, EditBuffer* eb, PhysicalControlParameter* phys,
                        ModulationRoutingParameter* router, MacroControlParameter* macro, ModulateableParameter* target)
  {
    INFO("ControlPositionString");
    CHECK(t.value == target->getDisplayString());
  }

  template <typename S> void writeStepParams(std::stringstream& str, const char* className, const S& t)
  {
    str << "\n\t\t\tnew " << className << "(" << t.numSteps << ", Direction." << toString(t.direction)
        << ", Resolution." << toString(t.resolution) << ")";
  }

  template <typename S> void writeSetParams(std::stringstream& str, const char* className, const S& t)
  {
    str << "\n\t\t\tnew " << className << "(" << t.v << ")";
  }

  template <typename S> void writeExpectDouble(std::stringstream& str, const char* className, const S& t)
  {
    str << "\n\t\t\tnew " << className << "(" << t.value << ")";
  }

  template <typename S> void writeExpectString(std::stringstream& str, const char* className, const S& t)
  {
    str << "\n\t\t\tnew " << className << "(\"" << t.value << "\")";
  }

  void writeJava(std::stringstream& str, const StepHWPosition& t)
  {
    writeStepParams(str, "StepHWPosition", t);
  }

  void writeJava(std::stringstream& str, const StepHWAmount& t)
  {
    writeStepParams(str, "StepHWAmount", t);
  }

  void writeJava(std::stringstream& str, const StepMacroPosition& t)
  {
    writeStepParams(str, "StepMacroPosition", t);
  }

  void writeJava(std::stringstream& str, const StepTargetValue& t)
  {
    writeStepParams(str, "StepTargetValue", t);
  }

  void writeJava(std::stringstream& str, const StepModAmount& t)
  {
    writeStepParams(str, "StepModAmount", t);
  }

  void writeJava(std::stringstream& str, const StepModRangeLow& t)
  {
    writeStepParams(str, "StepModRangeLow", t);
  }

  void writeJava(std::stringstream& str, const StepModRangeHi& t)
  {
    writeStepParams(str, "StepModRangeHi", t);
  }

  void writeJava(std::stringstream& str, const SetHWPosition& t)
  {
    writeSetParams(str, "SetHWPosition", t);
  }

  void writeJava(std::stringstream& str, const SetHWAmount& t)
  {
    writeSetParams(str, "SetHWAmount", t);
  }

  void writeJava(std::stringstream& str, const SetMacroPosition& t)
  {
    writeSetParams(str, "SetMacroPosition", t);
  }

  void writeJava(std::stringstream& str, const SetTargetValue& t)
  {
    writeSetParams(str, "SetTargetValue", t);
  }

  void writeJava(std::stringstream& str, const SetModAmount& t)
  {
    writeSetParams(str, "SetModAmount", t);
  }

  void writeJava(std::stringstream& str, const ExpectHWPosition& t)
  {
    writeExpectDouble(str, "ExpectHWPosition", t);
  }

  void writeJava(std::stringstream& str, const ExpectHWAmount& t)
  {
    writeExpectDouble(str, "ExpectHWAmount", t);
  }

  void writeJava(std::stringstream& str, const ExpectMacroPosition& t)
  {
    writeExpectDouble(str, "ExpectMacroPosition", t);
  }

  void writeJava(std::stringstream& str, const ExpectControlPosition& t)
  {
    writeExpectDouble(str, "ExpectControlPosition", t);
  }

  void writeJava(std::stringstream& str, const ExpectModAmount& t)
  {
    writeExpectDouble(str, "ExpectModAmount", t);
  }

  void writeJava(std::stringstream& str, const ExpectModRangeLow& t)
  {
    writeExpectDouble(str, "ExpectModRangeLow", t);
  }

  void writeJava(std::stringstream& str, const ExpectModRangeHi& t)
  {
    writeExpectDouble(str, "ExpectModRangeHi", t);
  }

  void writeJava(std::stringstream& str, const ExpectHWPositionString& t)
  {
    writeExpectString(str, "ExpectHWPositionString", t);
  }

  void writeJava(std::stringstream& str, const ExpectModAmountString& t)
  {
    writeExpectString(str, "ExpectModAmountString", t);
  }

  void writeJava(std::stringstream& str, const ExpectMacroPositionString& t)
  {
    writeExpectString(str, "ExpectMacroPositionString", t);
  }

  void writeJava(std::stringstream& str, const ExpectControlPositionString& t)
  {
    writeExpectString(str, "ExpectControlPositionString", t);
  }

  static void doTasksAndCheckExpectations(int block, const TasksAndExpectations& te, EditBuffer* eb,
                                          PhysicalControlParameter* phys, ModulationRoutingParameter* router,
                                          MacroControlParameter* macro, ModulateableParameter* target)
  {
    INFO("TasksAndExpectations block " << block);

    for(const auto& t : std::get<0>(te))
      std::visit([&](auto&& t) { doTask(t, eb, phys, router, macro, target); }, t);

    for(const auto& t : std::get<1>(te))
      std::visit([&](auto&& t) { checkExpectation(t, eb, phys, router, macro, target); }, t);
  }

  static void writeJavaTasksAndExceptions(std::stringstream& str, const TasksAndExpectations& te)
  {
    str << ",\n\t"
        << "new TasksAndExpectations(";
    str << "\n\t\t"
        << "new Tasks(";
    int count = 0;
    for(const auto& t : std::get<0>(te))
      std::visit(
          [&](auto&& t) {
            if(count++)
              str << ", ";
            writeJava(str, t);
          },
          t);

    str << "), \n\t\tnew Expectations(";

    count = 0;
    for(const auto& t : std::get<1>(te))
      std::visit(
          [&](auto&& t) {
            if(count++)
              str << ", ";
            writeJava(str, t);
          },
          t);

    str << "))";
  }

  template <bool writeJava, typename... TasksAndExpectations>
  static void testModulation(int line, const char* desc, const Setup& setup, const TasksAndExpectations&... t)
  {
    using Router = ModulationRoutingParameter;

    INFO("Setting up test from line " << line << ": " << desc);

    TestHelper::initSingleEditBuffer();
    auto eb = TestHelper::getEditBuffer();

    auto g = static_cast<MacroControlMappingGroup*>(eb->getParameterGroupByID({ "MCM", VoiceGroup::Global }));
    auto phys = eb->findAndCastParameterByID<PhysicalControlParameter>({ setup.hwParameterNumber, VoiceGroup::Global });
    auto mc = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });
    auto router = g->getModulationRoutingParameterFor(phys, mc);
    auto target = eb->findAndCastParameterByID<ModulateableParameter>({ setup.targetParameterNumber, VoiceGroup::I });

    phys->getValue().setRawValue(Initiator::EXPLICIT_LOAD, setup.initialHwPosition);
    router->getValue().setRawValue(Initiator::EXPLICIT_LOAD, setup.initialHWAmount);
    mc->getValue().setRawValue(Initiator::EXPLICIT_LOAD, setup.initialMacroControlPosition);
    target->getValue().setRawValue(Initiator::EXPLICIT_LOAD, setup.initialTargetControlPosition);

    ModParameterUseCases uc(target);
    uc.selectModSource(MacroControls::MC1);
    uc.setModulationAmount(setup.initialModAmount);

    int block = 1;

    (doTasksAndCheckExpectations(block++, t, eb, phys, router, mc, target), ...);

    if constexpr(writeJava)
    {
      std::stringstream str;
      str << "\n\njavaTestModulation("
          << "\"" << desc << "\"," << std::endl
          << '\t' << "new Setup(" << setup.hwParameterNumber << "," << setup.initialHwPosition << ","
          << setup.initialHWAmount << "," << setup.initialMacroControlPosition << "," << setup.targetParameterNumber
          << "," << setup.initialTargetControlPosition << "," << setup.initialModAmount << ")";

      (writeJavaTasksAndExceptions(str, t), ...);
      str << ");" << std::endl;

      std::cout << str.str();
    }
  }
};

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Parameter Modulation Batch")
{
  using namespace ModulationTest;

  constexpr bool writeJava = false;

  SECTION("Unipolar")
  {
    testModulation<writeJava>(
        __LINE__, "step mod amount - unipolar",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1.0,
                .initialMacroControlPosition = 0,
                .targetParameterNumber = C15::PID::Osc_A_Fluct,
                .initialTargetControlPosition = 0,
                .initialModAmount = 0.5 },

        TasksAndExpectations { { StepModAmount { 1, Direction::Inc, Resolution::Coarse } },
                               { ExpectModAmount { 0.51 }, ExpectModAmountString { "51.0 %" } } },
        TasksAndExpectations { { StepModAmount { 1, Direction::Inc, Resolution::Fine } },
                               { ExpectModAmount { 0.511 }, ExpectModAmountString { "51.1 %" } } });

    testModulation<writeJava>(__LINE__, "nothing special",
                              Setup { .hwParameterNumber = C15::PID::Bender,
                                      .initialHwPosition = 0,
                                      .initialHWAmount = 1.0,
                                      .initialMacroControlPosition = 0,
                                      .targetParameterNumber = C15::PID::Osc_A_Fluct,
                                      .initialTargetControlPosition = 0,
                                      .initialModAmount = 1.0 },

                              TasksAndExpectations { { StepHWPosition { 1, Direction::Inc, Resolution::Coarse } },
                                                     { ExpectHWPosition { 0.01 }, ExpectControlPosition { 0.01 },
                                                       ExpectModRangeLow { 0.0 }, ExpectModRangeHi { 1.0 } } },

                              TasksAndExpectations { { StepHWPosition { 1, Direction::Dec, Resolution::Coarse } },
                                                     { ExpectHWPosition { 0.00 }, ExpectControlPosition { 0.00 },
                                                       ExpectModRangeLow { 0.0 }, ExpectModRangeHi { 1.0 } } });

    testModulation<writeJava>(
        __LINE__, "nothing special",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1.0,
                .initialMacroControlPosition = 0,
                .targetParameterNumber = C15::PID::Osc_A_Fluct,
                .initialTargetControlPosition = 0,
                .initialModAmount = 1.0 },

        TasksAndExpectations { Tasks { StepHWPosition { 1, Direction::Inc, Resolution::Coarse } },
                               Expectations { ExpectHWPosition { 0.01 }, ExpectControlPosition { 0.01 },
                                              ExpectModRangeLow { 0 }, ExpectModRangeHi { 1.0 } } },

        TasksAndExpectations { Tasks { StepHWPosition { 1, Direction::Dec, Resolution::Coarse } },
                               Expectations { ExpectHWPosition { 0.00 }, ExpectControlPosition { 0.00 },
                                              ExpectModRangeLow { 0 }, ExpectModRangeHi { 1.0 } } });

    testModulation<writeJava>(
        __LINE__, "happy path with minimal effective positive coarse modulation",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1.0,
                .initialMacroControlPosition = 0.5,
                .targetParameterNumber = C15::PID::Osc_A_Fluct,
                .initialTargetControlPosition = 0.5,
                .initialModAmount = 0.01 },

        TasksAndExpectations { Tasks {},

                               Expectations { ExpectHWPositionString { "0.0 %" }, ExpectModAmountString { "1.0 %" },
                                              ExpectMacroPositionString { "50.0 %" },
                                              ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations { Tasks { SetHWPosition { -1 } },

                               Expectations { ExpectHWPositionString { "! -100.0 %" },
                                              ExpectMacroPositionString { "0.0 %" },
                                              ExpectControlPositionString { "49.5 %" } } },

        TasksAndExpectations { Tasks { SetHWPosition { 1 } },

                               Expectations { ExpectHWPositionString { "! 100.0 %" },
                                              ExpectMacroPositionString { "100.0 %" },
                                              ExpectControlPositionString { "50.5 %" } } });

    testModulation<writeJava>(
        __LINE__, "happy path with minimal effective negative coarse modulation",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1.0,
                .initialMacroControlPosition = 0.5,
                .targetParameterNumber = C15::PID::Osc_A_Fluct,
                .initialTargetControlPosition = 0.5,
                .initialModAmount = -0.01 },

        TasksAndExpectations { Tasks {},

                               Expectations { ExpectHWPositionString { "0.0 %" }, ExpectModAmountString { "-1.0 %" },
                                              ExpectMacroPositionString { "50.0 %" },
                                              ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations { Tasks { SetHWPosition { -1 } },

                               Expectations { ExpectHWPositionString { "! -100.0 %" },
                                              ExpectMacroPositionString { "0.0 %" },
                                              ExpectControlPositionString { "50.5 %" } } },

        TasksAndExpectations { Tasks { SetHWPosition { 1 } },

                               Expectations { ExpectHWPositionString { "! 100.0 %" },
                                              ExpectMacroPositionString { "100.0 %" },
                                              ExpectControlPositionString { "49.5 %" } } });

    testModulation<writeJava>(
        __LINE__, "happy path with minimal effective positive fine modulation",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1.0,
                .initialMacroControlPosition = 0.5,
                .targetParameterNumber = C15::PID::Osc_A_Fluct,
                .initialTargetControlPosition = 0.5,
                .initialModAmount = 0.001 },

        TasksAndExpectations { Tasks {},
                               Expectations { ExpectHWPositionString { "0.0 %" }, ExpectModAmountString { "0.1 %" },
                                              ExpectMacroPositionString { "50.0 %" },
                                              ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations {
            Tasks { SetHWPosition { -1 } },
            Expectations { ExpectMacroPositionString { "0.0 %" }, ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations {
            Tasks { SetHWPosition { 1 } },
            Expectations { ExpectMacroPositionString { "100.0 %" }, ExpectControlPositionString { "50.0 %" } } });

    testModulation<writeJava>(
        __LINE__, "happy path with minimal effective positive fine modulation",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1.0,
                .initialMacroControlPosition = 0.5,
                .targetParameterNumber = C15::PID::Osc_A_Fluct,
                .initialTargetControlPosition = 0.5,
                .initialModAmount = 0.002 },

        TasksAndExpectations { Tasks {},
                               Expectations { ExpectHWPositionString { "0.0 %" }, ExpectModAmountString { "0.2 %" },
                                              ExpectMacroPositionString { "50.0 %" },
                                              ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations {
            Tasks { SetHWPosition { -1 } },
            Expectations { ExpectMacroPositionString { "0.0 %" }, ExpectControlPositionString { "49.9 %" } } },

        TasksAndExpectations {
            Tasks { SetHWPosition { 1 } },
            Expectations { ExpectMacroPositionString { "100.0 %" }, ExpectControlPositionString { "50.1 %" } } });

    testModulation<writeJava>(
        __LINE__, "happy path with minimal effective negative fine modulation",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1.0,
                .initialMacroControlPosition = 0.5,
                .targetParameterNumber = C15::PID::Osc_A_Fluct,
                .initialTargetControlPosition = 0.5,
                .initialModAmount = -0.001 },

        TasksAndExpectations { Tasks {},

                               Expectations { ExpectHWPositionString { "0.0 %" }, ExpectModAmountString { "-0.1 %" },
                                              ExpectMacroPositionString { "50.0 %" },
                                              ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations { Tasks { SetHWPosition { -1 } },

                               Expectations { ExpectHWPositionString { "! -100.0 %" },
                                              ExpectMacroPositionString { "0.0 %" },
                                              ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations { Tasks { SetHWPosition { 1 } },

                               Expectations { ExpectHWPositionString { "! 100.0 %" },
                                              ExpectMacroPositionString { "100.0 %" },
                                              ExpectControlPositionString { "49.9 %" } } });

    testModulation<writeJava>(
        __LINE__, "happy path with all amounts set to minimal possible",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 0.001,
                .initialMacroControlPosition = 0.5,
                .targetParameterNumber = C15::PID::Osc_A_Fluct,
                .initialTargetControlPosition = 0.5,
                .initialModAmount = 0.001 },

        TasksAndExpectations {
            Tasks { StepHWPosition { .numSteps = 100, .direction = Direction::Inc, .resolution = Resolution::Fine } },
            Expectations { ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations {
            Tasks { StepHWPosition { .numSteps = 399, .direction = Direction::Inc, .resolution = Resolution::Fine } },
            Expectations { ExpectMacroPositionString { "50.0 %" }, ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations {
            Tasks { StepHWPosition { .numSteps = 1, .direction = Direction::Inc, .resolution = Resolution::Fine } },
            Expectations { ExpectMacroPositionString { "50.1 %" }, ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations {
            Tasks { StepHWPosition { .numSteps = 1, .direction = Direction::Dec, .resolution = Resolution::Fine } },
            Expectations { ExpectMacroPositionString { "50.0 %" }, ExpectControlPositionString { "50.0 %" } } });

    testModulation<writeJava>(__LINE__, "happy path with 1 to 1 mapping",
                              Setup { .hwParameterNumber = C15::PID::Bender,
                                      .initialHwPosition = 0,
                                      .initialHWAmount = 1,
                                      .initialMacroControlPosition = 0.5,
                                      .targetParameterNumber = C15::PID::Osc_A_Fluct,
                                      .initialTargetControlPosition = 0.0,
                                      .initialModAmount = 1 },

                              TasksAndExpectations { Tasks { SetHWPosition { .v = 1 } },
                                                     Expectations { ExpectControlPositionString { "50.0 %" } } },

                              TasksAndExpectations { Tasks { SetHWPosition { .v = -1 } },
                                                     Expectations { ExpectControlPositionString { "0.0 %" } } });

    testModulation<writeJava>(
        __LINE__, "step lower mod bound",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1,
                .initialMacroControlPosition = 0.5,
                .targetParameterNumber = C15::PID::Osc_A_Fluct,
                .initialTargetControlPosition = 0.5,
                .initialModAmount = 0.5 },

        TasksAndExpectations {
            Tasks {},
            Expectations { ExpectModRangeLow { 0.25 }, ExpectControlPosition { 0.5 }, ExpectModRangeHi { 0.75 } } },

        TasksAndExpectations {
            Tasks { StepModRangeLow { .numSteps = 1, .direction = Direction::Dec, .resolution = Resolution::Fine } },
            Expectations { ExpectModRangeLow { 0.249 }, ExpectControlPosition { 0.5 }, ExpectModRangeHi { 0.75 } } },

        TasksAndExpectations {
            // step down to next coarse step
            Tasks { StepModRangeLow { .numSteps = 1, .direction = Direction::Dec, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { 0.24 }, ExpectControlPosition { 0.495 }, ExpectModRangeHi { 0.75 } } },

        TasksAndExpectations {
            Tasks { StepModRangeLow { .numSteps = 1, .direction = Direction::Dec, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { 0.23 }, ExpectControlPosition { 0.49 }, ExpectModRangeHi { 0.75 } } },

        TasksAndExpectations {
            // step to 0
            Tasks { StepModRangeLow { .numSteps = 23, .direction = Direction::Dec, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { 0 }, ExpectControlPosition { 0.375 }, ExpectModRangeHi { 0.75 } } },

        TasksAndExpectations {
            // step into saturation
            Tasks { StepModRangeLow { .numSteps = 1, .direction = Direction::Dec, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { 0 }, ExpectControlPosition { 0.375 }, ExpectModRangeHi { 0.75 } } },

        TasksAndExpectations {
            // step out of saturation
            Tasks { StepModRangeLow { .numSteps = 25, .direction = Direction::Inc, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { 0.25 }, ExpectControlPosition { 0.5 }, ExpectModRangeHi { 0.75 } } },

        TasksAndExpectations {
            // pass upper boarder
            Tasks { StepModRangeLow { .numSteps = 60, .direction = Direction::Inc, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { 0.85 }, ExpectControlPosition { 0.8 }, ExpectModRangeHi { 0.75 } } });

    testModulation<writeJava>(
        __LINE__, "step upper mod bound",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1,
                .initialMacroControlPosition = 0.5,
                .targetParameterNumber = C15::PID::Osc_A_Fluct,
                .initialTargetControlPosition = 0.5,
                .initialModAmount = 0.5 },

        TasksAndExpectations {
            Tasks {},
            Expectations { ExpectModRangeLow { 0.25 }, ExpectControlPosition { 0.5 }, ExpectModRangeHi { 0.75 } } },

        TasksAndExpectations {
            Tasks { StepModRangeHi { .numSteps = 1, .direction = Direction::Inc, .resolution = Resolution::Fine } },
            Expectations { ExpectModRangeLow { 0.25 }, ExpectControlPosition { 0.5 }, ExpectModRangeHi { 0.751 } } },

        TasksAndExpectations {
            // step up to next coarse step
            Tasks { StepModRangeHi { .numSteps = 1, .direction = Direction::Inc, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { 0.25 }, ExpectControlPosition { 0.505 }, ExpectModRangeHi { 0.76 } } },

        TasksAndExpectations {
            Tasks { StepModRangeHi { .numSteps = 1, .direction = Direction::Inc, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { 0.25 }, ExpectControlPosition { 0.51 }, ExpectModRangeHi { 0.77 } } },

        TasksAndExpectations {
            // step to max
            Tasks { StepModRangeHi { .numSteps = 23, .direction = Direction::Inc, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { 0.25 }, ExpectControlPosition { 0.625 }, ExpectModRangeHi { 1 } } },

        TasksAndExpectations {
            // step into saturation
            Tasks { StepModRangeHi { .numSteps = 1, .direction = Direction::Inc, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { 0.25 }, ExpectControlPosition { 0.625 }, ExpectModRangeHi { 1 } } },

        TasksAndExpectations {
            // step out of saturation
            Tasks { StepModRangeHi { .numSteps = 25, .direction = Direction::Dec, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { 0.25 }, ExpectControlPosition { 0.5 }, ExpectModRangeHi { 0.75 } } },

        TasksAndExpectations {
            // pass lower border
            Tasks { StepModRangeHi { .numSteps = 60, .direction = Direction::Dec, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { 0.25 }, ExpectControlPosition { 0.2 }, ExpectModRangeHi { 0.15 } } });
  }

  SECTION("Bipolar")
  {
    testModulation<writeJava>(
        __LINE__, "step mod amount - bipolar",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1.0,
                .initialMacroControlPosition = 0,
                .targetParameterNumber = C15::PID::FB_Mix_Comb,
                .initialTargetControlPosition = 0,
                .initialModAmount = 0.5 },

        TasksAndExpectations { {}, { ExpectModAmount { 0.5 }, ExpectModAmountString { "100.0 %" } } },

        TasksAndExpectations { { StepModAmount { 1, Direction::Inc, Resolution::Coarse } },
                               { ExpectModAmount { 0.505 }, ExpectModAmountString { "101.0 %" } } },

        TasksAndExpectations { { StepModAmount { 1, Direction::Inc, Resolution::Fine } },
                               { ExpectModAmount { 0.5055 }, ExpectModAmountString { "101.1 %" } } });

    testModulation<writeJava>(__LINE__, "nothing special",
                              Setup { .hwParameterNumber = C15::PID::Bender,
                                      .initialHwPosition = 0,
                                      .initialHWAmount = 1.0,
                                      .initialMacroControlPosition = 0,
                                      .targetParameterNumber = C15::PID::FB_Mix_Comb,
                                      .initialTargetControlPosition = 0,
                                      .initialModAmount = 1.0 },

                              TasksAndExpectations { { StepHWPosition { 1, Direction::Inc, Resolution::Coarse } },
                                                     { ExpectHWPosition { 0.01 }, ExpectMacroPosition { 0.01 },
                                                       ExpectControlPosition { 0.02 }, ExpectModRangeLow { 0.0 },
                                                       ExpectModRangeHi { 1.0 } } },

                              TasksAndExpectations { { StepHWPosition { 1, Direction::Dec, Resolution::Coarse } },
                                                     { ExpectHWPosition { 0.00 }, ExpectMacroPosition { 0.00 },
                                                       ExpectControlPosition { 0.00 }, ExpectModRangeLow { 0.0 },
                                                       ExpectModRangeHi { 1.0 } } });

    testModulation<writeJava>(
        __LINE__, "nothing special",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1.0,
                .initialMacroControlPosition = 0,
                .targetParameterNumber = C15::PID::FB_Mix_Comb,
                .initialTargetControlPosition = 0,
                .initialModAmount = 1.0 },

        TasksAndExpectations { Tasks { StepHWPosition { 1, Direction::Inc, Resolution::Coarse } },
                               Expectations { ExpectHWPosition { 0.01 }, ExpectControlPosition { 0.02 },
                                              ExpectModRangeLow { 0 }, ExpectModRangeHi { 1.0 } } },

        TasksAndExpectations { Tasks { StepHWPosition { 1, Direction::Dec, Resolution::Coarse } },
                               Expectations { ExpectHWPosition { 0.00 }, ExpectControlPosition { 0.00 },
                                              ExpectModRangeLow { 0 }, ExpectModRangeHi { 1.0 } } });

    testModulation<writeJava>(__LINE__, "nothing special",
                              Setup { .hwParameterNumber = C15::PID::Bender,
                                      .initialHwPosition = 0,
                                      .initialHWAmount = 1.0,
                                      .initialMacroControlPosition = 0,
                                      .targetParameterNumber = C15::PID::FB_Mix_Comb,
                                      .initialTargetControlPosition = 0,
                                      .initialModAmount = 0.5 },

                              TasksAndExpectations { { StepHWPosition { 1, Direction::Inc, Resolution::Coarse } },
                                                     { ExpectHWPosition { 0.01 }, ExpectMacroPosition { 0.01 },
                                                       ExpectControlPosition { 0.01 }, ExpectModRangeLow { 0.0 },
                                                       ExpectModRangeHi { 1.0 } } },

                              TasksAndExpectations { { StepHWPosition { 1, Direction::Dec, Resolution::Coarse } },
                                                     { ExpectHWPosition { 0.00 }, ExpectMacroPosition { 0.00 },
                                                       ExpectControlPosition { 0.00 }, ExpectModRangeLow { 0.0 },
                                                       ExpectModRangeHi { 1.0 } } });

    testModulation<writeJava>(
        __LINE__, "happy path with minimal effective positive coarse modulation",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1.0,
                .initialMacroControlPosition = 0.5,
                .targetParameterNumber = C15::PID::FB_Mix_Comb,
                .initialTargetControlPosition = 0.5,
                .initialModAmount = 0.01 },

        TasksAndExpectations { Tasks {},

                               Expectations { ExpectHWPositionString { "0.0 %" }, ExpectModAmountString { "2.0 %" },
                                              ExpectMacroPositionString { "50.0 %" },
                                              ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations { Tasks { SetHWPosition { -1 } },

                               Expectations { ExpectHWPositionString { "! -100.0 %" },
                                              ExpectMacroPositionString { "0.0 %" },
                                              ExpectControlPositionString { "49.0 %" } } },

        TasksAndExpectations { Tasks { SetHWPosition { 1 } },

                               Expectations { ExpectHWPositionString { "! 100.0 %" },
                                              ExpectMacroPositionString { "100.0 %" },
                                              ExpectControlPositionString { "51.0 %" } } });

    testModulation<writeJava>(
        __LINE__, "happy path with minimal effective negative coarse modulation",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1.0,
                .initialMacroControlPosition = 0.5,
                .targetParameterNumber = C15::PID::FB_Mix_Comb,
                .initialTargetControlPosition = 0.5,
                .initialModAmount = -0.01 },

        TasksAndExpectations { Tasks {},

                               Expectations { ExpectHWPositionString { "0.0 %" }, ExpectModAmountString { "-2.0 %" },
                                              ExpectMacroPositionString { "50.0 %" },
                                              ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations { Tasks { SetHWPosition { -1 } },

                               Expectations { ExpectHWPositionString { "! -100.0 %" },
                                              ExpectMacroPositionString { "0.0 %" },
                                              ExpectControlPositionString { "51.0 %" } } },

        TasksAndExpectations { Tasks { SetHWPosition { 1 } },

                               Expectations { ExpectHWPositionString { "! 100.0 %" },
                                              ExpectMacroPositionString { "100.0 %" },
                                              ExpectControlPositionString { "49.0 %" } } });

    testModulation<writeJava>(
        __LINE__, "happy path with minimal effective positive fine modulation",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1.0,
                .initialMacroControlPosition = 0.5,
                .targetParameterNumber = C15::PID::FB_Mix_Comb,
                .initialTargetControlPosition = 0.5,
                .initialModAmount = 0.001 },

        TasksAndExpectations { Tasks {},

                               Expectations { ExpectHWPositionString { "0.0 %" }, ExpectModAmountString { "0.2 %" },
                                              ExpectMacroPositionString { "50.0 %" },
                                              ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations { Tasks { SetHWPosition { -1 } },

                               Expectations { ExpectHWPositionString { "! -100.0 %" },
                                              ExpectMacroPositionString { "0.0 %" },
                                              ExpectControlPositionString { "49.9 %" } } },

        TasksAndExpectations { Tasks { SetHWPosition { 1 } },

                               Expectations { ExpectHWPositionString { "! 100.0 %" },
                                              ExpectMacroPositionString { "100.0 %" },
                                              ExpectControlPositionString { "50.1 %" } } });

    testModulation<writeJava>(
        __LINE__, "happy path with minimal effective negative fine modulation",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1.0,
                .initialMacroControlPosition = 0.5,
                .targetParameterNumber = C15::PID::FB_Mix_Comb,
                .initialTargetControlPosition = 0.5,
                .initialModAmount = -0.001 },

        TasksAndExpectations { Tasks {},

                               Expectations { ExpectHWPositionString { "0.0 %" }, ExpectModAmountString { "-0.2 %" },
                                              ExpectMacroPositionString { "50.0 %" },
                                              ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations { Tasks { SetHWPosition { -1 } },

                               Expectations { ExpectHWPositionString { "! -100.0 %" },
                                              ExpectMacroPositionString { "0.0 %" },
                                              ExpectControlPositionString { "50.1 %" } } },

        TasksAndExpectations { Tasks { SetHWPosition { 1 } },

                               Expectations { ExpectHWPositionString { "! 100.0 %" },
                                              ExpectMacroPositionString { "100.0 %" },
                                              ExpectControlPositionString { "49.9 %" } } });

    testModulation<writeJava>(
        __LINE__, "happy path with all amounts set to minimal possible",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 0.001,
                .initialMacroControlPosition = 0.5,
                .targetParameterNumber = C15::PID::FB_Mix_Comb,
                .initialTargetControlPosition = 0.5,
                .initialModAmount = 0.001 },

        TasksAndExpectations {
            Tasks { StepHWPosition { .numSteps = 100, .direction = Direction::Inc, .resolution = Resolution::Fine } },
            Expectations { ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations {
            Tasks { StepHWPosition { .numSteps = 399, .direction = Direction::Inc, .resolution = Resolution::Fine } },
            Expectations { ExpectMacroPositionString { "50.0 %" }, ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations {
            Tasks { StepHWPosition { .numSteps = 1, .direction = Direction::Inc, .resolution = Resolution::Fine } },
            Expectations { ExpectMacroPositionString { "50.1 %" }, ExpectControlPositionString { "50.0 %" } } },

        TasksAndExpectations {
            Tasks { StepHWPosition { .numSteps = 1, .direction = Direction::Dec, .resolution = Resolution::Fine } },
            Expectations { ExpectMacroPositionString { "50.0 %" }, ExpectControlPositionString { "50.0 %" } } });

    testModulation<writeJava>(__LINE__, "happy path with 1 to 1 mapping",
                              Setup { .hwParameterNumber = C15::PID::Bender,
                                      .initialHwPosition = 0,
                                      .initialHWAmount = 1,
                                      .initialMacroControlPosition = 0.5,
                                      .targetParameterNumber = C15::PID::FB_Mix_Comb,
                                      .initialTargetControlPosition = 0.0,
                                      .initialModAmount = 1 },

                              TasksAndExpectations { Tasks { SetHWPosition { .v = 1 } },
                                                     Expectations { ExpectControlPositionString { "100.0 %" } } },

                              TasksAndExpectations { Tasks { SetHWPosition { .v = -1 } },
                                                     Expectations { ExpectControlPositionString { "-100.0 %" } } });

    testModulation<writeJava>(
        __LINE__, "step lower mod bound on bipolar parameter",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1,
                .initialMacroControlPosition = 0.5,
                .targetParameterNumber = C15::PID::FB_Mix_Comb,
                .initialTargetControlPosition = 0.0,
                .initialModAmount = 0.25 },

        TasksAndExpectations {
            Tasks {},
            Expectations { ExpectModRangeLow { -0.25 }, ExpectControlPosition { 0 }, ExpectModRangeHi { 0.25 } } },

        TasksAndExpectations {
            Tasks { StepModRangeLow { .numSteps = 1, .direction = Direction::Dec, .resolution = Resolution::Fine } },
            Expectations { ExpectModRangeLow { -0.251 }, ExpectControlPosition { -0.001 },
                           ExpectModRangeHi { 0.25 } } },

        TasksAndExpectations {
            // step down to next coarse step
            Tasks { StepModRangeLow { .numSteps = 1, .direction = Direction::Dec, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { -0.26 }, ExpectControlPosition { -0.005 }, ExpectModRangeHi { 0.25 } } },

        TasksAndExpectations {
            Tasks { StepModRangeLow { .numSteps = 1, .direction = Direction::Dec, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { -0.27 }, ExpectControlPosition { -0.01 }, ExpectModRangeHi { 0.25 } } },

        TasksAndExpectations {
            // step to 0
            Tasks { StepModRangeLow { .numSteps = 73, .direction = Direction::Dec, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { -1.0 }, ExpectControlPosition { -0.375 }, ExpectModRangeHi { 0.25 } } },

        TasksAndExpectations {
            // step into saturation
            Tasks { StepModRangeLow { .numSteps = 1, .direction = Direction::Dec, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { -1.0 }, ExpectControlPosition { -0.375 }, ExpectModRangeHi { 0.25 } } },

        TasksAndExpectations {
            // step out of saturation
            Tasks { StepModRangeLow { .numSteps = 75, .direction = Direction::Inc, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { -0.25 }, ExpectControlPosition { 0 }, ExpectModRangeHi { 0.25 } } },

        TasksAndExpectations {
            // pass upper boarder
            Tasks {
                StepModRangeLow { .numSteps = 110, .direction = Direction::Inc, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { 0.85 }, ExpectControlPosition { 0.55 }, ExpectModRangeHi { 0.25 } } });

    testModulation<writeJava>(
        __LINE__, "step upper mod bound on bipolar parameter",
        Setup { .hwParameterNumber = C15::PID::Bender,
                .initialHwPosition = 0,
                .initialHWAmount = 1,
                .initialMacroControlPosition = 0.5,
                .targetParameterNumber = C15::PID::FB_Mix_Comb,
                .initialTargetControlPosition = 0.0,
                .initialModAmount = 0.25 },

        TasksAndExpectations {
            Tasks {},
            Expectations { ExpectModRangeLow { -0.25 }, ExpectControlPosition { 0.0 }, ExpectModRangeHi { 0.25 } } },

        TasksAndExpectations {
            Tasks { StepModRangeHi { .numSteps = 1, .direction = Direction::Inc, .resolution = Resolution::Fine } },
            Expectations { ExpectModRangeLow { -0.25 }, ExpectControlPosition { 0.001 }, ExpectModRangeHi { 0.251 } } },

        TasksAndExpectations {
            // step up to next coarse step
            Tasks { StepModRangeHi { .numSteps = 1, .direction = Direction::Inc, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { -0.25 }, ExpectControlPosition { 0.005 }, ExpectModRangeHi { 0.26 } } },

        TasksAndExpectations {
            Tasks { StepModRangeHi { .numSteps = 1, .direction = Direction::Inc, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { -0.25 }, ExpectControlPosition { 0.01 }, ExpectModRangeHi { 0.27 } } },

        TasksAndExpectations {
            // step to max
            Tasks { StepModRangeHi { .numSteps = 73, .direction = Direction::Inc, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { -0.25 }, ExpectControlPosition { 0.375 }, ExpectModRangeHi { 1 } } },

        TasksAndExpectations {
            // step into saturation
            Tasks { StepModRangeHi { .numSteps = 1, .direction = Direction::Inc, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { -0.25 }, ExpectControlPosition { 0.375 }, ExpectModRangeHi { 1 } } },

        TasksAndExpectations {
            // step out of saturation
            Tasks { StepModRangeHi { .numSteps = 25, .direction = Direction::Dec, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { -0.25 }, ExpectControlPosition { 0.25 }, ExpectModRangeHi { 0.75 } } },

        TasksAndExpectations {
            // pass lower border
            Tasks { StepModRangeHi { .numSteps = 110, .direction = Direction::Dec, .resolution = Resolution::Coarse } },
            Expectations { ExpectModRangeLow { -0.25 }, ExpectControlPosition { -0.3 }, ExpectModRangeHi { -0.35 } } });
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Parameter Modulation")
{
  using Router = ModulationRoutingParameter;

  auto scope = TestHelper::createTestScope();
  auto eb = TestHelper::getEditBuffer();

  WHEN("using ribbon, routed to MCA, modulationg envelope A, Attack")
  {
    auto ribbon = eb->findAndCastParameterByID<RibbonParameter>({ C15::PID::Ribbon_1, VoiceGroup::Global });
    auto router = eb->findAndCastParameterByID<Router>({ C15::PID::Ribbon_1_to_MC_A, VoiceGroup::Global });
    auto mc = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });
    auto target = eb->findAndCastParameterByID<ModulateableParameter>({ C15::PID::Env_A_Att, VoiceGroup::I });

    WHEN("ribbon is set up in return mode")
    {
      ribbon->undoableSetRibbonReturnMode(scope->getTransaction(), RibbonReturnMode::RETURN, Initiator::EXPLICIT_HWUI);

      ribbon->getValue().setRawValue(Initiator::EXPLICIT_LOAD, 0);
      router->getValue().setRawValue(Initiator::EXPLICIT_LOAD, 1);
      mc->getValue().setRawValue(Initiator::EXPLICIT_LOAD, 0.5);
      target->getValue().setRawValue(Initiator::EXPLICIT_LOAD, 0.5);
      target->setModulationSource(scope->getTransaction(), MacroControls::MC1);
      target->setModulationAmount(scope->getTransaction(), 0.5);

      THEN("mod range is correct")
      {
        CHECK(target->getModulationRange(true).first == Approx(0.25));
        CHECK(target->getModulationRange(true).second == Approx(0.75));
      }

      THEN("mod range display values are correct")
      {
        CHECK(target->getModRangeAsDisplayValues().first == "2.00 ms");
        CHECK(target->getModRangeAsDisplayValues().second == "800 ms");
      }

      WHEN("and Ribbon is moved to 25%")
      {
        ribbon->setCPFromHwui(scope->getTransaction(), 0.25);

        THEN("MC moves as expected")
        {
          CHECK(mc->getControlPositionValue() == Approx(0.75));
        }

        THEN("Target moves as expected")
        {
          CHECK(target->getControlPositionValue() == Approx(0.625));
        }

        THEN("Range does not move")
        {
          CHECK(target->getModulationRange(true).first == Approx(0.25));
          CHECK(target->getModulationRange(true).second == Approx(0.75));
        }

        WHEN("and Ribbon is moved from 25% to 100%")
        {
          ribbon->setCPFromHwui(scope->getTransaction(), 1);

          THEN("MC is saturated")
          {
            CHECK(mc->getControlPositionValue() == Approx(1));
          }

          THEN("Target moves only to saturated MC position")
          {
            CHECK(target->getControlPositionValue() == Approx(0.75));
          }

          THEN("Targets modulation range still unchanged")
          {
            CHECK(target->getModulationRange(true).first == Approx(0.25));
            CHECK(target->getModulationRange(true).second == Approx(0.75));
          }

          WHEN("and Ribbon returns to 0 from saturated state")
          {
            ribbon->setCPFromHwui(scope->getTransaction(), 0);

            THEN("MC returns to its original state")
            {
              CHECK(mc->getControlPositionValue() == Approx(0.5));
            }

            THEN("Target returns to its original state")
            {
              CHECK(target->getControlPositionValue() == Approx(0.5));
            }

            THEN("Targets Modulation range still unchanged")
            {
              CHECK(target->getModulationRange(true).first == Approx(0.25));
              CHECK(target->getModulationRange(true).second == Approx(0.75));
            }
          }
        }
      }
    }
  }

  WHEN("using bender, routed to MC A, modulationg osc A, phase modulation feedback (to have many bipolar params)")
  {
    auto mc = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });
    auto target = eb->findAndCastParameterByID<ModulateableParameter>({ C15::PID::Osc_A_PM_FB, VoiceGroup::I });

    mc->getValue().setRawValue(Initiator::EXPLICIT_LOAD, 0.5);
    target->getValue().setRawValue(Initiator::EXPLICIT_LOAD, 0);
    target->setModulationSource(scope->getTransaction(), MacroControls::MC1);
    target->setModulationAmount(scope->getTransaction(), 1);

    THEN("Targets Modulation range is as expected")
    {
      CHECK(target->getModulationRange(true).first == Approx(-1));
      CHECK(target->getModulationRange(true).second == Approx(1));
    }

    THEN("mod range display values are correct")
    {
      CHECK(target->getModRangeAsDisplayValues().first == "-100.0 %");
      CHECK(target->getModRangeAsDisplayValues().second == "100.0 %");
    }
  }

  WHEN("using bender, routed to MC A, modulating osc A, phase modulation feedback (to have many bipolar params)")
  {
    auto bender = eb->findAndCastParameterByID<PitchbendParameter>({ C15::PID::Bender, VoiceGroup::Global });
    auto router = eb->findAndCastParameterByID<Router>({ C15::PID::Bender_to_MC_A, VoiceGroup::Global });
    auto mc = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });
    auto target = eb->findAndCastParameterByID<ModulateableParameter>({ C15::PID::Osc_A_PM_FB, VoiceGroup::I });

    bender->getValue().setRawValue(Initiator::EXPLICIT_LOAD, 0);
    router->getValue().setRawValue(Initiator::EXPLICIT_LOAD, -0.5);
    mc->getValue().setRawValue(Initiator::EXPLICIT_LOAD, 0.75);
    target->getValue().setRawValue(Initiator::EXPLICIT_LOAD, -0.5);
    target->setModulationSource(scope->getTransaction(), MacroControls::MC1);
    target->setModulationAmount(scope->getTransaction(), -0.125);

    THEN("Targets Modulation range is as expected")
    {
      CHECK(target->getModulationRange(true).first == Approx(-0.313));
      CHECK(target->getModulationRange(true).second == Approx(-0.563));
    }

    THEN("mod range display values are correct")
    {
      CHECK(target->getModRangeAsDisplayValues().first == "-31.3 %");
      CHECK(target->getModRangeAsDisplayValues().second == "-56.3 %");
    }
  }

  WHEN("a unipolar modulateable parameter is set up")
  {
    auto fluc = eb->findAndCastParameterByID<ModulateableParameter>({ C15::PID::Osc_A_Fluct, VoiceGroup::I });
    auto mc = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });

    mc->setCPFromHwui(scope->getTransaction(), 0.5);

    fluc->setCPFromHwui(scope->getTransaction(), 0.5);
    fluc->setModulationAmount(scope->getTransaction(), 0.5);
    fluc->setModulationSource(scope->getTransaction(), MacroControls::MC1);

    scope.reset();

    CHECK(fluc->getModulationRange(true).first == Approx(0.25));
    CHECK(fluc->getControlPositionValue() == Approx(0.5));
    CHECK(fluc->getModulationRange(true).second == Approx(0.75));

    WHEN("decrement the lower limit")
    {
      ModParameterUseCases uc(fluc);
      uc.incLowerModulationBound(-1, false);

      CHECK(fluc->getModulationRange(true).first == Approx(0.24));
      CHECK(fluc->getControlPositionValue() == Approx(0.495));
      CHECK(fluc->getModulationRange(true).second == Approx(0.75));

      WHEN("incrementing it again")
      {
        ModParameterUseCases uc(fluc);
        uc.incLowerModulationBound(1, false);

        CHECK(fluc->getModulationRange(true).first == Approx(0.25));
        CHECK(fluc->getControlPositionValue() == Approx(0.5));
        CHECK(fluc->getModulationRange(true).second == Approx(0.75));
      }
    }
  }

  WHEN("a unipolar modulateable parameter is set up at left of its range")
  {
    auto fluc = eb->findAndCastParameterByID<ModulateableParameter>({ C15::PID::Osc_A_Fluct, VoiceGroup::I });
    auto mc = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });

    mc->setCPFromHwui(scope->getTransaction(), 0.5);

    fluc->setCPFromHwui(scope->getTransaction(), 0.0);
    fluc->setModulationAmount(scope->getTransaction(), 0.5);
    fluc->setModulationSource(scope->getTransaction(), MacroControls::MC1);

    scope.reset();

    CHECK(fluc->getModulationRange(true).first == Approx(0.0));
    CHECK(fluc->getControlPositionValue() == Approx(0.0));
    CHECK(fluc->getModulationRange(true).second == Approx(0.25));

    WHEN("decrement the lower limit")
    {
      THEN("range is bound to its limit, value jumps")
      {
        ModParameterUseCases uc(fluc);
        uc.incLowerModulationBound(-1, false);

        CHECK(fluc->getModulationRange(true).first == Approx(0.0));
        CHECK(fluc->getControlPositionValue() == Approx(0.125));
        CHECK(fluc->getModulationRange(true).second == Approx(0.25));
      }
    }
  }

  WHEN("a bipolar modulateable parameter is set up")
  {
    auto target = eb->findAndCastParameterByID<ModulateableParameter>({ C15::PID::Osc_A_PM_FB, VoiceGroup::I });
    auto mc = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });

    mc->setCPFromHwui(scope->getTransaction(), 0.5);

    target->setCPFromHwui(scope->getTransaction(), 0.5);
    target->setModulationAmount(scope->getTransaction(), 0.25);
    target->setModulationSource(scope->getTransaction(), MacroControls::MC1);

    scope.reset();

    CHECK(target->getModulationRange(true).first == Approx(0.25));
    CHECK(target->getControlPositionValue() == Approx(0.5));
    CHECK(target->getModulationRange(true).second == Approx(0.75));

    WHEN("decrement the lower limit")
    {
      ModParameterUseCases uc(target);
      uc.incLowerModulationBound(-1, false);

      CHECK(target->getModulationRange(true).first == Approx(0.24));
      CHECK(target->getControlPositionValue() == Approx(0.495));
      CHECK(target->getModulationRange(true).second == Approx(0.75));

      WHEN("incrementing it again")
      {
        ModParameterUseCases uc(target);
        uc.incLowerModulationBound(1, false);

        CHECK(target->getModulationRange(true).first == Approx(0.25));
        CHECK(target->getControlPositionValue() == Approx(0.5));
        CHECK(target->getModulationRange(true).second == Approx(0.75));
      }
    }

    WHEN("increment the lower limit")
    {
      ModParameterUseCases uc(target);
      uc.incLowerModulationBound(1, false);

      CHECK(target->getModulationRange(true).first == Approx(0.26));
      CHECK(target->getControlPositionValue() == Approx(0.505));
      CHECK(target->getModulationRange(true).second == Approx(0.75));

      WHEN("decrementing it again")
      {
        ModParameterUseCases uc(target);
        uc.incLowerModulationBound(-1, false);

        CHECK(target->getModulationRange(true).first == Approx(0.25));
        CHECK(target->getControlPositionValue() == Approx(0.5));
        CHECK(target->getModulationRange(true).second == Approx(0.75));
      }
    }

    WHEN("decrement the upper limit")
    {
      ModParameterUseCases uc(target);
      uc.incUpperModulationBound(-1, false);

      CHECK(target->getModulationRange(true).first == Approx(0.25));
      CHECK(target->getControlPositionValue() == Approx(0.495));
      CHECK(target->getModulationRange(true).second == Approx(0.74));

      WHEN("incrementing it again")
      {
        ModParameterUseCases uc(target);
        uc.incUpperModulationBound(1, false);

        CHECK(target->getModulationRange(true).first == Approx(0.25));
        CHECK(target->getControlPositionValue() == Approx(0.5));
        CHECK(target->getModulationRange(true).second == Approx(0.75));
      }
    }

    WHEN("increment the upper limit")
    {
      ModParameterUseCases uc(target);
      uc.incUpperModulationBound(1, false);

      CHECK(target->getModulationRange(true).first == Approx(0.25));
      CHECK(target->getControlPositionValue() == Approx(0.505));
      CHECK(target->getModulationRange(true).second == Approx(0.76));

      WHEN("decrementing it again")
      {
        ModParameterUseCases uc(target);
        uc.incUpperModulationBound(-1, false);

        CHECK(target->getModulationRange(true).first == Approx(0.25));
        CHECK(target->getControlPositionValue() == Approx(0.5));
        CHECK(target->getModulationRange(true).second == Approx(0.75));
      }
    }
  }

  WHEN("a bipolar modulateable parameter is set up at left of its range")
  {
    auto target = eb->findAndCastParameterByID<ModulateableParameter>({ C15::PID::Osc_A_PM_FB, VoiceGroup::I });
    auto mc = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });

    mc->setCPFromHwui(scope->getTransaction(), 0.5);

    target->setCPFromHwui(scope->getTransaction(), -1.0);
    target->setModulationAmount(scope->getTransaction(), 0.5);
    target->setModulationSource(scope->getTransaction(), MacroControls::MC1);

    scope.reset();

    CHECK(target->getModulationRange(true).first == Approx(-1.0));
    CHECK(target->getControlPositionValue() == Approx(-1.0));
    CHECK(target->getModulationRange(true).second == Approx(-0.5));

    WHEN("decrement the lower limit")
    {
      THEN("range is bound to its limits, value jumps")
      {
        ModParameterUseCases uc(target);
        uc.incLowerModulationBound(-1, false);

        CHECK(target->getModulationRange(true).first == Approx(-1.0));
        CHECK(target->getControlPositionValue() == Approx(-0.75));
        CHECK(target->getModulationRange(true).second == Approx(-0.5));
      }
    }
  }

  WHEN("Comb Filter APTune parameter is set up modulated")
  {
    auto target = eb->findAndCastParameterByID<ModulateableParameter>({ C15::PID::Comb_Flt_AP_Tune, VoiceGroup::I });
    auto mc = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });

    mc->setCPFromHwui(scope->getTransaction(), 0.5);

    target->setCPFromHwui(scope->getTransaction(), 0.5);
    target->setModulationAmount(scope->getTransaction(), 0.01);
    target->setModulationSource(scope->getTransaction(), MacroControls::MC1);

    scope.reset();

    CHECK(target->getModulationRange(true).first == Approx(0.495));
    CHECK(target->getControlPositionValue() == Approx(0.5));
    CHECK(target->getModulationRange(true).second == Approx(0.505));

    MacroControlParameterUseCases uc(mc);

    WHEN("moving macro control to min")
    {
      THEN("range is bound to its limits, value jumps")
      {
        uc.setControlPosition(0);

        CHECK(target->getModulationRange(true).first == Approx(0.495));
        CHECK(target->getControlPositionValue() == Approx(0.495));
        CHECK(target->getModulationRange(true).second == Approx(0.505));
      }
    }

    WHEN("decrementing the macro control to min")
    {
      THEN("range is sticky")
      {
        for(int i = 0; i < 100; i++)
        {
          uc.incDecPosition(-1, false, false);

          CHECK(target->getModulationRange(true).first == Approx(0.495));
          CHECK(target->getModulationRange(true).second == Approx(0.505));
        }
      }
    }

    WHEN("incrementing the macro control to min")
    {
      THEN("range is sticky")
      {
        for(int i = 0; i < 100; i++)
        {
          uc.incDecPosition(1, false, false);

          CHECK(target->getModulationRange(true).first == Approx(0.495));
          CHECK(target->getModulationRange(true).second == Approx(0.505));
        }
      }
    }

    WHEN("moving macro control to max")
    {
      THEN("range is bound to its limits, value jumps")
      {
        uc.setControlPosition(1);

        CHECK(target->getModulationRange(true).first == Approx(0.495));
        CHECK(target->getControlPositionValue() == Approx(0.505));
        CHECK(target->getModulationRange(true).second == Approx(0.505));
      }
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "undo/redo on modulated parameters")
{
  auto scope = TestHelper::createTestScope();
  auto eb = TestHelper::getEditBuffer();

  GIVEN("a modulated parameter")
  {
    auto target = eb->findAndCastParameterByID<ModulateableParameter>({ C15::PID::Osc_A_PM_FB, VoiceGroup::I });
    auto mc = eb->findAndCastParameterByID<MacroControlParameter>({ C15::PID::MC_A, VoiceGroup::Global });

    mc->setCPFromHwui(scope->getTransaction(), 0.5);

    target->setCPFromHwui(scope->getTransaction(), -1.0);
    target->setModulationAmount(scope->getTransaction(), 0.5);
    target->setModulationSource(scope->getTransaction(), MacroControls::MC1);

    WHEN("the parameter is first set")
    {
      scope.reset();
      scope = TestHelper::createTestScope();
      target->setCPFromHwui(scope->getTransaction(), 0.6);

      AND_THEN("is modulated from play controller")
      {
        scope.reset();
        mc->getValue().changeRawValue(Initiator::EXPLICIT_PLAYCONTROLLER, 0.25);

        CHECK(target->getControlPositionValue() == Approx(0.85));

        AND_THEN("is loaded")
        {
          scope = TestHelper::createTestScope();
          EditBufferSnapshotMaker::get().addSnapshotIfRequired(scope->getTransaction(), eb);
          target->setCPFromLoad(scope->getTransaction(), -0.5, false);

          CHECK(target->getControlPositionValue() == Approx(-0.5));

          THEN("undo the load restores the modulated value")
          {
            scope.reset();
            TestHelper::getPresetManager()->getUndoScope().undo();
            CHECK(target->getControlPositionValue() == Approx(0.85));
          }
        }
      }
    }
  }
}
