package com.nonlinearlabs.NonMaps.client.presenters;

import java.util.LinkedList;
import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.setup.DeviceInfomation;

public class DeviceInformationProvider {
	public static DeviceInformationProvider theInstance = new DeviceInformationProvider();

	public static DeviceInformationProvider get() {
		return theInstance;
	}

	private LinkedList<Function<DeviceInformation, Boolean>> clients = new LinkedList<Function<DeviceInformation, Boolean>>();
	private DeviceInformation info = new DeviceInformation();

	private DeviceInformationProvider() {
		DeviceInfomation.get().freeDiscSpace.onChange(v -> {
			info.freeDiscSpace = v;
			notifyClients();
			return true;
		});

		DeviceInfomation.get().softwareVersion.onChange(v -> {
			info.uiVersion = v;
			notifyClients();
			return true;
		});

		DeviceInfomation.get().rtSoftwareVersion.onChange(v -> {
			info.rtVersion = v;
			notifyClients();
			return true;
		});

		DeviceInfomation.get().osVersion.onChange(v -> {
			info.osVersion = v;
			notifyClients();
			return true;
		});

		DeviceInfomation.get().dateTimeDisplay.onChange(v -> {
			info.dateTime = v;
			notifyClients();
			return true;
		});
	}

	protected void notifyClients() {
		clients.removeIf(listener -> !listener.apply(info));
	}

	public void register(Function<DeviceInformation, Boolean> cb) {
		clients.add(cb);
		cb.apply(info);
	}
}
