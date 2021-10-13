package com.nonlinearlabs.client.dataModel.setup;

import java.util.HashMap;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.DataModelEntityBase;
import com.nonlinearlabs.client.dataModel.Updater;

public class DeviceInfoUpdater extends Updater {
	private final HashMap<String, DataModelEntityBase> xmlNodeNameToDeviceInfo = createSettingMap();

	public DeviceInfoUpdater(Node deviceInformation) {
		super(deviceInformation);
	}

	private HashMap<String, DataModelEntityBase> createSettingMap() {
		HashMap<String, DataModelEntityBase> m = new HashMap<String, DataModelEntityBase>();

		m.put("free-disc-space", DeviceInformation.get().freeDiscSpace);
		m.put("software-version", DeviceInformation.get().softwareVersion);
		m.put("date-time", DeviceInformation.get().dateTime);
		m.put("date-time-display", DeviceInformation.get().dateTimeDisplay);
		m.put("buffer-underruns", DeviceInformation.get().bufferUnderruns);
                m.put("serial-number", DeviceInformation.get().UniqueHardwareID);

		return m;
	}

	public void doUpdate() {
		if (didChange(root)) {
			Node info = root.getFirstChild();

			while (info != null) {
				if (info.getNodeType() == Node.ELEMENT_NODE) {
					String value = getText(info);
					DataModelEntityBase s = findDeviceInfoFromTagName(info.getNodeName());
					if (s != null)
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
