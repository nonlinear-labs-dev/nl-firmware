package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;

class BankTitle extends Label {

	BankTitle(BankHeader parent) {
		super(parent);
		super.setFontHeightInMM(6);
	}

	@Override
	public BankHeader getParent() {
		return (BankHeader) super.getParent();
	}

	@Override
	public String getDrawText(Context2d ctx) {
		Bank bank = getParent().getParent().getBankInCharge();

		if (bank != null) {
			return bank.getOrderNumber() + " - " + bank.getCurrentName();
		} else {
			return "---";
		}
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		Rect pix = getPixRect();
		pix.fill(ctx, new Gray(51));
		super.draw(ctx, invalidationMask);
	}

	@Override
	public RGB getColorFont() {
		return new Gray(204);
	}
}
