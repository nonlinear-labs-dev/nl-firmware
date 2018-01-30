package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.core.shared.GWT;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public class StoreModeButton extends SVGImage {
	
	public StoreModeButton(OverlayLayout parent) {
		super(parent, "StoreMode_Enabled.svg", "StoreMode_Active.svg", "StoreMode_Disabled.svg");
		((StorePresetArea)getParent()).getCommitButton().setActive(false);
	}

	private PresetManager getPresetManager() {
		return NonMaps.get().getNonLinearWorld().getPresetManager();
	}
	
	@Override
	public int getSelectedPhase() {
		if(getPresetManager().isInStoreMode()) {
			return drawStates.active.ordinal();
		}
		
		return drawStates.normal.ordinal();
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
		
		GWT.log("Click on storeMode!");
		
		if(getPresetManager().isInStoreMode()) {
			getPresetManager().endStoreMode();
			((StorePresetArea)getParent()).getCommitButton().setActive(false);
			super.selectPhase(0);
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		} else {
			getPresetManager().startStoreMode();
			((StorePresetArea)getParent()).getCommitButton().setActive(true);
			super.selectPhase(2);
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}		
		
		requestLayout();

		return this;
	}

	public void update(Node settingsNode, Node presetManagerNode) {
		
	}
}
