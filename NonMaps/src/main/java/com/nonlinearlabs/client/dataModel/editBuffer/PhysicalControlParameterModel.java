package com.nonlinearlabs.client.dataModel.editBuffer;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;

public class PhysicalControlParameterModel extends BasicParameterModel {
	public PhysicalControlParameterModel(ParameterId id) {
		super(id);
	}

	public boolean isReturning() {
		return false;
	}

	public ParameterId[] getAssociatedModulationRouters() {
		ParameterId[] h = new ParameterId[8];

		for (int i = 0; i < 4; i++)
		{
			h[i] = new ParameterId(id.getNumber() + i + 1, VoiceGroup.I);
			h[i + 4] = new ParameterId(id.getNumber() + i + 1, VoiceGroup.II);
		}

		return h;
	}

}
