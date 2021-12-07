package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.BooleanDataModelEntity;
import com.nonlinearlabs.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.client.dataModel.Updater;
import com.nonlinearlabs.client.world.maps.parameters.PhysicalControlParameter.ReturnMode;

public class SendParameterModel extends BasicParameterModel {

	public BooleanDataModelEntity enabled = new BooleanDataModelEntity();
	public EnumDataModelEntity<ReturnMode> mode = new EnumDataModelEntity<ReturnMode>(ReturnMode.class, ReturnMode.None);

	public SendParameterModel(ParameterId id) {
		super(id);
		enabled.onChange(v -> notifyChanges());
	}

	@Override
	public Updater createUpdater(Node c) {
		return new SendParameterModelUpdater(c, this);
	}
}
