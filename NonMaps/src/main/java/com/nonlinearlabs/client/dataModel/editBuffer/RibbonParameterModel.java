package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.client.dataModel.Updater;

public class RibbonParameterModel extends PhysicalControlParameterModel {

	public EnumDataModelEntity<ReturnModes> mode = new EnumDataModelEntity<ReturnModes>(ReturnModes.class,
			ReturnModes.non_return);

	public RibbonParameterModel(ParameterId id) {
		super(id);
		mode.onChange(v -> notifyChanges());
	}

	public enum ReturnModes {
		non_return, return_to_center
	};

	@Override
	public boolean isReturning() {
		return mode.getValue() != ReturnModes.non_return;
	}

	@Override
	public Updater createUpdater(Node c) {
		return new RibbonParameterModelUpdater(c, this);
	}

}
