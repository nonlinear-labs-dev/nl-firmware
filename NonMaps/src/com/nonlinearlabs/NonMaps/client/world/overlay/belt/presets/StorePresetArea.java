package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class StorePresetArea extends OverlayLayout {

	private StoreOptionsMenu menu;
	private StorePreset button;

	protected StorePresetArea(BeltPresetLayout parent) {
		super(parent);

		menu = addChild(new StoreOptionsMenu(this));
		button = addChild(new StorePreset(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		double buttonDim = getButtonDimension();

		menu.doLayout(0, 0, w, buttonDim);
		button.doLayout(0, buttonDim, w, buttonDim);
	}

	public void update(Node settingsNode, Node presetManagerNode) {
		menu.update(settingsNode);
		button.update(settingsNode, presetManagerNode);
	}
}
