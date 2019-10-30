package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.client.dataModel.Updater;

public class RibbonParameterModel extends PhysicalControlParameterModel {
	public RibbonParameterModel(int id) {
		super(id);
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
		return new RibbonParameterUpdater(c, this);
	}

	public EnumDataModelEntity<ReturnModes> mode = new EnumDataModelEntity<ReturnModes>(ReturnModes.class,
			ReturnModes.non_return);

}
