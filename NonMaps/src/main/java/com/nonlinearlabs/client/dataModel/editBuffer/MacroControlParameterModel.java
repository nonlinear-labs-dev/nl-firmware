package com.nonlinearlabs.client.dataModel.editBuffer;

import com.nonlinearlabs.client.dataModel.StringDataModelEntity;

public class MacroControlParameterModel extends BasicParameterModel {
	public StringDataModelEntity givenName = new StringDataModelEntity();
	public StringDataModelEntity info = new StringDataModelEntity();

	public MacroControlParameterModel(int id) {
		super(id);

		givenName.onChange(e -> notifyChanges());
		info.onChange(e -> notifyChanges());
	}

}
