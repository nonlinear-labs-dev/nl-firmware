package com.nonlinearlabs.NonMaps.client.dataModel.setup;

import com.nonlinearlabs.NonMaps.client.dataModel.StringDataModelEntity;

public class DeviceInfomation {
	private static DeviceInfomation theInstance = new DeviceInfomation();

	static public DeviceInfomation get() {
		return theInstance;
	}

	public StringDataModelEntity freeDiscSpace = new StringDataModelEntity();
	public StringDataModelEntity softwareVersion = new StringDataModelEntity();
	public StringDataModelEntity rtSoftwareVersion = new StringDataModelEntity();
	public StringDataModelEntity osVersion = new StringDataModelEntity();
	public StringDataModelEntity dateTime = new StringDataModelEntity();
	public StringDataModelEntity dateTimeDisplay = new StringDataModelEntity();
}
