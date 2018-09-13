package com.nonlinearlabs.NonMaps.client.dataModel.editBuffer;

import com.nonlinearlabs.NonMaps.client.dataModel.BooleanDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.StringDataModelEntity;

public class ParameterGroupModel {
	public StringDataModelEntity shortName = new StringDataModelEntity();
	public StringDataModelEntity longName = new StringDataModelEntity();
	public BooleanDataModelEntity locked = new BooleanDataModelEntity();
}
