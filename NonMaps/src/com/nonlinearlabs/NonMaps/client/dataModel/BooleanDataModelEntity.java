package com.nonlinearlabs.NonMaps.client.dataModel;

import com.nonlinearlabs.NonMaps.client.dataModel.Setup.BooleanValues;

public class BooleanDataModelEntity extends EnumDataModelEntity<BooleanValues> {
	public BooleanDataModelEntity() {
		super(BooleanValues.class, BooleanValues.off);
	}
}