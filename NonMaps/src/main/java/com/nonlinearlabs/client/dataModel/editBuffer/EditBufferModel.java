package com.nonlinearlabs.client.dataModel.editBuffer;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import com.nonlinearlabs.client.dataModel.BooleanDataModelEntity;
import com.nonlinearlabs.client.dataModel.DateDataModelEntity;
import com.nonlinearlabs.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.client.dataModel.IntegerDataModelEntity;
import com.nonlinearlabs.client.dataModel.Notifier;
import com.nonlinearlabs.client.dataModel.StringDataModelEntity;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel.ModSource;

public class EditBufferModel extends Notifier<EditBufferModel> {
	private static EditBufferModel theModel = new EditBufferModel();

	public static EditBufferModel get() {
		return theModel;
	}

	public static enum Color {
		green, blue, yellow, orange, purple, red, none;
	}

	public static enum SoundType {
		Single, Split, Layer
	}

	public static enum VoiceGroup {
		I, II, Global
	}

	public static class ByVoiceGroup {
		public HashMap<String, ParameterGroupModel> parameterGroups = new HashMap<String, ParameterGroupModel>();
		public HashMap<Integer, BasicParameterModel> parameters = new HashMap<Integer, BasicParameterModel>();
		public ArrayList<ModulateableParameterModel> modulateableParametersCache = new ArrayList<ModulateableParameterModel>();
	}

	public ByVoiceGroup[] byVoiceGroup = { new ByVoiceGroup(), new ByVoiceGroup(), new ByVoiceGroup() };

	public IntegerDataModelEntity selectedParameter = new IntegerDataModelEntity();
	public StringDataModelEntity loadedPreset = new StringDataModelEntity();
	public StringDataModelEntity loadedPresetName = new StringDataModelEntity();
	public StringDataModelEntity loadedPresetBankName = new StringDataModelEntity();
	public BooleanDataModelEntity isZombie = new BooleanDataModelEntity();
	public BooleanDataModelEntity isModified = new BooleanDataModelEntity();
	public BooleanDataModelEntity isChanged = new BooleanDataModelEntity();

	public EnumDataModelEntity<Color> color = new EnumDataModelEntity<Color>(Color.class, Color.none);
	public EnumDataModelEntity<SoundType> soundType = new EnumDataModelEntity<SoundType>(SoundType.class,
			SoundType.Split);
	public EnumDataModelEntity<VoiceGroup> voiceGroup = new EnumDataModelEntity<VoiceGroup>(VoiceGroup.class,
			VoiceGroup.I);
	public StringDataModelEntity comment = new StringDataModelEntity();
	public StringDataModelEntity deviceName = new StringDataModelEntity();
	public DateDataModelEntity storeTime = new DateDataModelEntity();

	// dual voice mock
	// TODO
	public StringDataModelEntity loadedPresetInVG1 = new StringDataModelEntity("Chili");
	public StringDataModelEntity loadedPresetInVG2 = new StringDataModelEntity("Jalapeño");

	public ParameterGroupModel getOrCreateGroup(String id, VoiceGroup vg) {
		ParameterGroupModel g = byVoiceGroup[vg.ordinal()].parameterGroups.get(id);

		if (g == null) {
			g = new ParameterGroupModel();
			byVoiceGroup[vg.ordinal()].parameterGroups.put(id, g);
			notifyChanges();
		}

		return g;
	}

	public BasicParameterModel getOrCreateParameter(int id, VoiceGroup vg) {
		BasicParameterModel p = byVoiceGroup[vg.ordinal()].parameters.get(id);

		if (p == null)
			p = addParameter(id, vg);

		return p;
	}

	public BasicParameterModel getSelectedParameter() {
		int paramID = selectedParameter.getValue();
		BasicParameterModel global = byVoiceGroup[VoiceGroup.Global.ordinal()].parameters.get(paramID);
		if (global != null)
			return global;

		return byVoiceGroup[voiceGroup.getValue().ordinal()].parameters.get(paramID);
	}

	public BasicParameterModel addParameter(int id, VoiceGroup vg) {
		BasicParameterModel p = ParameterFactory.create(id);

		if (p != null) {
			byVoiceGroup[vg.ordinal()].parameters.put(id, p);

			if (p instanceof ModulateableParameterModel)
				byVoiceGroup[vg.ordinal()].modulateableParametersCache.add((ModulateableParameterModel) p);
		}
		notifyChanges();
		return p;
	}

	public String getPresetNameOfVoiceGroup(VoiceGroup group) {
		switch (group) {
		case I:
		case Global:
			return loadedPresetInVG1.getValue();

		case II:
			return loadedPresetInVG2.getValue();
		}
		return "";
	}

	public MacroControlParameterModel getOrCreateParameter(ModSource value, VoiceGroup vg) {
		return (MacroControlParameterModel) getOrCreateParameter(value.toParameterId(), vg);
	}

	public List<ModulateableParameterModel> getAllModulateableParameters() {
		return byVoiceGroup[voiceGroup.getValue().ordinal()].modulateableParametersCache;
	}

	public ParameterGroupModel getAnyGroup(String id) {
		ParameterGroupModel g = byVoiceGroup[VoiceGroup.Global.ordinal()].parameterGroups.get(id);
		if (g != null)
			return g;
		return byVoiceGroup[VoiceGroup.I.ordinal()].parameterGroups.get(id);
	}

	public BasicParameterModel getAnyParameter(int id) {
		BasicParameterModel p = byVoiceGroup[VoiceGroup.Global.ordinal()].parameters.get(id);

		if (p != null)
			return p;

		return byVoiceGroup[VoiceGroup.I.ordinal()].parameters.get(id);
	}

	@Override
	public EditBufferModel getValue() {
		return this;
	}

}