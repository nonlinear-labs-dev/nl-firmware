package com.nonlinearlabs.client;

import java.util.NoSuchElementException;

import com.google.gwt.core.client.Callback;
import com.google.gwt.core.client.JavaScriptObject;
import com.google.gwt.core.client.ScriptInjector;
import com.google.gwt.junit.client.GWTTestCase;
import com.google.gwt.user.client.Window;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.MacroControlParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel.ModSource;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulationRouterParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterFactory;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.dataModel.editBuffer.PhysicalControlParameterModel;
import com.nonlinearlabs.client.presenters.ParameterPresenterProviders;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;

/**
 * GWT JUnit tests must extend GWTTestCase.
 * 
 * howto:
 * in nonmaps dir, run
 * mvn test -Dgwt.args="-runStyle Manual:1 -port 1234 -sourceLevel 11 -style
 * PRETTY"
 * 
 * also, run playground (needed for providing parameter setup and NonMaps.js)
 * navigate to:
 * http://localhost:1234/com.nonlinearlabs.NonMapsJUnit.JUnit/junit.html in
 * chrome
 */
public class NonMapsTest extends GWTTestCase {

      /**
       * Must refer to a valid module that sources this class.
       */
      public String getModuleName() {
            return "com.nonlinearlabs.NonMapsJUnit";
      }

      static native void fixWndPtr() /*-{
            window['isCurrentlyTesting'] = 1;
            $wnd['formatDimensionDigits'] = window['formatDimensionDigits'];
            $wnd['formatDimension'] = window['formatDimension'];
       }-*/;

      public void testModulation() {

            ScriptInjector.fromUrl("http://localhost:8080/nonmaps/NonMaps.js").setCallback(
                        new Callback<Void, Exception>() {
                              public void onFailure(Exception reason) {
                                    Window.alert("Script load failed.");
                              }

                              public void onSuccess(Void result) {
                                    fixWndPtr();
                                    doTheTests();
                              }
                        }).inject();
      }

      private void assertRouterMapsToPhysical(int routerId, int physicalId) {
            assertEquals(ParameterFactory.getPhysicalControlForRouter(new ParameterId(routerId, VoiceGroup.Global)),
                        new ParameterId(physicalId, VoiceGroup.Global));
      }

      public void testParameterFactory() {
            assertRouterMapsToPhysical(255, 254);
            assertRouterMapsToPhysical(256, 254);
            assertRouterMapsToPhysical(257, 254);
            assertRouterMapsToPhysical(258, 254);
            assertRouterMapsToPhysical(373, 254);
            assertRouterMapsToPhysical(374, 254);

            assertRouterMapsToPhysical(260, 259);
            assertRouterMapsToPhysical(261, 259);
            assertRouterMapsToPhysical(262, 259);
            assertRouterMapsToPhysical(263, 259);
            assertRouterMapsToPhysical(375, 259);
            assertRouterMapsToPhysical(376, 259);

            assertRouterMapsToPhysical(265, 264);
            assertRouterMapsToPhysical(266, 264);
            assertRouterMapsToPhysical(267, 264);
            assertRouterMapsToPhysical(268, 264);
            assertRouterMapsToPhysical(377, 264);
            assertRouterMapsToPhysical(378, 264);

            assertRouterMapsToPhysical(270, 269);
            assertRouterMapsToPhysical(271, 269);
            assertRouterMapsToPhysical(272, 269);
            assertRouterMapsToPhysical(273, 269);
            assertRouterMapsToPhysical(379, 269);
            assertRouterMapsToPhysical(380, 269);

            assertRouterMapsToPhysical(275, 274);
            assertRouterMapsToPhysical(276, 274);
            assertRouterMapsToPhysical(277, 274);
            assertRouterMapsToPhysical(278, 274);
            assertRouterMapsToPhysical(381, 274);
            assertRouterMapsToPhysical(382, 274);

            assertRouterMapsToPhysical(280, 279);
            assertRouterMapsToPhysical(281, 279);
            assertRouterMapsToPhysical(282, 279);
            assertRouterMapsToPhysical(283, 279);
            assertRouterMapsToPhysical(383, 279);
            assertRouterMapsToPhysical(384, 279);

            assertRouterMapsToPhysical(285, 284);
            assertRouterMapsToPhysical(286, 284);
            assertRouterMapsToPhysical(287, 284);
            assertRouterMapsToPhysical(288, 284);
            assertRouterMapsToPhysical(385, 284);
            assertRouterMapsToPhysical(386, 284);

            assertRouterMapsToPhysical(290, 289);
            assertRouterMapsToPhysical(291, 289);
            assertRouterMapsToPhysical(292, 289);
            assertRouterMapsToPhysical(293, 289);
            assertRouterMapsToPhysical(387, 289);
            assertRouterMapsToPhysical(388, 289);
      }

