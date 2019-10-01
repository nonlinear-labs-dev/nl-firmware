package com.nonlinearlabs.NonMaps.client.dataModel.editBuffer;

import java.util.HashMap;
import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.BooleanDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.DateDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.IntegerDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.StringDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.ValueDataModelEntity;

public class EditBufferModel {

	private static EditBufferModel theInstance = new EditBufferModel();
	private static ParameterFactory factory = new ParameterFactory();

	public enum Color {
		green, blue, yellow, orange, purple, red, none;
	}

	private HashMap<String, ParameterGroupModel> parameterGroups = new HashMap<String, ParameterGroupModel>();
	private HashMap<Integer, BasicParameterModel> parameters = new HashMap<Integer, BasicParameterModel>();

	public IntegerDataModelEntity selectedParameter = new IntegerDataModelEntity();
	public StringDataModelEntity loadedPreset = new StringDataModelEntity();
	public StringDataModelEntity loadedPresetName = new StringDataModelEntity();
	public StringDataModelEntity loadedPresetBankName = new StringDataModelEntity();
	public BooleanDataModelEntity isZombie = new BooleanDataModelEntity();
	public BooleanDataModelEntity isModified = new BooleanDataModelEntity();
	public BooleanDataModelEntity isChanged = new BooleanDataModelEntity();

	public EnumDataModelEntity<Color> color = new EnumDataModelEntity<Color>(Color.class, Color.none);
	public StringDataModelEntity comment = new StringDataModelEntity();
	public StringDataModelEntity deviceName = new StringDataModelEntity();
	public DateDataModelEntity storeTime = new DateDataModelEntity();

	static public EditBufferModel get() {
		return theInstance;
	}

	private EditBufferModel() {
	}

	@Override
	public String toString() {
		String ret = "";
		ret += "Selected Parameter: " + selectedParameter.getValue().toString() + "\n";
		ret += "Loaded Preset: " + loadedPreset.getValue() + "\n";
		ret += "Loaded Preset Name: " + loadedPresetName.getValue() + "\n";
		ret += "Loaded Preset Bank Name: " + loadedPresetBankName.getValue() + "\n";
		ret += "Is Zombie: " + isZombie.getValue().toString() + "\n";
		ret += "Is Modified: " + isModified.getValue().toString() + "\n";
		ret += "Is Changed: " + isChanged.getValue().toString() + "\n";
		return ret;
	}

	public ParameterGroupModel getGroup(String id) {
		ParameterGroupModel g = parameterGroups.get(id);

		if (g == null) {
			g = new ParameterGroupModel();
			parameterGroups.put(id, g);
		}

		return g;
	}

	public BasicParameterModel findParameter(int id) {
		BasicParameterModel p = parameters.get(id);

		if (p == null)
			p = addParameter(id);

		return p;
	}

	public BasicParameterModel getSelectedParameter() {
		return parameters.get(selectedParameter.getValue());
	}

	public BasicParameterModel addParameter(int id) {
		BasicParameterModel p = factory.create(id);

		if (p != null)
			parameters.put(id, p);

		return p;
	}

	public void onParameterChange(String groupId, int parameterId, Function<ValueDataModelEntity, Boolean> cb) {
		findParameter(parameterId).value.onChange(cb);
	}

	public boolean isAnyParamChanged() {
		for (BasicParameterModel param : parameters.values()) {
			if (param.isChanged())
				return true;
		}
		return false;
	}

}
