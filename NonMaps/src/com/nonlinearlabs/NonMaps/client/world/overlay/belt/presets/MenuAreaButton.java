package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.NonLinearWorld;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public abstract class MenuAreaButton extends SVGImage {

	public enum ImageSelection {
		Undefined, Large, Small
	};

	public enum State {
		Undefined, Disabled, Enabled, Active,
	}

	ImageSelection selection = ImageSelection.Undefined;
	State state = State.Undefined;

	public MenuAreaButton(MenuArea parent, String... images) {
		super(parent, images);
	}

	private int selectionToInt() {
		switch (selection) {
		case Small:
			return 0;

		case Large:
			return 3;

		default:
			break;
		}
		return 0;
	}

	private int stateToInt() {
		switch (getState()) {
		case Disabled:
			return 0;

		case Enabled:
			return 1;

		case Active:
			return 2;

		default:
			break;
		}
		return 0;
	}

	public void onMouseCaptured() {
		requestLayout();
	}

	public void onMouseLost() {
		requestLayout();
	}

	protected abstract State getState();

	@Override
	public void doLayout(double x, double y, double w, double h) {
		setSelection(w >= getLargeButtonWidth() ? ImageSelection.Large : ImageSelection.Small);
		super.doLayout(x, y, w, h);
	}

	public void setSelection(ImageSelection desiredSelection) {
		State desiredState = getState();

		if (selection != desiredSelection || state != desiredState) {
			selection = desiredSelection;
			state = desiredState;
			selectPhase(stateToInt() + selectionToInt());
		}
	}

	public Overlay getOverlay() {
		return getWorld().getViewport().getOverlay();
	}

	public NonLinearWorld getWorld() {
		return NonMaps.theMaps.getNonLinearWorld();
	}

	protected PresetManager getPresetManager() {
		return getWorld().getPresetManager();
	}

	protected double getMinWidth() {
		return Millimeter.toPixels(5);
	}

	protected double getMinHeight() {
		return Millimeter.toPixels(5);
	}

	private double getLargeButtonWidth() {
		return Millimeter.toPixels(31);
	}

}
