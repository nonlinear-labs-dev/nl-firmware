package com.nonlinearlabs.NonMaps.client.presenters;

import java.util.LinkedList;
import java.util.function.Function;

import com.google.gwt.core.client.JavaScriptObject;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.BooleanValues;

public class DeviceSettingsProvider {
	public static DeviceSettingsProvider theInstance = new DeviceSettingsProvider();

	public static DeviceSettingsProvider get() {
		return theInstance;
	}

	private LinkedList<Function<DeviceSettings, Boolean>> clients = new LinkedList<Function<DeviceSettings, Boolean>>();
	private DeviceSettings settings = new DeviceSettings();
	private JavaScriptObject editSmoothingTimeStringizer;

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
			settings.editSmoothingTime.sliderPosition = t;

			if (editSmoothingTimeStringizer == null) {
				String body = Setup.get().systemSettings.editSmoothingTime.scaling;

				if (body != null && body.length() > 0) {
					createEditSmoothingTimeStringizer(body);
				}
			}

			if (editSmoothingTimeStringizer != null) {
				settings.editSmoothingTime.displayValue = stringizeEditSmoothingTime(t);
			}

			notifyClients();
			return true;
		});
	}

	protected native void createEditSmoothingTimeStringizer(String body) /*-{
		this.@com.nonlinearlabs.NonMaps.client.presenters.DeviceSettingsProvider::editSmoothingTimeStringizer = new Function(
				"cpValue", "withUnit", body);
	}-*/;

	private native String stringizeEditSmoothingTime(double cpValue) /*-{
		var stringizer = this.@com.nonlinearlabs.NonMaps.client.presenters.DeviceSettingsProvider::editSmoothingTimeStringizer;
		var scaledText = stringizer(cpValue, true);
		return scaledText;
	}-*/;

	protected void notifyClients() {
		clients.removeIf(listener -> !listener.apply(settings));
	}

	public void register(Function<DeviceSettings, Boolean> cb) {
		clients.add(cb);
	}
}
