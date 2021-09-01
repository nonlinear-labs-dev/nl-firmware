package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.client.world.overlay.SVGImage;

class PreviousPreset extends SVGImage {

	PreviousPreset(PrevNextPresetButtons parent) {
		super(parent, "Preset_Up_Enabled.svg", "Preset_Up_Active.svg", "Preset_Up_Disabled.svg");
	}

	@Override
	public Control click(Position eventPoint) {
		doPresetSelection();
		return this;
	}

	@Override
	public Control longLeftPress(Position eventPoint) {
		ButtonRepeat.get().start(new Runnable() {

			@Override
			public void run() {
				doPresetSelection();
			}
		});
		return super.longLeftPress(eventPoint);
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		return this;
	}

	@Override
	public void onMouseLost() {
		ButtonRepeat.get().cancel();
		super.onMouseLost();
	}

	private void doPresetSelection() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		pm.selectPreviousPreset();
	}

	@Override
	public int getSelectedPhase() {

		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();

		Bank b = pm.findBank(pm.getSelectedBank());
		if (b != null)
			if (b.getPresetList().getPresetCount() == 0)
				return drawStates.disabled.ordinal();

		if (!pm.canPrev())
			return drawStates.disabled.ordinal();

		return super.getSelectedPhase();
	}
}
