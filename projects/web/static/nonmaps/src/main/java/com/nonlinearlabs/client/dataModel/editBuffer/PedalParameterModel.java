package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.client.dataModel.Updater;

public class PedalParameterModel extends PhysicalControlParameterModel {

	public EnumDataModelEntity<Modes> mode = new EnumDataModelEntity<Modes>(Modes.class, Modes.stay);

	public PedalParameterModel(ParameterId id) {
		super(id);
		mode.onChange(v -> notifyChanges());
	}

	public enum Modes {
		stay, returnToZero, returnToCenter
	};

	@Override
	public boolean isReturning() {
		return mode.getValue() != Modes.stay;
	}

	@Override
	public Updater createUpdater(Node c) {
		return new PedalParameterModelUpdater(c, this);
	}

}
