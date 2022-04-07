package com.nonlinearlabs.client.dataModel.setup;

import com.nonlinearlabs.client.dataModel.StringDataModelEntity;

public class DeviceInformation {
	private static DeviceInformation theInstance = new DeviceInformation();

	static public DeviceInformation get() {
		return theInstance;
	}

	public StringDataModelEntity freeDiscSpace = new StringDataModelEntity();
	public StringDataModelEntity softwareVersion = new StringDataModelEntity();
	public StringDataModelEntity dateTime = new StringDataModelEntity();
	public StringDataModelEntity dateTimeDisplay = new StringDataModelEntity();
	public StringDataModelEntity branch = new StringDataModelEntity();
	public StringDataModelEntity commits = new StringDataModelEntity();
	public StringDataModelEntity head = new StringDataModelEntity();
	public StringDataModelEntity date = new StringDataModelEntity();
	public StringDataModelEntity usedRam = new StringDataModelEntity();
	public StringDataModelEntity totalRam = new StringDataModelEntity();
	public StringDataModelEntity bufferUnderruns = new StringDataModelEntity("0");
	public StringDataModelEntity uniqueHardwareID = new StringDataModelEntity("0");
	public StringDataModelEntity uiRTVersion = new StringDataModelEntity();
}
