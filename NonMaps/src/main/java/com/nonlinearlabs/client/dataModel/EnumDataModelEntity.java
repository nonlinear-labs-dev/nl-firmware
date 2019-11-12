package com.nonlinearlabs.client.dataModel;

import com.nonlinearlabs.client.Tracer;

public class EnumDataModelEntity<T extends Enum<T>> extends DataModelEntity<T> {
	Class<T> klass;

	public EnumDataModelEntity(Class<T> c, T def) {
		super(def);
		klass = c;
	}

	@Override
	public void fromString(String str) {
		str = str.replace("-", "_");
		str = str.replace(" ", "_");
		try {
			setValue(Enum.valueOf(klass, str.toLowerCase()));
		} catch (Exception e) {
			Tracer.addToLog("could not convert string " + str + " to enum value.");
		}
	}

	public void fromInt(int i) {
		for (T v : klass.getEnumConstants()) {
			if (v.ordinal() == i) {
				setValue(v);
				return;
			}
		}
	}
}