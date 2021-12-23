package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.presets.PresetManager;

class PreviousBank extends PrevNextBank {

	PreviousBank(BankHeader parent) {
		super(parent, "Bank_Left_Enabled.svg", "Bank_Left_Active.svg", "Bank_Left_Disabled.svg");
	}

	@Override
	public Control click(Position eventPoint) {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		pm.selectPreviousBank(true);
		return this;
	}

	@Override
	public int getSelectedPhase() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();

		if (!pm.canSelectPreviousBank())
			return drawStates.disabled.ordinal();

		return super.getSelectedPhase();
	}
}
