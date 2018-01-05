package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls;

import java.util.LinkedList;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.ColorTable;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.Renameable;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.world.Name;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelLarge;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.SliderHorizontalWithHandle;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ValueDisplaySmall;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.ParameterInfoDialog;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.MacroControlContextMenu;

public abstract class MacroControlParameter extends Parameter implements Renameable {

	private String info = "";

	private class MacroControlValueDisplay extends ValueDisplaySmall {
		private MacroControlValueDisplay(MapsLayout parent) {
			super(parent);
		}

		@Override
		protected double getBasicHeight() {
			return 22;
		}
	}

	private class MacroControlSlider extends SliderHorizontalWithHandle {
		private MacroControlSlider(MapsLayout parent) {
			super(parent);
		}
	}

	private LinkedList<ModulatableParameter> targets = new LinkedList<ModulatableParameter>();

	MacroControlParameter(MacrosCol parent, String defName) {
		super(parent);
		addChild(new LabelLarge(this, getName()) {
			@Override
			protected double getBasicHeight() {
				return 23;
			}

			@Override
			protected double getBasicWidth() {
				return 80;
			}

		});
		addChild(new MacroControlSlider(this));
		addChild(new MacroControlValueDisplay(this));
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
	protected Name createName() {
		return new MacroParameterName(this);
	}

	public void applyModulation(Initiator initiator, double delta) {
		getValue().applyModulation(initiator, delta);
	}

	public void addModulatableParameter(ModulatableParameter modulatableParameter) {
		targets.add(modulatableParameter);
	}

	public void removeModulatableParameter(ModulatableParameter modulatableParameter) {
		targets.remove(modulatableParameter);
	}

	@Override
	public void onValueChanged(Initiator initiator, double diff) {
		super.onValueChanged(initiator, diff);

		if (initiator == Initiator.EXPLICIT_USER_ACTION || initiator == Initiator.MODULATION)
			if (Math.abs(diff) > 0.0)
				for (ModulatableParameter p : targets)
					p.applyModulation(Initiator.MODULATION, diff);
	}

	@Override
	public MacroParameterName getName() {
		return (MacroParameterName) super.getName();
	}

	@Override
	public String getCurrentName() {
		return getName().getEditName();
	}

	@Override
	public String getTitleName() {
		return getName().getLongName();
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

	@Override
	public boolean hasContextMenu() {
		return true;
	}

	@Override
	protected boolean updateValues(Node child) {
		if (super.updateValues(child))
			return true;

		String nodeName = child.getNodeName();
		if (nodeName != null && nodeName.equals("info")) {
			try {
				setInfo(ServerProxy.getText(child));
			} catch (Exception e) {
			}
		}

		return false;
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