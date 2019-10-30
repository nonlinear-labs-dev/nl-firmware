package com.nonlinearlabs.client.dataModel.editBuffer;

public class PhysicalControlParameterModel extends BasicParameterModel {
	public PhysicalControlParameterModel(int id) {
		super(id);
	}

	@Override
	public boolean isValueChanged() {
		return false;
	}

	@Override
	public boolean isChanged() {
		return false;
	}

	public boolean isReturning() {
		return false;
	}

	public int[] getAssociatedModulationRouters() {
		int[] h = new int[4];

		for (int i = 0; i < 4; i++)
			h[i] = id + i + 1;

		return h;
	}

}
