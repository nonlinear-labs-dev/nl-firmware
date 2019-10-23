package com.nonlinearlabs.client.world.maps.settings;

import java.util.function.Function;

import com.nonlinearlabs.client.dataModel.setup.Setup;
import com.nonlinearlabs.client.dataModel.setup.Setup.BooleanValues;
import com.nonlinearlabs.client.useCases.SystemSettings;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.NonLinearWorld;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.ContextMenu;
import com.nonlinearlabs.client.world.maps.ContextMenuItem;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.NonPosition;

class SendPresetAsLPCWriteFallback extends Setting {

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

	SendPresetAsLPCWriteFallback(DeveloperSettings parent) {
		super(parent, "LPC write fallback", "Off");

		Setup.get().systemSettings.sendPresetAsLPCFallback.onChange(new Function<Setup.BooleanValues, Boolean>() {

			@Override
			public Boolean apply(BooleanValues t) {
				setCurrentValue(t == BooleanValues.on);
				return true;
			}
		});
	}

	private void setValue(boolean val) {
		SystemSettings.get().setSendPresetAsLPCWriteFallback(val ? BooleanValues.on : BooleanValues.off);
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
