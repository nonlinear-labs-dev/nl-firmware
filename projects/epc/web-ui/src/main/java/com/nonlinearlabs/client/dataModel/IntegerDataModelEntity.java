package com.nonlinearlabs.client.dataModel;

public class IntegerDataModelEntity extends DataModelEntity<Integer> {

	public IntegerDataModelEntity() {
		super(0);
	}

	@Override
	public void fromString(String str) {
		setValue(Integer.parseInt(str));
	}
}