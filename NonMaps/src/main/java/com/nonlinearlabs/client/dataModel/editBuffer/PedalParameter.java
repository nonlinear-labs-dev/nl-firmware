package com.nonlinearlabs.client.dataModel.editBuffer;

import com.nonlinearlabs.client.dataModel.EnumDataModelEntity;

public class PedalParameter extends PhysicalControlParameter {
	public PedalParameter(int id) {
		super(id);
	}

	public enum Modes {
		stay, returnToZero, returnToCenter
	};

	public EnumDataModelEntity<Modes> mode = new EnumDataModelEntity<Modes>(Modes.class, Modes.stay);
}
