package com.nonlinearlabs.NonMaps.client.dataModel;

public class ValueDataModelEntity extends DataModelEntity<Double> {
	public double defaultValue = 0.0;
	public String scaling = "";
	public int coarseDenominator = 0;
	public int fineDenominator = 0;
	public boolean bipolar = false;

	public ValueDataModelEntity() {
		super(0.0);
	}

	@Override
	public void fromString(String str) {
		setValue(Double.parseDouble(str));
	}
}