package com.nonlinearlabs.NonMaps.client.dataModel;

public class PedalParameter extends PhysicalControlParameter {
	public enum Modes {
		stay, returnToZero, returnToCenter
	};

	public EnumDataModelEntity<Modes> mode = new EnumDataModelEntity<Modes>(Modes.class, Modes.stay);
}
