package com.nonlinearlabs.client.dataModel;

public class StringDataModelEntity extends DataModelEntity<String> {
	public StringDataModelEntity() {
		super("");
	}

	public StringDataModelEntity(String s) {
		super(s);
	}

	@Override
	public void fromString(String str) {
		setValue(str);
	}
}