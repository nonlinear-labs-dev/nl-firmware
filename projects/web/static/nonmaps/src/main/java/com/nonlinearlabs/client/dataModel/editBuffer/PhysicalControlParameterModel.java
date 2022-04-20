package com.nonlinearlabs.client.dataModel.editBuffer;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;

public class PhysicalControlParameterModel extends BasicParameterModel {
	public PhysicalControlParameterModel(ParameterId id) {
		super(id);
	}

	public boolean isReturning() {
		return false;
	}

	public ParameterId[] getAssociatedModulationRouters() {
		return ParameterFactory.getModulationRouters(id);
	}

	public boolean isLocalEnabled()
	{
		SetupModel model = SetupModel.get();
		boolean globalEnable = model.systemSettings.localEnabled.getBool();
		boolean hwEnabled = model.systemSettings.routingAspects.getValue().isLocalEnabled(this);
		return hwEnabled && globalEnable;
	}

	public int getSibling()
    {
        switch(id.getNumber())
        {
            case 254:
                return 398;
            case 259:
                return 399;
            case 264:
                return 400;
            case 269:
                return 401;
            case 274:
                return 402;
            case 279:
                return 403;
            case 284:
                return 404;
            case 289:
                return 405;
        }
        return -1;
    }

}
