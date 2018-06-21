package com.nonlinearlabs.NonMaps.client.dataModel;

public class RibbonParameter extends PhysicalControlParameter {
	public enum ReturnModes {
		non_return, return_to_center
	};

	public EnumDataModelEntity<ReturnModes> mode = new EnumDataModelEntity<ReturnModes>(ReturnModes.class, ReturnModes.non_return);
}
