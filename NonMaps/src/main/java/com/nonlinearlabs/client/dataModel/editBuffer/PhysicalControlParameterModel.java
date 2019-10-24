package com.nonlinearlabs.client.dataModel.editBuffer;

public class PhysicalControlParameterModel extends BasicParameterModel {
	public PhysicalControlParameterModel(int id) {
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
