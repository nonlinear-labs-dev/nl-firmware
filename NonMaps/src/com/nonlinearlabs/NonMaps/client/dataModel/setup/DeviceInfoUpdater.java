package com.nonlinearlabs.NonMaps.client.dataModel.setup;

import java.util.HashMap;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.dataModel.DataModelEntityBase;
import com.nonlinearlabs.NonMaps.client.dataModel.Updater;

public class DeviceInfoUpdater extends Updater {
	private Node deviceInformation;
	private final HashMap<String, DataModelEntityBase> xmlNodeNameToDeviceInfo = createSettingMap();

	public DeviceInfoUpdater(Node deviceInformation) {
		this.deviceInformation = deviceInformation;
	}

	private HashMap<String, DataModelEntityBase> createSettingMap() {
		HashMap<String, DataModelEntityBase> m = new HashMap<String, DataModelEntityBase>();

		m.put("free-disc-space", DeviceInfomation.get().freeDiscSpace);
		m.put("software-version", DeviceInfomation.get().softwareVersion);
		m.put("rt-software-version", DeviceInfomation.get().rtSoftwareVersion);
		m.put("os-version", DeviceInfomation.get().osVersion);
		m.put("date-time", DeviceInfomation.get().dateTime);
		m.put("date-time-display", DeviceInfomation.get().dateTimeDisplay);

		return m;
	}

	public void doUpdate() {
		if (didChange(deviceInformation)) {
			Node info = deviceInformation.getFirstChild();

			while (info != null) {
				if (info.getNodeType() == Node.ELEMENT_NODE) {
					String value = getText(info);
					DataModelEntityBase s = findDeviceInfoFromTagName(info.getNodeName());
					s.fromString(value);

				}
				info = info.getNextSibling();
			}
		}
	}

	private DataModelEntityBase findDeviceInfoFromTagName(String nodeName) {
		return xmlNodeNameToDeviceInfo.get(nodeName);
	}
}
