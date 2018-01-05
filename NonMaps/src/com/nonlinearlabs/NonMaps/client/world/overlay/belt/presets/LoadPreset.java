package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

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
				String p = bank.getSelectedPreset();
				if (p != null) {
					Preset preset = bank.findPreset(p);
					if (preset != null) {
						return preset.equals(pm.findLoadedPreset());
					}
				}
			}
		}
		return false;
	}

	public void load() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		pm.loadSelectedPreset();
	}
}
