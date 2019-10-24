package com.nonlinearlabs.client.dataModel.editBuffer;

import com.nonlinearlabs.client.dataModel.EnumDataModelEntity;

public class PedalParameterModel extends PhysicalControlParameterModel {
	public PedalParameterModel(int id) {
		super(id);
	}

	public enum Modes {
		stay, returnToZero, returnToCenter
	};

	@Override
	public boolean isReturning() {
		return mode.getValue() != Modes.stay;
	}

	public EnumDataModelEntity<Modes> mode = new EnumDataModelEntity<Modes>(Modes.class, Modes.stay);
}
