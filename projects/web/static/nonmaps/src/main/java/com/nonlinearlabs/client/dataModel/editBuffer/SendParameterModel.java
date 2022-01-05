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
		super.getHash(crc);
		crc.eat(mode.getValue().ordinal());
		crc.eat(enabled.getBool());
	}

	public boolean isReturning() {
		return mode.getValue() != ReturnMode.None;
	}

    public PhysicalControlParameterModel getSibling() {
		EditBufferModel m = EditBufferModel.get();
		switch(id.getNumber())
        {
            case 398:
                return (PhysicalControlParameterModel)m.getAnyParameter(254);
            case 399:
                return (PhysicalControlParameterModel)m.getAnyParameter(259);
            case 400:
                return (PhysicalControlParameterModel)m.getAnyParameter(264);
            case 401:
                return (PhysicalControlParameterModel)m.getAnyParameter(269);
            case 402:
                return (PhysicalControlParameterModel)m.getAnyParameter(274);
            case 403:
                return (PhysicalControlParameterModel)m.getAnyParameter(279);
            case 404:
                return (PhysicalControlParameterModel)m.getAnyParameter(284);
            case 405:
                return (PhysicalControlParameterModel)m.getAnyParameter(289);
        }
		return null;
    }
}
