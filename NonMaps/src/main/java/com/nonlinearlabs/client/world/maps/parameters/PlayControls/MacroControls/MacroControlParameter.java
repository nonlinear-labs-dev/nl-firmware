package com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.ColorTable;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.Renameable;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelLarge;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontalWithHandle;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;
import com.nonlinearlabs.client.world.overlay.ContextMenu;
import com.nonlinearlabs.client.world.overlay.Overlay;
import com.nonlinearlabs.client.world.overlay.ParameterInfoDialog;
import com.nonlinearlabs.client.world.overlay.belt.parameters.MacroControlContextMenu;

public abstract class MacroControlParameter extends Parameter implements Renameable {

	private String info = "";

	private class MacroControlValueDisplay extends ValueDisplaySmall {
		private MacroControlValueDisplay(MapsLayout parent, int parameterID) {
			super(parent, parameterID);
		}

		@Override
		protected double getBasicHeight() {
			return 22;
		}
	}

	private class MacroControlSlider extends SliderHorizontalWithHandle {
		private MacroControlSlider(MapsLayout parent, int parameterID) {
			super(parent, parameterID);
		}
	}

	MacroControlParameter(MacrosCol parent, String defName, int parameterID) {
		super(parent, parameterID);
		addChild(new LabelLarge(this) {
			@Override
			protected double getBasicHeight() {
				return 23;
			}

			@Override
			protected double getBasicWidth() {
				return 80;
			}

		});
		addChild(new MacroControlSlider(this, getParameterID()));
		addChild(new MacroControlValueDisplay(this, getParameterID()));
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);

		if (isSelected())
			getPixRect().drawRoundedRect(ctx, getBackgroundRoundings(), toXPixels(4), toXPixels(1), null,
					ColorTable.getMacroControlTargetColor());
	}

	@Override
	public boolean shouldHaveHandleOnly() {
		return true;
	}

	@Override
	public String getCurrentName() {
		return presenter.userGivenName;
	}

	@Override
	public String getTitleName() {
		return presenter.longName;
	}

	@Override
	public String getEntityName() {
		return "Macro Control";
	}

	@Override
	public void setName(String newName) {
		NonMaps.theMaps.getServerProxy().renameMacroControl(this, newName);
	}

	@Override
	public ContextMenu createContextMenu(Overlay o) {
		return new MacroControlContextMenu(o, this);
	}

	public void setInfo(String text) {
		if (!text.equals(info)) {
			info = text;
			if (ParameterInfoDialog.isShown())
				ParameterInfoDialog.update();
		}
	}

	public String getInfo() {
		return info;
	}
}