package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class StoreModeButton extends SVGImage {

	public StoreModeButton(OverlayLayout parent) {
		super(parent, "Select_Enabled.svg", "Select_Active.svg", "Select_Disabled.svg");
	}

	private PresetManager getPresetManager() {
		return NonMaps.get().getNonLinearWorld().getPresetManager();
	}

	@Override
	public int getSelectedPhase() {
		if (getPresetManager().isEmpty()) {
			return drawStates.disabled.ordinal();
		} else if (getPresetManager().isInStoreSelectMode()) {
			return drawStates.active.ordinal();
		} else {
			return drawStates.normal.ordinal();
		}
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		return super.mouseDown(eventPoint);
	}

	@Override
	public Control mouseUp(Position eventPoint) {
		requestLayout();
		return super.mouseUp(eventPoint);
	}

	@Override
	public Control click(Position eventPoint) {
		toggle();
		return this;
	}

	public void update(Node settingsNode, Node presetManagerNode) {

	}

	public void toggle() {
		if (getPresetManager().isInStoreSelectMode()) {
			getPresetManager().endStoreSelectMode();
		} else {
			getPresetManager().startStoreSelectMode();
		}
	}

	public void storeSelectOff() {
		if (getPresetManager().isInStoreSelectMode())
			getPresetManager().endStoreSelectMode();
	}
}
