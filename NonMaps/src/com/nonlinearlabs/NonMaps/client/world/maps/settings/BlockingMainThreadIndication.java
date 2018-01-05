package com.nonlinearlabs.NonMaps.client.world.maps.settings;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.world.NonLinearWorld;
import com.nonlinearlabs.NonMaps.client.world.maps.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.maps.NonPosition;
import com.nonlinearlabs.NonMaps.client.world.maps.settings.OnOffContextMenu.Items;
import com.nonlinearlabs.NonMaps.client.world.maps.settings.OnOffContextMenu.Listener;

class BlockingMainThreadIndication extends Setting {

	BlockingMainThreadIndication(DeveloperSettings parent) {
		super(parent, "Indicate blocked UI on Lower Ribbon", "Off");
	}

	@Override
	public ContextMenu createMenu(NonPosition pos) {
		NonLinearWorld world = getWorld();
		return world.setContextMenu(new OnOffContextMenu(new Listener() {

			@Override
			public void setValue(Items item) {
				NonMaps.get().getServerProxy().setSetting("IndicateBlockedUI", item.toString().toLowerCase());
			}
		}, world), pos);
	}

	@Override
	public void setDefault() {
		setCurrentValue(Items.OFF.toString());
	}

	public void update(Node settingsNode) {
		String str = ServerProxy.getChildText(settingsNode, "IndicateBlockedUI", "value");

		if (str != null && !str.isEmpty())
			setCurrentValue(str.toLowerCase().equals("on"));
	}

	public void setCurrentValue(boolean val) {
		super.setCurrentValue(val ? "On" : "Off");
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
	}

}
