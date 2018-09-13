package com.nonlinearlabs.NonMaps.client.dataModel.editBuffer;

import com.nonlinearlabs.NonMaps.client.dataModel.EnumDataModelEntity;

public class PedalParameter extends PhysicalControlParameter {
	public enum Modes {
		stay, returnToZero, returnToCenter
	};

	public EnumDataModelEntity<Modes> mode = new EnumDataModelEntity<Modes>(Modes.class, Modes.stay);
}