      enum Direction {
            Inc,
            Dec
      }

      enum Resolution {
            Coarse,
            Fine
      }

      class Setup {
            Setup(int hwParameterNumber,
                        double initialHwPosition,
                        double initialHWAmount,
                        double initialMacroControlPosition,
                        int targetParameterNumber,
                        double initialTargetControlPosition,
                        double initialModAmount) {
                  this.hwParameterNumber = hwParameterNumber;
                  this.initialHwPosition = initialHwPosition;
                  this.initialHWAmount = initialHWAmount;
                  this.initialMacroControlPosition = initialMacroControlPosition;
                  this.targetParameterNumber = targetParameterNumber;
                  this.initialTargetControlPosition = initialTargetControlPosition;
                  this.initialModAmount = initialModAmount;
            }

            int hwParameterNumber;
            double initialHwPosition;
            double initialHWAmount;
            double initialMacroControlPosition;
            int targetParameterNumber;
            double initialTargetControlPosition;
            double initialModAmount;
      }

      class TasksAndExpectations {
            TasksAndExpectations(Tasks tasks, Expectations expectations) {
                  this.tasks = tasks;
                  this.expectations = expectations;
            }

            Tasks tasks;
            Expectations expectations;
      }

      abstract class Task {
            public abstract void run(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target);
      }

      abstract class Expectation {
            public abstract void check(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target);
      }

      class Tasks {
            Tasks(Task... tasks) {
                  this.tasks = tasks;
            }

            Task[] tasks;
      }

      class SetHWPosition extends Task {
            SetHWPosition(double v) {
                  this.v = v;
            }

            @Override
            public void run(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target) {
                  EditBufferUseCases.get().setParameterValue(phys.id, this.v, true);
            }

            double v;

      }

      abstract class StepTask extends Task {
            public StepTask(int steps, Direction dir, Resolution res) {
                  this.steps = steps;
                  this.dir = dir;
                  this.res = res;
            }

            int steps;
            Direction dir;
            Resolution res;

      }

      class Expectations {
            Expectations(Expectation... e) {
                  this.e = e;
            }

            Expectation[] e;
      }

      class StepHWPosition extends StepTask {
            StepHWPosition(int steps, Direction dir, Resolution res) {
                  super(steps, dir, res);
            }

            @Override
            public void run(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target) {
                  for (int i = 0; i < this.steps; i++)
                        EditBufferUseCases.get().incDecParameter(phys.id, this.res == Resolution.Fine,
                                    this.dir == Direction.Inc ? 1 : -1);

            }
      }

      class StepModAmount extends StepTask {
            public StepModAmount(int steps, Direction dir, Resolution res) {
                  super(steps, dir, res);
            }

            @Override
            public void run(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target) {
                  for (int i = 0; i < this.steps; i++) {
                        if (this.dir == Direction.Inc)
                              EditBufferUseCases.get().incModulationAmount(target.id, this.res == Resolution.Fine);
                        else if (this.dir == Direction.Dec)
                              EditBufferUseCases.get().decModulationAmount(target.id, this.res == Resolution.Fine);
                  }
            }
      }

      class StepModRangeLow extends StepTask {
            public StepModRangeLow(int steps, Direction dir, Resolution res) {
                  super(steps, dir, res);
            }

            @Override
            public void run(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target) {
                  for (int i = 0; i < this.steps; i++) {
                        if (this.dir == Direction.Inc)
                              EditBufferUseCases.get().startEditModulationAmountLowerBound(target.id, 100)
                                          .inc(this.res == Resolution.Fine);
                        else if (this.dir == Direction.Dec)
                              EditBufferUseCases.get().startEditModulationAmountLowerBound(target.id, 100)
                                          .dec(this.res == Resolution.Fine);
                  }
            }
      }

      class StepModRangeHi extends StepTask {
            public StepModRangeHi(int steps, Direction dir, Resolution res) {
                  super(steps, dir, res);
            }

            @Override
            public void run(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target) {
                  for (int i = 0; i < this.steps; i++) {
                        if (this.dir == Direction.Inc)
                              EditBufferUseCases.get().startEditModulationAmountUpperBound(target.id, 100)
                                          .inc(this.res == Resolution.Fine);
                        else if (this.dir == Direction.Dec)
                              EditBufferUseCases.get().startEditModulationAmountUpperBound(target.id, 100)
                                          .dec(this.res == Resolution.Fine);
                  }
            }
      }

      abstract class ExpectValue extends Expectation {
            ExpectValue(double v) {
                  this.v = v;
            }

