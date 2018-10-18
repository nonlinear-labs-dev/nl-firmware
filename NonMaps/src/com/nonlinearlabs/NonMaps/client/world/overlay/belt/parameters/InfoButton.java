package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.ParameterInfoDialog;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public class InfoButton extends SVGImage {

	public InfoButton(BeltParameterLayout parent) {
		super(parent, "Menu_Info_Enabled_S.svg", "Menu_Info_Active_S.svg");
	}

	@Override
	public BeltParameterLayout getParent() {
		return (BeltParameterLayout) super.getParent();
	}

	@Override
	public Control click(Position eventPoint) {
		ParameterInfoDialog.toggle();
		return this;
	}

	@Override
	public int getSelectedPhase() {
		return ParameterInfoDialog.isShown() ? 1 : 0;
	}
}
