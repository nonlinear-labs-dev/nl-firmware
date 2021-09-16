package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.presets.PresetManager;

class NextBank extends PrevNextBank {

	NextBank(BankHeader parent) {
		super(parent, "Bank_Right_Enabled.svg", "Bank_Right_Active.svg", "Bank_Right_Disabled.svg");
	}

	@Override
	public Control click(Position eventPoint) {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		pm.selectNextBank(true);
		return this;
	}

	@Override
	public int getSelectedPhase() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();

		if (!pm.canSelectNextBank())
			return drawStates.disabled.ordinal();

		return super.getSelectedPhase();
	}
}
