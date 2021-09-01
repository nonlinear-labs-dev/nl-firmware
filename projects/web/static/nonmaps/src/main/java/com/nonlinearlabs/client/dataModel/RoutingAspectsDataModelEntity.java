package com.nonlinearlabs.client.dataModel;

public class RoutingAspectsDataModelEntity extends DataModelEntity<RoutingAspectsSetting> {

	public RoutingAspectsDataModelEntity() {
        super(new RoutingAspectsSetting());
	}

	@Override
	public void fromString(String str) {
		setValue(RoutingAspectsSetting.fromString(str));
	}

}