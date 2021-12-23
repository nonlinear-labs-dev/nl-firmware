package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public class AdvancedBankInformation extends Label {

	public AdvancedBankInformation(OverlayLayout parent) {
		super(parent);
		super.setFontColor(new RGB(120, 120, 120));
		super.setFontHeightInMM(5);
	}

	@Override
	public String getDrawText(Context2d ctx) {
		String bankUUID = NonMaps.theMaps.getNonLinearWorld().getPresetManager().getSelectedBank();
		Bank currentBank = NonMaps.theMaps.getNonLinearWorld().getPresetManager().findBank(bankUUID);
		if (currentBank != null) {
			return "[" + currentBank.getPresetList().getPresetCount() + "]";
		} else {
			return "";
		}
	}

	@Override
	protected String crop(Context2d ctx, Rect pixRect, String text) {
		return text;
	}
}
