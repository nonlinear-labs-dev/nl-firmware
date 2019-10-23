package com.nonlinearlabs.client.dataModel.setup;

import com.nonlinearlabs.client.dataModel.StringDataModelEntity;

public class DeviceInformation {
	private static DeviceInformation theInstance = new DeviceInformation();

	static public DeviceInformation get() {
		return theInstance;
	}

	public StringDataModelEntity freeDiscSpace = new StringDataModelEntity();
	public StringDataModelEntity softwareVersion = new StringDataModelEntity();
	public StringDataModelEntity rtSoftwareVersion = new StringDataModelEntity();
	public StringDataModelEntity osVersion = new StringDataModelEntity();
	public StringDataModelEntity dateTime = new StringDataModelEntity();
	public StringDataModelEntity dateTimeDisplay = new StringDataModelEntity();
}
