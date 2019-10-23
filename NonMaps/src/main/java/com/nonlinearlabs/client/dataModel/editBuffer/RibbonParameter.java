package com.nonlinearlabs.client.dataModel.editBuffer;

import com.nonlinearlabs.client.dataModel.EnumDataModelEntity;

public class RibbonParameter extends PhysicalControlParameter {
	public RibbonParameter(int id) {
		super(id);
	}

	public enum ReturnModes {
		non_return, return_to_center
	};

	public EnumDataModelEntity<ReturnModes> mode = new EnumDataModelEntity<ReturnModes>(ReturnModes.class,
			ReturnModes.non_return);
}
