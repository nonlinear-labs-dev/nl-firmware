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

}
