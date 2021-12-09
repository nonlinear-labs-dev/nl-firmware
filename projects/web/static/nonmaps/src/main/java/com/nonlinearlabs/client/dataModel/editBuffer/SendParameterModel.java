package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.Checksum;
import com.nonlinearlabs.client.dataModel.BooleanDataModelEntity;
import com.nonlinearlabs.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.client.dataModel.Updater;
import com.nonlinearlabs.client.world.maps.parameters.PhysicalControlParameter.ReturnMode;

public class SendParameterModel extends BasicParameterModel {

	public BooleanDataModelEntity enabled = new BooleanDataModelEntity();
	public EnumDataModelEntity<ReturnMode> mode = new EnumDataModelEntity<ReturnMode>(ReturnMode.class, ReturnMode.None);

	public SendParameterModel(ParameterId id) {
		super(id);
		enabled.onChange(v -> 
		{
			notifyChanges();
			return true;
		});

		mode.onChange(v -> {
			notifyChanges();
			return true;
		});
	}

	@Override
	public Updater createUpdater(Node c) {
		return new SendParameterModelUpdater(c, this);
	}
	
	@Override
	public void getHash(Checksum crc) {
		crc.eat(mode.getValue().ordinal());
		crc.eat(enabled.getBool());
	}

	public boolean isReturning() {
		return mode.getValue() != ReturnMode.None;
	}
}
