package com.nonlinearlabs.client.dataModel;

public class DoubleDataModelEntity extends DataModelEntity<Double> {

	public DoubleDataModelEntity() {
		super(0.0);
	}

	@Override
	public void fromString(String str) {
		setValue(Double.parseDouble(str));
	}

}