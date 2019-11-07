package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.StringDataModelEntity;
import com.nonlinearlabs.client.dataModel.Updater;

public class MacroControlParameterModel extends BasicParameterModel {
	public StringDataModelEntity givenName = new StringDataModelEntity();
	public StringDataModelEntity info = new StringDataModelEntity();

	public MacroControlParameterModel(int id) {
		super(id);

		givenName.onChange(e -> notifyChanges());
		info.onChange(e -> notifyChanges());
	}

	public int[] getRouterIDs() {
		return new int[] { id + 12, id + 17, id + 22, id + 27, id + 32, id + 37, id + 42, id + 47 };
	}

	@Override
	public Updater createUpdater(Node c) {
		return new MacroControlParameterModelUpdater(c, this);
	}

}
