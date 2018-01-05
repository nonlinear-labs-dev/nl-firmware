package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;

class PreviousBank extends PrevNextBank {

	PreviousBank(BankHeader parent) {
		super(parent, "Bank_Left.svg", "Bank_Left_Disabled.svg");
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
			return 1;

		return super.getSelectedPhase();
	}
}
