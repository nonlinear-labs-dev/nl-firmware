package com.nonlinearlabs.client.dataModel.editBuffer;

import java.util.HashMap;
import java.util.function.Function;

import com.nonlinearlabs.client.dataModel.BooleanDataModelEntity;
import com.nonlinearlabs.client.dataModel.DateDataModelEntity;
import com.nonlinearlabs.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.client.dataModel.IntegerDataModelEntity;
import com.nonlinearlabs.client.dataModel.StringDataModelEntity;
import com.nonlinearlabs.client.dataModel.ValueDataModelEntity;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel.ModSource;

public class EditBufferModel {

	public static enum Color {
		green, blue, yellow, orange, purple, red, none;
	}

	public static enum SoundType {
		Single, Split, Layer
	}

	public static enum VoiceGroup {
		I, II
	}

	private static HashMap<String, ParameterGroupModel> parameterGroups = new HashMap<String, ParameterGroupModel>();
	private static HashMap<Integer, BasicParameterModel> parameters = new HashMap<Integer, BasicParameterModel>();

	public static IntegerDataModelEntity selectedParameter = new IntegerDataModelEntity();
	public static StringDataModelEntity loadedPreset = new StringDataModelEntity();
	public static StringDataModelEntity loadedPresetName = new StringDataModelEntity();
	public static StringDataModelEntity loadedPresetBankName = new StringDataModelEntity();
	public static BooleanDataModelEntity isZombie = new BooleanDataModelEntity();
	public static BooleanDataModelEntity isModified = new BooleanDataModelEntity();
	public static BooleanDataModelEntity isChanged = new BooleanDataModelEntity();

	public static EnumDataModelEntity<Color> color = new EnumDataModelEntity<Color>(Color.class, Color.none);
	public static EnumDataModelEntity<SoundType> soundType = new EnumDataModelEntity<SoundType>(SoundType.class,
			SoundType.Split);
	public static StringDataModelEntity comment = new StringDataModelEntity();
	public static StringDataModelEntity deviceName = new StringDataModelEntity();
	public static DateDataModelEntity storeTime = new DateDataModelEntity();

	// dual voice mock
	// TODO
	public static StringDataModelEntity loadedPresetInVG1 = new StringDataModelEntity("Chili");
	public static StringDataModelEntity loadedPresetInVG2 = new StringDataModelEntity("Jalapeño");

	public static ParameterGroupModel getGroup(String id) {
		ParameterGroupModel g = parameterGroups.get(id);

		if (g == null) {
			g = new ParameterGroupModel();
			parameterGroups.put(id, g);
		}

		return g;
	}

	public static BasicParameterModel findParameter(int id) {
		BasicParameterModel p = parameters.get(id);

		if (p == null)
			p = addParameter(id);

		return p;
	}

	public static BasicParameterModel getSelectedParameter() {
		return parameters.get(selectedParameter.getValue());
	}

	public static BasicParameterModel addParameter(int id) {
		BasicParameterModel p = ParameterFactory.create(id);

		if (p != null)
			parameters.put(id, p);

		return p;
	}

	public static void onParameterChange(int parameterId, Function<ValueDataModelEntity, Boolean> cb) {
		findParameter(parameterId).value.onChange(cb);
	}

	public static boolean isAnyParamChanged() {
		for (BasicParameterModel param : parameters.values()) {
			if (param.isChanged())
				return true;
		}
		return false;
	}

	public static String getPresetNameOfVoiceGroup(VoiceGroup group) {
		switch (group) {
		case I:
			return loadedPresetInVG1.getValue();

		case II:
			return loadedPresetInVG2.getValue();
		}
		return "";
	}

	public static MacroControlParameterModel findParameter(ModSource value) {
		return (MacroControlParameterModel) findParameter(value.toParameterId());
	}
}