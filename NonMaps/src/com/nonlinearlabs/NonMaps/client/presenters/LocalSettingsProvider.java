package com.nonlinearlabs.NonMaps.client.presenters;

import java.util.LinkedList;
import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.BooleanValues;

public class LocalSettingsProvider {
	public static LocalSettingsProvider theInstance = new LocalSettingsProvider();

	public static LocalSettingsProvider get() {
		return theInstance;
	}

	private LinkedList<Function<LocalSettings, Boolean>> clients = new LinkedList<Function<LocalSettings, Boolean>>();
	private LocalSettings settings = new LocalSettings();

	private LocalSettingsProvider() {
		com.nonlinearlabs.NonMaps.client.dataModel.Setup.LocalSettings s = Setup.get().localSettings;

		s.selectionAutoScroll.onChange(t -> {
			settings.selectionAutoScroll.selected = t.ordinal();
			notifyClients();
			return true;
		});

		s.editParameter.onChange(t -> {
			settings.editParameter.selected = t.ordinal();
			notifyClients();
			return true;
		});

		s.displayScaling.onChange(t -> {
			settings.displayScalingFactor.selected = t.ordinal();
			notifyClients();
			return true;
		});

		s.stripeBrightness.onChange(t -> {
			settings.stripeBrightness.selected = t.ordinal();
			notifyClients();
			return true;
		});

		s.contextMenus.onChange(t -> {
			settings.contextMenus.value = t == BooleanValues.on;
			notifyClients();
			return true;
		});

		s.presetDragDrop.onChange(t -> {
			settings.presetDragDrop.value = t == BooleanValues.on;
			notifyClients();
			return true;
		});

		s.bitmapCache.onChange(t -> {
			settings.bitmapCache.value = t == BooleanValues.on;
			notifyClients();
			return true;
		});

		s.showDeveloperOptions.onChange(t -> {
			settings.showDeveloperOptions.value = t == BooleanValues.on;
			notifyClients();
			return true;
		});
	}

	protected void notifyClients() {
		clients.removeIf(listener -> !listener.apply(settings));
	}

	public void register(Function<LocalSettings, Boolean> cb) {
		clients.add(cb);
		cb.apply(settings);
	}
}
