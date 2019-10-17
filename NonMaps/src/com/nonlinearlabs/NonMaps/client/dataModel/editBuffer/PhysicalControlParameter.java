package com.nonlinearlabs.NonMaps.client.dataModel.editBuffer;

public class PhysicalControlParameter extends BasicParameterModel {
	public PhysicalControlParameter(int id) {
		super(id);
	}

	@Override
	public boolean isValueChanged() {
		return false;
	}

	@Override
	public boolean isChanged() {
		return false;
	}
}