            double v;
      }

      abstract class ExpectString extends Expectation {
            ExpectString(String v) {
                  this.v = v;
            }

            String v;
      }

      class ExpectHWPosition extends ExpectValue {
            ExpectHWPosition(double v) {
                  super(v);
            }

            @Override
            public void check(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target) {

                  assertEquals(this.v, ParameterPresenterProviders.get().getParameterPresenter(VoiceGroup.Global,
                              phys.id.getNumber()).controlPosition);
            }
      }

      class ExpectControlPosition extends ExpectValue {
            ExpectControlPosition(double v) {
                  super(v);
            }

            @Override
            public void check(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target) {
                  assertEquals(this.v,
                              ParameterPresenterProviders.get().getParameterPresenter(target.id.getVoiceGroup(),
                                          target.id.getNumber()).controlPosition);
            }
      }

      class ExpectModAmount extends ExpectValue {
            ExpectModAmount(double v) {
                  super(v);
            }

            @Override
            public void check(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target) {

                  assertEquals(this.v,
                              ParameterPresenterProviders.get().getParameterPresenter(target.id.getVoiceGroup(),
                                          target.id.getNumber()).modulation.modulationAmount);
            }
      }

      class ExpectModRangeLow extends ExpectValue {

            ExpectModRangeLow(double v) {
                  super(v);
            }

            @Override
            public void check(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target) {

                  assertEquals(this.v,
                              ParameterPresenterProviders.get().getParameterPresenter(target.id.getVoiceGroup(),
                                          target.id.getNumber()).modulation.modRange.lower.clippedQuantized);
            }
      }

      class ExpectModRangeHi extends ExpectValue {
            ExpectModRangeHi(double v) {
                  super(v);
            }

            @Override
            public void check(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target) {

                  assertEquals(this.v,
                              ParameterPresenterProviders.get().getParameterPresenter(target.id.getVoiceGroup(),
                                          target.id.getNumber()).modulation.modRange.upper.clippedQuantized);
            }
      }

      class ExpectMacroPosition extends ExpectValue {
            ExpectMacroPosition(double v) {
                  super(v);
            }

            @Override
            public void check(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target) {
                  assertEquals(this.v, ParameterPresenterProviders.get().getParameterPresenter(VoiceGroup.Global,
                              mc.id.getNumber()).controlPosition);
            }
      }

      class ExpectHWPositionString extends ExpectString {
            ExpectHWPositionString(String v) {
                  super(v);
            }

            @Override
            public void check(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target) {

                  String toTest = this.v;
                  if (toTest != "0.0 %")
                        toTest = toTest.substring(2);

                  assertEquals(toTest, // the playground renders a "! ", that we do not(?) want.
                              ParameterPresenterProviders.get().getParameterPresenter(VoiceGroup.Global,
                                          phys.id.getNumber()).displayValues[0]);
            }
      }

      class ExpectModAmountString extends ExpectString {
            ExpectModAmountString(String v) {
                  super(v);
            }

            @Override
            public void check(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target) {
                  assertEquals("MC Amount: " + this.v,
                              ParameterPresenterProviders.get().getParameterPresenter(target.id.getVoiceGroup(),
                                          target.id.getNumber()).modulation.amountDisplayValues[0]);
            }
      }

      class ExpectMacroPositionString extends ExpectString {
            ExpectMacroPositionString(String v) {
                  super(v);
            }

            @Override
            public void check(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target) {
                  assertEquals(this.v, ParameterPresenterProviders.get().getParameterPresenter(VoiceGroup.Global,
                              mc.id.getNumber()).displayValues[0]);
            }
      }

      class ExpectControlPositionString extends ExpectString {
            ExpectControlPositionString(String v) {
                  super(v);
            }

            @Override
            public void check(PhysicalControlParameterModel phys, ModulationRouterParameterModel router,
                        MacroControlParameterModel mc, ModulateableParameterModel target) {
                  assertEquals(this.v,
                              ParameterPresenterProviders.get().getParameterPresenter(target.id.getVoiceGroup(),
                                          target.id.getNumber()).displayValues[0]);
            }
      }

      ParameterId findRouter(MacroControlParameterModel mc, int physicalParamNumber) {
            for (ParameterId routerId : mc.getRouterIDs()) {
                  ModulationRouterParameterModel r = (ModulationRouterParameterModel) EditBufferModel.get()
                              .getParameter(routerId);
                  ParameterId physicalControlID = r.getAssociatedPhysicalControlID();
                  if (physicalControlID.getNumber() == physicalParamNumber) {
                        return routerId;
                  }
            }

            throw new NoSuchElementException("could not find router");
      }

