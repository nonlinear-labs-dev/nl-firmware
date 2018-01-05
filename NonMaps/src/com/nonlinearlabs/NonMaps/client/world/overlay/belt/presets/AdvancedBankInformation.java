package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class AdvancedBankInformation extends Label {

	public AdvancedBankInformation(OverlayLayout parent) {
		super(parent);
		super.setFontColor(new RGB(120,120,120));
		super.setFontHeightInPx(20);
	}

	@Override
	public String getDrawText(Context2d ctx) {
		String bankUUID = NonMaps.theMaps.getNonLinearWorld().getPresetManager().getSelectedBank();
		Bank currentBank = NonMaps.theMaps.getNonLinearWorld().getPresetManager().findBank(bankUUID);
		if (currentBank != null) {
			return "[" + currentBank.getPresetCount() + "]";
		} else {
			return "";
		}
	}
}
