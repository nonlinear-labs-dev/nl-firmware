package com.nonlinearlabs.NonMaps.client.world.maps.settings;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
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
	}

	private void setValue(boolean val) {
		getNonMaps().getServerProxy().setSetting("KioskMode", val ? "on" : "off");
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

	void update(Node settingsNode) {
		String str = ServerProxy.getChildText(settingsNode, "KioskMode", "value");

		if (str != null && !str.isEmpty())
			setCurrentValue(str.toLowerCase().equals("on"));
	}

	@Override
	public void setDefault() {
		setValue(false);
	}

}