      void javaTestModulation(String name, Setup s, TasksAndExpectations... te) {
            EditBufferModel eb = EditBufferModel.get();
            PhysicalControlParameterModel phys = (PhysicalControlParameterModel) eb
                        .getParameter(new ParameterId(s.hwParameterNumber, VoiceGroup.Global));
            MacroControlParameterModel mc = (MacroControlParameterModel) eb.getParameter(new ParameterId(ModSource.A));
            ModulationRouterParameterModel router = (ModulationRouterParameterModel) eb
                        .getParameter(findRouter(mc, s.hwParameterNumber));
            ModulateableParameterModel target = (ModulateableParameterModel) eb
                        .getParameter(new ParameterId(s.targetParameterNumber, VoiceGroup.I));

            phys.value.value.setValue(s.initialHwPosition);
            router.value.value.setValue(s.initialHWAmount);
            mc.value.value.setValue(s.initialMacroControlPosition);
            target.value.value.setValue(s.initialTargetControlPosition);
            target.modSource.setValue(ModSource.A);
            target.modAmount.value.setValue(s.initialModAmount);

            EditBufferUseCases.get().updateModulationBase(target);

            for (TasksAndExpectations a : te) {
                  for (Task t : a.tasks.tasks)
                        t.run(phys, router, mc, target);

                  for (Expectation t : a.expectations.e)
                        t.check(phys, router, mc, target);
            }
      }

      private static native JavaScriptObject fetchXml() /*-{ 
            var res = null;
            var req = new XMLHttpRequest();
            req.addEventListener("load", function (event) { 
                  res = req.responseXML;
            });
            req.open("GET", "http://localhost:8080/get-update?updateID=0", false);
            req.send();
            return res;
      }-*/;

      private static native String xPath(JavaScriptObject doc, String path) /*-{ 
            return doc.evaluate(path, doc, null, XPathResult.STRING_TYPE, null).stringValue;
      }-*/;

      private void setupParam(JavaScriptObject x, BasicParameterModel p) {
            p.value.metaData.coarseDenominator
                        .setValue(Integer.valueOf(
                                    xPath(x, "//parameter[@id='" + p.id.toString() + "'][1]/coarse-denominator")));

            p.value.metaData.fineDenominator
                        .setValue(Integer.valueOf(
                                    xPath(x, "//parameter[@id='" + p.id.toString() + "'][1]/fine-denominator")));

            p.value.metaData.bipolar.setValue(xPath(x, "//parameter[@id='" + p.id.toString() + "'][1]/bipolar") == "1");

            p.value.metaData.scaling.setValue(xPath(x, "//parameter[@id='" + p.id.toString() + "'][1]/scaling"));

            if (p instanceof ModulateableParameterModel) {
                  ModulateableParameterModel m = (ModulateableParameterModel) p;
                  m.modAmount.metaData.coarseDenominator
                              .setValue(Integer.valueOf(
                                          xPath(x, "//parameter[@id='" + p.id.toString() + "'][1]/mod-amount-coarse")));

                  m.modAmount.metaData.fineDenominator
                              .setValue(Integer.valueOf(
                                          xPath(x, "//parameter[@id='" + p.id.toString() + "'][1]/mod-amount-fine")));

                  m.modAmount.metaData.scaling.setValue(
                              xPath(x, "//parameter[@id='" + p.id.toString() + "'][1]/mod-amount-stringizer"));
            }
      }

      private void setupParameters() {
            EditBufferModel eb = EditBufferModel.get();
            PhysicalControlParameterModel phys = (PhysicalControlParameterModel) eb
                        .getParameter(new ParameterId(274, VoiceGroup.Global));
            MacroControlParameterModel mc = (MacroControlParameterModel) eb.getParameter(new ParameterId(ModSource.A));
            ModulationRouterParameterModel router = (ModulationRouterParameterModel) eb
                        .getParameter(findRouter(mc, 274));

            ModulateableParameterModel fluctuation = (ModulateableParameterModel) eb
                        .getParameter(new ParameterId(57, VoiceGroup.I));

            ModulateableParameterModel fbMixComb = (ModulateableParameterModel) eb
                        .getParameter(new ParameterId(156, VoiceGroup.I));

            var x = fetchXml();
            setupParam(x, phys);
            setupParam(x, router);
            setupParam(x, mc);
            setupParam(x, fluctuation);
            setupParam(x, fbMixComb);
      }

