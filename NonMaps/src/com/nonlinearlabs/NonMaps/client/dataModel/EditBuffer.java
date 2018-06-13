package com.nonlinearlabs.NonMaps.client.dataModel;

import java.util.HashMap;
import java.util.function.Function;

public class EditBuffer {

	public class Parameter extends DataModelEntity<Parameter> {
		public Parameter(Parameter def) {
			super(def);
		}

		public double value = 0.0;
		public double defaultValue = 0.0;
		public String scaling = "";
		public boolean isBipolar = false;
		public String longName;
		public String shortName;
		public int coarseDenominator = 100;
		public int fineDenominator = 1000;

		@Override
		public Parameter getValue() {
			return null;
		}

		@Override
		public void fromString(String str) {
			// TODO Auto-generated method stub

		}
	}

	public class UnmodulateableParameter extends Parameter {

		public UnmodulateableParameter(Parameter def) {
			super(def);
		}
	}

	public enum ModulationSource {
		None, A, B, C, D
	}

	public enum RibbonTouchBehavior {

	}

	public enum RibbonReturnMode {

	}

	public enum PedalMode {

	}

	public class ModulateableParameter extends Parameter {

		public ModulateableParameter(Parameter def) {
			super(def);
		}

		double modAmount = 0.0;
		ModulationSource modSource = ModulationSource.None;
	}

	public class MacroControlParameter extends UnmodulateableParameter {
		public MacroControlParameter(Parameter def) {
			super(def);
		}

		String givenName;
	}

	public class PhysicalControlParameter extends UnmodulateableParameter {

		public PhysicalControlParameter(Parameter def) {
			super(def);
		}

	}

	public class RibbonParameter extends PhysicalControlParameter {
		public RibbonParameter(Parameter def) {
			super(def);
		}

		public RibbonTouchBehavior touchBehavior;
		public RibbonReturnMode returnMode;
	}

	public class PedalParameter extends PhysicalControlParameter {
		public PedalParameter(Parameter def) {
			super(def);
		}

		public PedalMode pedalMode;
	}

	public class AftertouchParameter extends PhysicalControlParameter {

		public AftertouchParameter(Parameter def) {
			super(def);
		}

	}

	public class BenderParameter extends PhysicalControlParameter {

		public BenderParameter(Parameter def) {
			super(def);
		}

	}

	public class ParameterGroup {
		private HashMap<Integer, Parameter> parameters = new HashMap<Integer, Parameter>();

		public String longName;
		public String shortName;

		public Parameter getParameter(int id) {
			return parameters.get(id);
		}

		public void onParameterChange(int parameterId, Function<Parameter, Boolean> cb) {
			getParameter(parameterId).onChange(cb);
		}
	}

	private static EditBuffer theInstance = new EditBuffer();
	private HashMap<String, ParameterGroup> parameterGroups = new HashMap<String, ParameterGroup>();

	public Parameter selectedParameter;
	public PresetManager.Bank.Preset loadedPreset;

	public String color;
	public String comment;
	public String deviceName;
	public String storeTime;

	static public EditBuffer get() {
		return theInstance;
	}

	private EditBuffer() {
	}

	public ParameterGroup getGroup(String id) {
		return parameterGroups.get(id);
	}

	public void onParameterChange(String groupId, int parameterId, Function<Parameter, Boolean> cb) {
		getGroup(groupId).onParameterChange(parameterId, cb);
	}

}
