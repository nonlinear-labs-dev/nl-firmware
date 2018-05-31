package com.nonlinearlabs.NonMaps.client.dataModel;

import java.util.HashMap;
import java.util.function.Consumer;

public class EditBuffer {

	public class Parameter extends DataModelEntity<Parameter> {
		public double value = 0.0;
		public double defaultValue = 0.0;
		public String scaling = "";
		public boolean isBipolar = false;
		public String longName;
		public String shortName;
		public int coarseDenominator = 100;
		public int fineDenominator = 1000;
	}

	public class UnmodulateableParameter extends Parameter {
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

		double modAmount = 0.0;
		ModulationSource modSource = ModulationSource.None;
	}

	public class MacroControlParameter extends UnmodulateableParameter {
		String givenName;
	}

	public class PhysicalControlParameter extends UnmodulateableParameter {

	}

	public class RibbonParameter extends PhysicalControlParameter {
		public RibbonTouchBehavior touchBehavior;
		public RibbonReturnMode returnMode;
	}

	public class PedalParameter extends PhysicalControlParameter {
		public PedalMode pedalMode;
	}

	public class AftertouchParameter extends PhysicalControlParameter {

	}

	public class BenderParameter extends PhysicalControlParameter {

	}

	public class ParameterGroup {
		private HashMap<Integer, Parameter> parameters = new HashMap<Integer, Parameter>();

		public String longName;
		public String shortName;

		public Parameter getParameter(int id) {
			return parameters.get(id);
		}

		public void onParameterChange(int parameterId, Consumer<Parameter> cb) {
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

	public void onParameterChange(String groupId, int parameterId, Consumer<Parameter> cb) {
		getGroup(groupId).onParameterChange(parameterId, cb);
	}

}
