package com.nonlinearlabs.client.dataModel.editBuffer;

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
		boolean globalEnable = model.globalLocalEnable;
		boolean hwEnabled = model.isLocalEnabled(this);
		return hwEnabled && globalEnable;
	}

}
