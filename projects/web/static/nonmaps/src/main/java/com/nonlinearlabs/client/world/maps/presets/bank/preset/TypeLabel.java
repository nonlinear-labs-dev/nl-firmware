package com.nonlinearlabs.client.world.maps.presets.bank.preset;

import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.world.maps.Label;
import com.nonlinearlabs.client.world.maps.MapsLayout;

class TypeLabel extends Label {

	public TypeLabel(MapsLayout parent, String text) {
		super(parent, text);
	}

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
		super.doFirstLayoutPass(levelOfDetail);
		setNonSize(getBaseWidth() + getWidthMargin(), getNonPosition().getHeight());
	}

	@Override
	protected TextAlign getTextAlignment() {
		return TextAlign.LEFT;
	}

    public void updateType(SoundType type) {
        switch(type)
        {
            case Single:
                setText("");
                break;
            case Split:
                setText("\uE0F0");
                break;
            case Layer:
                setText("\uE0F3");
                break;
        }
    }

	@Override
	public double getLeftMargin() {
		return 5;
	}

	private double getBaseWidth() {
		return 15;
	}
}
