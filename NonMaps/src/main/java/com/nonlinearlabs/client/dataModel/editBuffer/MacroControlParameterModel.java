package com.nonlinearlabs.client.dataModel.editBuffer;

import com.nonlinearlabs.client.dataModel.StringDataModelEntity;

public class MacroControlParameterModel extends BasicParameterModel {
	public MacroControlParameterModel(int id) {
		super(id);
	}

	public StringDataModelEntity givenName = new StringDataModelEntity();
	public StringDataModelEntity info = new StringDataModelEntity();
}
