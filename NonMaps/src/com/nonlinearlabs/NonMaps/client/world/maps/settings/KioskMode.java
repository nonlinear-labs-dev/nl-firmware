package com.nonlinearlabs.NonMaps.client.world.maps.settings;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.BooleanValues;
import com.nonlinearlabs.NonMaps.client.useCases.SystemSettings;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.NonLinearWorld;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.maps.ContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.NonPosition;

class KioskMode extends Setting {

	private boolean value;

	private class EnabledDisabledContextMenu extends ContextMenu {

		private EnabledDisabledContextMenu(MapsLayout parent) {
			super(parent);

			addChild(new ContextMenuItem(this, "On") {
				@Override
				public Control click(Position eventPoint) {
					setValue(true);
					return super.click(eventPoint);
				}
			});

			addChild(new ContextMenuItem(this, "Off") {
				@Override
				public Control click(Position eventPoint) {
					setValue(false);
					return super.click(eventPoint);
				}
			});
		}
	}

	protected KioskMode(DeveloperSettings parent) {
		super(parent, "Kiosk Mode", "Off");

		Setup.get().systemSettings.kioskMode.onChange(new Function<Setup.BooleanValues, Boolean>() {

			@Override
			public Boolean apply(Setup.BooleanValues t) {
				setCurrentValue(t == Setup.BooleanValues.on);
				return true;
			}
		});
	}

	private void setValue(boolean val) {
		SystemSettings.get().setKioskMode(val ? BooleanValues.on : BooleanValues.off);
	}

	public void setCurrentValue(boolean val) {
		if (value != val) {
			value = val;
			super.setCurrentValue(val ? "On" : "Off");
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}

	@Override
	public ContextMenu createMenu(NonPosition pos) {
		NonLinearWorld world = getWorld();
		return world.setContextMenu(new EnabledDisabledContextMenu(world), pos);
	}

	@Override
	public void setDefault() {
		setValue(false);
	}

}
