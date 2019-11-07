package com.nonlinearlabs.client.world.maps.settings;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BooleanValues;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.NonLinearWorld;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.ContextMenu;
import com.nonlinearlabs.client.world.maps.ContextMenuItem;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.NonPosition;

class BenderRampBypassSetting extends Setting {
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

	protected BenderRampBypassSetting(DeveloperSettings parent) {
		super(parent, "Bender Ramp Bypass", "Off");

		SetupModel.get().systemSettings.benderRampBypass.onChange(v -> {
			setCurrentValue(v == BooleanValues.on);
			return true;
		});
	}

	private void setValue(boolean val) {
		NonMaps.get().getServerProxy().setBenderRampBypass(val ? BooleanValues.on : BooleanValues.off);
		setCurrentValue(val);
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