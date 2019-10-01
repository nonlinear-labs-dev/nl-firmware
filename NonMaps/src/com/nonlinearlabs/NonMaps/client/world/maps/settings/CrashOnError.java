package com.nonlinearlabs.NonMaps.client.world.maps.settings;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.BooleanValues;
import com.nonlinearlabs.NonMaps.client.useCases.SystemSettings;
import com.nonlinearlabs.NonMaps.client.world.NonLinearWorld;
import com.nonlinearlabs.NonMaps.client.world.maps.IContextMenu;
import com.nonlinearlabs.NonMaps.client.world.maps.NonPosition;
import com.nonlinearlabs.NonMaps.client.world.maps.settings.OnOffContextMenu.Items;
import com.nonlinearlabs.NonMaps.client.world.maps.settings.OnOffContextMenu.Listener;

public class CrashOnError extends Setting {

	protected CrashOnError(DeveloperSettings parent) {
		super(parent, "Crash on Error", "Off");

		Setup.get().systemSettings.crashOnError.onChange(new Function<Setup.BooleanValues, Boolean>() {

			@Override
			public Boolean apply(Setup.BooleanValues t) {
				setCurrentValue(t == Setup.BooleanValues.on);
				return true;
			}
		});
	}

	@Override
	public IContextMenu createMenu(NonPosition pos) {
		NonLinearWorld world = getWorld();
		return world.setContextMenu(new OnOffContextMenu(new Listener() {

			@Override
			public void setValue(Items item) {
				SystemSettings.get().setCrashOnError(item == Items.ON ? BooleanValues.on : BooleanValues.off);
			}
		}, world), pos);
	}

	@Override
	public void setDefault() {
		setCurrentValue(Items.OFF.toString());
	}

	public void setCurrentValue(boolean val) {
		super.setCurrentValue(val ? "On" : "Off");
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
	}
}
