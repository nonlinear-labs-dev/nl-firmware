package com.nonlinearlabs.client.dataModel.editBuffer;

import com.nonlinearlabs.client.dataModel.EnumDataModelEntity;

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

	public EnumDataModelEntity<ReturnModes> mode = new EnumDataModelEntity<ReturnModes>(ReturnModes.class,
			ReturnModes.non_return);

}
