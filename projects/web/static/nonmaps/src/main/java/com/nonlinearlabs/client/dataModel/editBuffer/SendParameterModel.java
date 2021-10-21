package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.BooleanDataModelEntity;
import com.nonlinearlabs.client.dataModel.Updater;

public class SendParameterModel extends BasicParameterModel {

	public BooleanDataModelEntity enabled = new BooleanDataModelEntity();

	public SendParameterModel(ParameterId id) {
		super(id);
		enabled.onChange(v -> notifyChanges());
	}

	@Override
	public Updater createUpdater(Node c) {
		return new SendParameterModelUpdater(c, this);
	}
}
