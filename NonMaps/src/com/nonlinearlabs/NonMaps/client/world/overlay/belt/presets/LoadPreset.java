package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

class LoadPreset extends SVGImage {

	boolean isEnabled = true;

	LoadPreset(OverlayLayout parent) {
		super(parent, "Load_Enabled.svg", "Load_Disabled.svg");
	}

	@Override
	public Control click(Position eventPoint) {
		if(NonMaps.get().getNonLinearWorld().getPresetManager().isInStoreSelectMode())
			return this;
		
		if (isEnabled) {
			load();
			return this;
		}
		return super.click(eventPoint);
	}

	public void update(Node editBufferNode) {	
		if (editBufferNode != null) {
			if (isSelectedPresetLoaded()) {
				String isModifiedStr = editBufferNode.getAttributes().getNamedItem("is-modified").getNodeValue();
				boolean modified = isModifiedStr != null && isModifiedStr.equals("1");
				setEnabled(modified);
				return;
			}
			setEnabled(true);
		}
	}

	private void setEnabled(boolean b) {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		if(pm.isInStoreSelectMode()) {
			isEnabled = false;
			selectPhase(isEnabled ? 0 : 1);
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
			requestLayout();
		}
		
		if (b != isEnabled) {
			isEnabled = b;
			selectPhase(isEnabled ? 0 : 1);
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}

	protected boolean isSelectedPresetLoaded() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		String b = pm.getSelectedBank();
		if (b != null) {
			Bank bank = pm.findBank(b);
			if (bank != null) {
				String p = bank.getPresetList().getSelectedPreset();
				if (p != null) {
					Preset preset = bank.getPresetList().findPreset(p);
					if (preset != null) {
						return preset.equals(pm.findLoadedPreset());
					}
				}
			}
		}
		return false;
	}
	
	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		setEnabled(!pm.isInStoreSelectMode());
		requestLayout();
		super.draw(ctx, invalidationMask);
	}

	public void load() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		pm.loadSelectedPreset();
	}
}
