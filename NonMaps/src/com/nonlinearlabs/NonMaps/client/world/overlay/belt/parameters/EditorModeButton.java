package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public class EditorModeButton extends SVGImage {

	public EditorModeButton(BeltParameterLayout parent) {
		super(parent, "MC_Edit_Active.svg", "MC_Edit_Enabled.svg");
	}

	@Override
	public BeltParameterLayout getParent() {
		return (BeltParameterLayout) super.getParent();
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		if (isActive())
			getParent().setMode(BeltParameterLayout.Mode.modulateableParameter);
		else
			getParent().setMode(BeltParameterLayout.Mode.paramValue);

		return this;
	}

	@Override
	public boolean isVisible() {
		if (getParent().isOneOf(BeltParameterLayout.Mode.unmodulateableParameter))
			return false;

		return super.isVisible();
	}

	@Override
	public int getSelectedPhase() {
		boolean active = isActive();
		return active ? 0 : 1;
	}

	private boolean isActive() {
		return getParent().isOneOf(BeltParameterLayout.Mode.mcValue, BeltParameterLayout.Mode.mcAmount, BeltParameterLayout.Mode.mcSource,
				BeltParameterLayout.Mode.paramValue, BeltParameterLayout.Mode.mcLower, BeltParameterLayout.Mode.mcUpper);
	}

	@Override
	public Control doubleClick() {
		return this;
	}
}
