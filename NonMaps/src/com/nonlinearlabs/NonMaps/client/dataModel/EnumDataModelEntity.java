package com.nonlinearlabs.NonMaps.client.dataModel;

public class EnumDataModelEntity<T extends Enum<T>> extends DataModelEntity<T> {
	Class<T> klass;

	public EnumDataModelEntity(Class<T> c, T def) {
		super(def);
		klass = c;
	}

	@Override
	public void fromString(String str) {
		str = str.replace("-", "_");
		setValue(Enum.valueOf(klass, str.toLowerCase()));
	}

}