      private void doTheTests() {
            setupParameters();
            ParameterPresenterProviders.get();

            javaTestModulation("step mod amount - unipolar",
                        new Setup(274, 0, 1, 0, 57, 0, 0.5),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModAmount(1, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModAmount(0.51),
                                                new ExpectModAmountString("51.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModAmount(1, Direction.Inc, Resolution.Fine)),
                                    new Expectations(
                                                new ExpectModAmount(0.511),
                                                new ExpectModAmountString("51.1 %"))));

            javaTestModulation("nothing special",
                        new Setup(274, 0, 1, 0, 57, 0, 1),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(1, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectHWPosition(0.01),
                                                new ExpectControlPosition(0.01),
                                                new ExpectModRangeLow(0),
                                                new ExpectModRangeHi(1))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(1, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectHWPosition(0),
                                                new ExpectControlPosition(0),
                                                new ExpectModRangeLow(0),
                                                new ExpectModRangeHi(1))));

            javaTestModulation("nothing special",
                        new Setup(274, 0, 1, 0, 57, 0, 1),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(1, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectHWPosition(0.01),
                                                new ExpectControlPosition(0.01),
                                                new ExpectModRangeLow(0),
                                                new ExpectModRangeHi(1))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(1, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectHWPosition(0),
                                                new ExpectControlPosition(0),
                                                new ExpectModRangeLow(0),
                                                new ExpectModRangeHi(1))));

