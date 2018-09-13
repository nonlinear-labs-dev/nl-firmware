package com.nonlinearlabs.NonMaps.client.presenters;

import java.util.LinkedList;
import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.PedalParameter;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.BooleanValues;
import com.nonlinearlabs.NonMaps.client.presenters.DeviceSettings.Pedal;

public class DeviceSettingsProvider {
	public static DeviceSettingsProvider theInstance = new DeviceSettingsProvider();

	public static DeviceSettingsProvider get() {
		return theInstance;
	}

	private LinkedList<Function<DeviceSettings, Boolean>> clients = new LinkedList<Function<DeviceSettings, Boolean>>();
	private DeviceSettings settings = new DeviceSettings();

	private DeviceSettingsProvider() {
		Setup.get().systemSettings.velocityCurve.onChange(t -> {
			settings.velocityCurve.selected = t.ordinal();
			notifyClients();
			return true;
		});

		Setup.get().systemSettings.aftertouchCurve.onChange(t -> {
			settings.aftertouchCurve.selected = t.ordinal();
			notifyClients();
			return true;
		});

		Setup.get().systemSettings.benderCurve.onChange(t -> {
			settings.benderCurve.selected = t.ordinal();
			notifyClients();
			return true;
		});

		Setup.get().systemSettings.pedal1Type.onChange(t -> {
			settings.pedal1.selected = t.ordinal();
			notifyClients();
			return true;
		});

		Setup.get().systemSettings.pedal2Type.onChange(t -> {
			settings.pedal2.selected = t.ordinal();
			notifyClients();
			return true;
		});

		Setup.get().systemSettings.pedal3Type.onChange(t -> {
			settings.pedal3.selected = t.ordinal();
			notifyClients();
			return true;
		});

		Setup.get().systemSettings.pedal4Type.onChange(t -> {
			settings.pedal4.selected = t.ordinal();
			notifyClients();
			return true;
		});

		Setup.get().systemSettings.presetGlitchSuppression.onChange(t -> {
			settings.presetGlitchSurpession.value = (t == BooleanValues.on);
			notifyClients();
			return true;
		});

		Setup.get().systemSettings.editSmoothingTime.onChange(t -> {
			double v = t.value.getValue();
			settings.editSmoothingTime.sliderPosition = v;
			settings.editSmoothingTime.displayValue = Stringizers.get().stringize(
					Setup.get().systemSettings.editSmoothingTime.metaData.scaling.getValue(), v);
			notifyClients();
			return true;
		});

		Setup.get().systemSettings.deviceName.onChange(t -> {
			settings.deviceName = t;
			notifyClients();
			return true;
		});

		connectToPedal(254, settings.pedal1);
		connectToPedal(259, settings.pedal2);
		connectToPedal(264, settings.pedal3);
		connectToPedal(269, settings.pedal4);
	}

	public void connectToPedal(int id, Pedal target) {
		PedalParameter srcPedal = (PedalParameter) EditBufferModel.get().findParameter(id);
		srcPedal.value.onChange(t -> {
			double v = t.value.getValue();
			target.displayValue = Stringizers.get().stringize(srcPedal.value.metaData.scaling.getValue(), v);
			target.sliderPosition = v;
			notifyClients();
			return true;
		});
	}

	protected void notifyClients() {
		clients.removeIf(listener -> !listener.apply(settings));
	}

	public void register(Function<DeviceSettings, Boolean> cb) {
		clients.add(cb);
		cb.apply(settings);
	}
}
