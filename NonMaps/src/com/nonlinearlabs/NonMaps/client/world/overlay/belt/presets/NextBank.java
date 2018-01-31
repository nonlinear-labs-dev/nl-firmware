package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;

class NextBank extends PrevNextBank {

	NextBank(BankHeader parent) {
		super(parent, "Bank_Right.svg", "Bank_Right_Active.svg", "Bank_Right_Disabled.svg");
	}

	@Override
	public Control click(Position eventPoint) {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		if(pm.isInStoreMode() == false)
			pm.selectNextBank(true);
		else
			pm.getStoreMode().selecteNextBank();
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