            javaTestModulation("happy path with minimal effective positive coarse modulation",
                        new Setup(274, 0, 1, 0.5, 57, 0.5, 0.01),
                        new TasksAndExpectations(
                                    new Tasks(),
                                    new Expectations(
                                                new ExpectHWPositionString("0.0 %"),
                                                new ExpectModAmountString("1.0 %"),
                                                new ExpectMacroPositionString("50.0 %"),
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(-1)),
                                    new Expectations(
                                                new ExpectHWPositionString("! -100.0 %"),
                                                new ExpectMacroPositionString("0.0 %"),
                                                new ExpectControlPositionString("49.5 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(1)),
                                    new Expectations(
                                                new ExpectHWPositionString("! 100.0 %"),
                                                new ExpectMacroPositionString("100.0 %"),
                                                new ExpectControlPositionString("50.5 %"))));

            javaTestModulation("happy path with minimal effective negative coarse modulation",
                        new Setup(274, 0, 1, 0.5, 57, 0.5, -0.01),
                        new TasksAndExpectations(
                                    new Tasks(),
                                    new Expectations(
                                                new ExpectHWPositionString("0.0 %"),
                                                new ExpectModAmountString("-1.0 %"),
                                                new ExpectMacroPositionString("50.0 %"),
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(-1)),
                                    new Expectations(
                                                new ExpectHWPositionString("! -100.0 %"),
                                                new ExpectMacroPositionString("0.0 %"),
                                                new ExpectControlPositionString("50.5 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(1)),
                                    new Expectations(
                                                new ExpectHWPositionString("! 100.0 %"),
                                                new ExpectMacroPositionString("100.0 %"),
                                                new ExpectControlPositionString("49.5 %"))));

            javaTestModulation("happy path with minimal effective positive fine modulation",
                        new Setup(274, 0, 1, 0.5, 57, 0.5, 0.001),
                        new TasksAndExpectations(
                                    new Tasks(),
                                    new Expectations(
                                                new ExpectHWPositionString("0.0 %"),
                                                new ExpectModAmountString("0.1 %"),
                                                new ExpectMacroPositionString("50.0 %"),
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(-1)),
                                    new Expectations(
                                                new ExpectMacroPositionString("0.0 %"),
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(1)),
                                    new Expectations(
                                                new ExpectMacroPositionString("100.0 %"),
                                                new ExpectControlPositionString("50.0 %"))));

            javaTestModulation("happy path with minimal effective positive fine modulation",
                        new Setup(274, 0, 1, 0.5, 57, 0.5, 0.002),
                        new TasksAndExpectations(
                                    new Tasks(),
                                    new Expectations(
                                                new ExpectHWPositionString("0.0 %"),
                                                new ExpectModAmountString("0.2 %"),
                                                new ExpectMacroPositionString("50.0 %"),
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(-1)),
                                    new Expectations(
                                                new ExpectMacroPositionString("0.0 %"),
                                                new ExpectControlPositionString("49.9 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(1)),
                                    new Expectations(
                                                new ExpectMacroPositionString("100.0 %"),
                                                new ExpectControlPositionString("50.1 %"))));

            javaTestModulation("happy path with minimal effective negative fine modulation",
                        new Setup(274, 0, 1, 0.5, 57, 0.5, -0.001),
                        new TasksAndExpectations(
                                    new Tasks(),
                                    new Expectations(
                                                new ExpectHWPositionString("0.0 %"),
                                                new ExpectModAmountString("-0.1 %"),
                                                new ExpectMacroPositionString("50.0 %"),
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(-1)),
                                    new Expectations(
                                                new ExpectHWPositionString("! -100.0 %"),
                                                new ExpectMacroPositionString("0.0 %"),
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(1)),
                                    new Expectations(
                                                new ExpectHWPositionString("! 100.0 %"),
                                                new ExpectMacroPositionString("100.0 %"),
                                                new ExpectControlPositionString("49.9 %"))));

            javaTestModulation("happy path with all amounts set to minimal possible",
                        new Setup(274, 0, 0.001, 0.5, 57, 0.5, 0.001),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(100, Direction.Inc, Resolution.Fine)),
                                    new Expectations(
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(399, Direction.Inc, Resolution.Fine)),
                                    new Expectations(
                                                new ExpectMacroPositionString("50.0 %"),
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(1, Direction.Inc, Resolution.Fine)),
                                    new Expectations(
                                                new ExpectMacroPositionString("50.1 %"),
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(1, Direction.Dec, Resolution.Fine)),
                                    new Expectations(
                                                new ExpectMacroPositionString("50.0 %"),
                                                new ExpectControlPositionString("50.0 %"))));

            javaTestModulation("happy path with 1 to 1 mapping",
                        new Setup(274, 0, 1, 0.5, 57, 0, 1),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(1)),
                                    new Expectations(
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(-1)),
                                    new Expectations(
                                                new ExpectControlPositionString("0.0 %"))));

            javaTestModulation("step lower mod bound",
                        new Setup(274, 0, 1, 0.5, 57, 0.5, 0.5),
                        new TasksAndExpectations(
                                    new Tasks(),
                                    new Expectations(
                                                new ExpectModRangeLow(0.25),
                                                new ExpectControlPosition(0.5),
                                                new ExpectModRangeHi(0.75))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeLow(1, Direction.Dec, Resolution.Fine)),
                                    new Expectations(
                                                new ExpectModRangeLow(0.249),
                                                new ExpectControlPosition(0.5),
                                                new ExpectModRangeHi(0.75))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeLow(1, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(0.24),
                                                new ExpectControlPosition(0.495),
                                                new ExpectModRangeHi(0.75))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeLow(1, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(0.23),
                                                new ExpectControlPosition(0.49),
                                                new ExpectModRangeHi(0.75))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeLow(23, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(0),
                                                new ExpectControlPosition(0.375),
                                                new ExpectModRangeHi(0.75))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeLow(1, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(0),
                                                new ExpectControlPosition(0.375),
                                                new ExpectModRangeHi(0.75))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeLow(25, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(0.25),
                                                new ExpectControlPosition(0.5),
                                                new ExpectModRangeHi(0.75))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeLow(60, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(0.85),
                                                new ExpectControlPosition(0.8),
                                                new ExpectModRangeHi(0.75))));

            javaTestModulation("step upper mod bound",
                        new Setup(274, 0, 1, 0.5, 57, 0.5, 0.5),
                        new TasksAndExpectations(
                                    new Tasks(),
                                    new Expectations(
                                                new ExpectModRangeLow(0.25),
                                                new ExpectControlPosition(0.5),
                                                new ExpectModRangeHi(0.75))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeHi(1, Direction.Inc, Resolution.Fine)),
                                    new Expectations(
                                                new ExpectModRangeLow(0.25),
                                                new ExpectControlPosition(0.5),
                                                new ExpectModRangeHi(0.751))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeHi(1, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(0.25),
                                                new ExpectControlPosition(0.505),
                                                new ExpectModRangeHi(0.76))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeHi(1, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(0.25),
                                                new ExpectControlPosition(0.51),
                                                new ExpectModRangeHi(0.77))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeHi(23, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(0.25),
                                                new ExpectControlPosition(0.625),
                                                new ExpectModRangeHi(1))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeHi(1, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(0.25),
                                                new ExpectControlPosition(0.625),
                                                new ExpectModRangeHi(1))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeHi(25, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(0.25),
                                                new ExpectControlPosition(0.5),
                                                new ExpectModRangeHi(0.75))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeHi(60, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(0.25),
                                                new ExpectControlPosition(0.2),
                                                new ExpectModRangeHi(0.15))));

