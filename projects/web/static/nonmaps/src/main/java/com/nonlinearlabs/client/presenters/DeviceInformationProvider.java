package com.nonlinearlabs.client.presenters;

import java.util.LinkedList;
import java.util.function.Function;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BooleanValues;

public class DeviceInformationProvider {
	public static DeviceInformationProvider theInstance = new DeviceInformationProvider();

	public static DeviceInformationProvider get() {
		return theInstance;
	}

	private LinkedList<Function<DeviceInformation, Boolean>> clients = new LinkedList<Function<DeviceInformation, Boolean>>();
	private DeviceInformation info = new DeviceInformation();

	private DeviceInformationProvider() {
		com.nonlinearlabs.client.dataModel.setup.DeviceInformation.get().freeDiscSpace.onChange(v -> {
			info.freeDiscSpace = v;
			notifyClients();
			return true;
		});

		com.nonlinearlabs.client.dataModel.setup.DeviceInformation.get().softwareVersion.onChange(v -> {
			info.uiVersion = v;
			notifyClients();
			return true;
		});

		com.nonlinearlabs.client.dataModel.setup.DeviceInformation.get().dateTimeDisplay.onChange(v -> {
			info.dateTime = v;
			notifyClients();
			return true;
		});

		com.nonlinearlabs.client.dataModel.setup.DeviceInformation.get().branch.onChange(v -> {
			info.branch = v;
			notifyClients();
			return true;
		});

		com.nonlinearlabs.client.dataModel.setup.DeviceInformation.get().head.onChange(v -> {
			info.head = v;
			notifyClients();
			return true;
		});

		com.nonlinearlabs.client.dataModel.setup.DeviceInformation.get().commits.onChange(v -> {
			info.commits = v;
			notifyClients();
			return true;
		});

		com.nonlinearlabs.client.dataModel.setup.DeviceInformation.get().date.onChange(v -> {
			info.commitDate = v;
			notifyClients();
			return true;
		});

		com.nonlinearlabs.client.dataModel.setup.DeviceInformation.get().totalRam.onChange(v -> {
			info.totalRam = v;
			notifyClients();
			return true;
		});

		com.nonlinearlabs.client.dataModel.setup.DeviceInformation.get().usedRam.onChange(v -> {
			info.usedRam = v;
			notifyClients();
			return true;
		});

		com.nonlinearlabs.client.dataModel.setup.DeviceInformation.get().bufferUnderruns.onChange(v -> {
			info.bufferUnderruns = v;
			notifyClients();
			return true;
		});

		com.nonlinearlabs.client.dataModel.setup.DeviceInformation.get().uniqueHardwareID.onChange(v -> {
			info.uniqueHardwareID = v;
			notifyClients();
			return true;
		});

		com.nonlinearlabs.client.dataModel.setup.DeviceInformation.get().uiRTVersion.onChange(v -> {
			info.rtVersion = v;
			notifyClients();
			return true;
		});

		com.nonlinearlabs.client.dataModel.setup.DeviceInformation.get().aftertouchCalibrated.onChange(v -> {
			info.isAftertouchCalibrated = v.equals(BooleanValues.on);
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
