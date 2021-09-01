package com.nonlinearlabs.client.dataModel.editBuffer;

import com.nonlinearlabs.client.dataModel.BooleanDataModelEntity;
import com.nonlinearlabs.client.dataModel.StringDataModelEntity;

public class ParameterGroupModel {
	public GroupId id;
	public StringDataModelEntity shortName = new StringDataModelEntity();
	public StringDataModelEntity longName = new StringDataModelEntity();
	public BooleanDataModelEntity locked = new BooleanDataModelEntity();
}
