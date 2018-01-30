package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class StorePresetArea extends OverlayLayout {

	private StoreOptionsMenu menu;
	private StoreModeButton m_modeButton;
	private StorePreset m_commitButton;

	protected StorePresetArea(BeltPresetLayout parent) {
		super(parent);

		m_commitButton = addChild(new StorePreset(this));
		menu = addChild(new StoreOptionsMenu(this));
		m_modeButton = addChild(new StoreModeButton(this));

	}
	
	public StorePreset getCommitButton() {
		return m_commitButton;
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		double buttonDim = getButtonDimension();

		menu.doLayout(0, 0, w, buttonDim);
		m_commitButton.doLayout(buttonDim, buttonDim, w, buttonDim);
		m_modeButton.doLayout(-buttonDim/2, buttonDim, w, buttonDim);
	}

	public void update(Node settingsNode, Node presetManagerNode) {
		menu.update(settingsNode);
		m_commitButton.update(settingsNode, presetManagerNode);
		m_modeButton.update(settingsNode, presetManagerNode);
	}
}
