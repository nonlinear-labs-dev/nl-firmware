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

	public EnumDataModelEntity<Color> color = new EnumDataModelEntity<Color>(Color.class, Color.none);
	public StringDataModelEntity comment = new StringDataModelEntity();
	public StringDataModelEntity deviceName = new StringDataModelEntity();
	public DateDataModelEntity storeTime = new DateDataModelEntity();
	
	static public EditBufferModel get() {
		return theInstance;
	}

	private EditBufferModel() {
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
		for(BasicParameterModel param: parameters.values()) {
			if(param.isChanged())
				return true;
		}
		return false;
	}

}