            javaTestModulation("step mod amount - bipolar",
                        new Setup(274, 0, 1, 0, 156, 0, 0.5),
                        new TasksAndExpectations(
                                    new Tasks(),
                                    new Expectations(
                                                new ExpectModAmount(0.5),
                                                new ExpectModAmountString("100.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModAmount(1, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModAmount(0.505),
                                                new ExpectModAmountString("101.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModAmount(1, Direction.Inc, Resolution.Fine)),
                                    new Expectations(
                                                new ExpectModAmount(0.5055),
                                                new ExpectModAmountString("101.1 %"))));

            javaTestModulation("nothing special",
                        new Setup(274, 0, 1, 0, 156, 0, 1),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(1, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectHWPosition(0.01),
                                                new ExpectMacroPosition(0.01),
                                                new ExpectControlPosition(0.02),
                                                new ExpectModRangeLow(0),
                                                new ExpectModRangeHi(1))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(1, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectHWPosition(0),
                                                new ExpectMacroPosition(0),
                                                new ExpectControlPosition(0),
                                                new ExpectModRangeLow(0),
                                                new ExpectModRangeHi(1))));

            javaTestModulation("nothing special",
                        new Setup(274, 0, 1, 0, 156, 0, 1),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(1, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectHWPosition(0.01),
                                                new ExpectControlPosition(0.02),
                                                new ExpectModRangeLow(0),
                                                new ExpectModRangeHi(1))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(1, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectHWPosition(0),
                                                new ExpectControlPosition(0),
                                                new ExpectModRangeLow(0),
                                                new ExpectModRangeHi(1))));

            javaTestModulation("nothing special",
                        new Setup(274, 0, 1, 0, 156, 0, 0.5),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(1, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectHWPosition(0.01),
                                                new ExpectMacroPosition(0.01),
                                                new ExpectControlPosition(0.01),
                                                new ExpectModRangeLow(0),
                                                new ExpectModRangeHi(1))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(1, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectHWPosition(0),
                                                new ExpectMacroPosition(0),
                                                new ExpectControlPosition(0),
                                                new ExpectModRangeLow(0),
                                                new ExpectModRangeHi(1))));

