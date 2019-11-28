package com.nonlinearlabs.client.dataModel;

import com.nonlinearlabs.client.dataModel.setup.SetupModel.BooleanValues;

public class BooleanDataModelEntity extends EnumDataModelEntity<BooleanValues> {
	public BooleanDataModelEntity() {
		super(BooleanValues.class, BooleanValues.off);
	}

	public BooleanDataModelEntity(BooleanValues v) {
		super(BooleanValues.class, v);
	}

	public void toggle() {
		if (getValue() == BooleanValues.on)
			setValue(BooleanValues.off);
		else
			setValue(BooleanValues.on);
	}

	public boolean isTrue() {
		return getValue() == BooleanValues.on;
	}

	public boolean isFalse() {
		return getValue() == BooleanValues.off;
	}

	public void setValue(boolean v) {
		setValue(v ? BooleanValues.on : BooleanValues.off);
	}

	public boolean getBool() {
		return isTrue();
	}
}