            javaTestModulation("happy path with minimal effective positive coarse modulation",
                        new Setup(274, 0, 1, 0.5, 156, 0.5, 0.01),
                        new TasksAndExpectations(
                                    new Tasks(),
                                    new Expectations(
                                                new ExpectHWPositionString("0.0 %"),
                                                new ExpectModAmountString("2.0 %"),
                                                new ExpectMacroPositionString("50.0 %"),
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(-1)),
                                    new Expectations(
                                                new ExpectHWPositionString("! -100.0 %"),
                                                new ExpectMacroPositionString("0.0 %"),
                                                new ExpectControlPositionString("49.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(1)),
                                    new Expectations(
                                                new ExpectHWPositionString("! 100.0 %"),
                                                new ExpectMacroPositionString("100.0 %"),
                                                new ExpectControlPositionString("51.0 %"))));

            javaTestModulation("happy path with minimal effective negative coarse modulation",
                        new Setup(274, 0, 1, 0.5, 156, 0.5, -0.01),
                        new TasksAndExpectations(
                                    new Tasks(),
                                    new Expectations(
                                                new ExpectHWPositionString("0.0 %"),
                                                new ExpectModAmountString("-2.0 %"),
                                                new ExpectMacroPositionString("50.0 %"),
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(-1)),
                                    new Expectations(
                                                new ExpectHWPositionString("! -100.0 %"),
                                                new ExpectMacroPositionString("0.0 %"),
                                                new ExpectControlPositionString("51.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(1)),
                                    new Expectations(
                                                new ExpectHWPositionString("! 100.0 %"),
                                                new ExpectMacroPositionString("100.0 %"),
                                                new ExpectControlPositionString("49.0 %"))));

            javaTestModulation("happy path with minimal effective positive fine modulation",
                        new Setup(274, 0, 1, 0.5, 156, 0.5, 0.001),
                        new TasksAndExpectations(
                                    new Tasks(),
                                    new Expectations(
                                                new ExpectHWPositionString("0.0 %"),
                                                new ExpectModAmountString("0.2 %"),
                                                new ExpectMacroPositionString("50.0 %"),
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(-1)),
                                    new Expectations(
                                                new ExpectHWPositionString("! -100.0 %"),
                                                new ExpectMacroPositionString("0.0 %"),
                                                new ExpectControlPositionString("49.9 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(1)),
                                    new Expectations(
                                                new ExpectHWPositionString("! 100.0 %"),
                                                new ExpectMacroPositionString("100.0 %"),
                                                new ExpectControlPositionString("50.1 %"))));

            javaTestModulation("happy path with minimal effective negative fine modulation",
                        new Setup(274, 0, 1, 0.5, 156, 0.5, -0.001),
                        new TasksAndExpectations(
                                    new Tasks(),
                                    new Expectations(
                                                new ExpectHWPositionString("0.0 %"),
                                                new ExpectModAmountString("-0.2 %"),
                                                new ExpectMacroPositionString("50.0 %"),
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(-1)),
                                    new Expectations(
                                                new ExpectHWPositionString("! -100.0 %"),
                                                new ExpectMacroPositionString("0.0 %"),
                                                new ExpectControlPositionString("50.1 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(1)),
                                    new Expectations(
                                                new ExpectHWPositionString("! 100.0 %"),
                                                new ExpectMacroPositionString("100.0 %"),
                                                new ExpectControlPositionString("49.9 %"))));

            javaTestModulation("happy path with all amounts set to minimal possible",
                        new Setup(274, 0, 0.001, 0.5, 156, 0.5, 0.001),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(100, Direction.Inc, Resolution.Fine)),
                                    new Expectations(
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(399, Direction.Inc, Resolution.Fine)),
                                    new Expectations(
                                                new ExpectMacroPositionString("50.0 %"),
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(1, Direction.Inc, Resolution.Fine)),
                                    new Expectations(
                                                new ExpectMacroPositionString("50.1 %"),
                                                new ExpectControlPositionString("50.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepHWPosition(1, Direction.Dec, Resolution.Fine)),
                                    new Expectations(
                                                new ExpectMacroPositionString("50.0 %"),
                                                new ExpectControlPositionString("50.0 %"))));

            javaTestModulation("happy path with 1 to 1 mapping",
                        new Setup(274, 0, 1, 0.5, 156, 0, 1),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(1)),
                                    new Expectations(
                                                new ExpectControlPositionString("100.0 %"))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new SetHWPosition(-1)),
                                    new Expectations(
                                                new ExpectControlPositionString("-100.0 %"))));

            javaTestModulation("step lower mod bound on bipolar parameter",
                        new Setup(274, 0, 1, 0.5, 156, 0, 0.25),
                        new TasksAndExpectations(
                                    new Tasks(),
                                    new Expectations(
                                                new ExpectModRangeLow(-0.25),
                                                new ExpectControlPosition(0),
                                                new ExpectModRangeHi(0.25))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeLow(1, Direction.Dec, Resolution.Fine)),
                                    new Expectations(
                                                new ExpectModRangeLow(-0.251),
                                                new ExpectControlPosition(-0.001),
                                                new ExpectModRangeHi(0.25))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeLow(1, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(-0.26),
                                                new ExpectControlPosition(-0.005),
                                                new ExpectModRangeHi(0.25))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeLow(1, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(-0.27),
                                                new ExpectControlPosition(-0.01),
                                                new ExpectModRangeHi(0.25))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeLow(73, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(-1),
                                                new ExpectControlPosition(-0.375),
                                                new ExpectModRangeHi(0.25))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeLow(1, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(-1),
                                                new ExpectControlPosition(-0.375),
                                                new ExpectModRangeHi(0.25))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeLow(75, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(-0.25),
                                                new ExpectControlPosition(0),
                                                new ExpectModRangeHi(0.25))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeLow(110, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(0.85),
                                                new ExpectControlPosition(0.55),
                                                new ExpectModRangeHi(0.25))));

            javaTestModulation("step upper mod bound on bipolar parameter",
                        new Setup(274, 0, 1, 0.5, 156, 0, 0.25),
                        new TasksAndExpectations(
                                    new Tasks(),
                                    new Expectations(
                                                new ExpectModRangeLow(-0.25),
                                                new ExpectControlPosition(0),
                                                new ExpectModRangeHi(0.25))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeHi(1, Direction.Inc, Resolution.Fine)),
                                    new Expectations(
                                                new ExpectModRangeLow(-0.25),
                                                new ExpectControlPosition(0.001),
                                                new ExpectModRangeHi(0.251))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeHi(1, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(-0.25),
                                                new ExpectControlPosition(0.005),
                                                new ExpectModRangeHi(0.26))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeHi(1, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(-0.25),
                                                new ExpectControlPosition(0.01),
                                                new ExpectModRangeHi(0.27))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeHi(73, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(-0.25),
                                                new ExpectControlPosition(0.375),
                                                new ExpectModRangeHi(1))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeHi(1, Direction.Inc, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(-0.25),
                                                new ExpectControlPosition(0.375),
                                                new ExpectModRangeHi(1))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeHi(25, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(-0.25),
                                                new ExpectControlPosition(0.25),
                                                new ExpectModRangeHi(0.75))),
                        new TasksAndExpectations(
                                    new Tasks(
                                                new StepModRangeHi(110, Direction.Dec, Resolution.Coarse)),
                                    new Expectations(
                                                new ExpectModRangeLow(-0.25),
                                                new ExpectControlPosition(-0.3),
                                                new ExpectModRangeHi(-0.35))));

      }
}