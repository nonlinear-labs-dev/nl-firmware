package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.Checksum;
import com.nonlinearlabs.NonMaps.client.ColorTable;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.MacroControlParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.value.ModulationAmount;

public abstract class ModulatableParameter extends Parameter {

	private ModulationAmount amount = new ModulationAmount(new ModulationAmount.ChangeListener() {

		@Override
		public void onClippedValueChanged(Initiator initiator, double oldClippedValue, double newClippedValue) {
		}

		@Override
		public void onQuantizedValueChanged(Initiator initiator, double oldQuantizedValue, double newQuantizedValue) {
			if (initiator == Initiator.EXPLICIT_USER_ACTION)
				getNonMaps().getServerProxy().setModulationAmount(ModulatableParameter.this);

			notifyListeners();
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}

		@Override
		public void onRawValueChanged(Initiator initiator, double oldRawValue, double newRawValue) {
		}
	});

	private MacroControls modulationSource = MacroControls.NONE;

	public ModulatableParameter(MapsLayout parent) {
		super(parent);
		addChild(new ParameterName(this, getName()));
	}

	public ModulatableParameter(MapsLayout parent, String name) {
		super(parent);
		addChild(new ParameterName(this, name));
	}

	public ModulationAmount getModulationAmount() {
		return amount;
	}

	public void setModulationAmount(Initiator initiator, double amount) {
		this.amount.setRawValue(initiator, amount);
	}

	public MacroControls getModulationSource() {
		return modulationSource;
	}

	@Override
	public void getStateHash(Checksum crc) {
		super.getStateHash(crc);
		crc.eat(amount.getQuantizedClipped());
		crc.eat(modulationSource.hashCode());
		crc.eat(isSelectedParameterMyMacroControl());
	}

	public boolean isSelectedParameterMyMacroControl() {
		Parameter selectedParam = getNonMaps().getNonLinearWorld().getParameterEditor().getSelection();

		if (selectedParam != null && selectedParam instanceof MacroControlParameter) {
			MacroControlParameter myParam = getNonMaps().getNonLinearWorld().getParameterEditor().getMacroControls()
					.getControl(getModulationSource());

			if (myParam != null && myParam.equals(selectedParam))
				return true;
		}

		return false;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);

		if (isSelectedParameterMyMacroControl())
			getPixRect().drawRoundedRect(ctx, getBackgroundRoundings(), toXPixels(5), toXPixels(1.5), null,
					ColorTable.getMacroControlTargetColor());
	}

	public void setModulationSource(MacroControls src, Initiator initiator) {
		if (modulationSource != src) {

			MacroControlParameter modSrcParam = getModulationSourceParameter();

			if (modSrcParam != null)
				modSrcParam.removeModulatableParameter(this);

			modulationSource = src;

			modSrcParam = getModulationSourceParameter();

			if (modSrcParam != null)
				modSrcParam.addModulatableParameter(this);

			if (initiator == Initiator.EXPLICIT_USER_ACTION)
				getNonMaps().getServerProxy().setModulationSource(this);

			getValue().setRawValue(Initiator.INDIRECT_USER_ACTION, getValue().getQuantizedClipped());
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}

	private MacroControlParameter getModulationSourceParameter() {
		return getNonMaps().getNonLinearWorld().getParameterEditor().getMacroControls()
				.getControl(getModulationSource());
	}

	@Override
	public boolean updateValues(Node child) {

		if (super.updateValues(child))
			return true;

		String nodeName = child.getNodeName();

		try {
			String value = ServerProxy.getText(child);

			if (nodeName.equals("modAmount")) {
				setModulationAmount(Initiator.INDIRECT_USER_ACTION, Double.parseDouble(value));
			} else if (nodeName.equals("modSrc")) {
				setModulationSource(MacroControls.fromInt(Integer.parseInt(value)), Initiator.INDIRECT_USER_ACTION);
			} else if (nodeName.equals("mod-amount-stringizer")) {
				amount.setStringizer(value);
			} else if (nodeName.equals("mod-amount-coarse")) {
				amount.setCoarseDenominator(Double.parseDouble(value));
			} else if (nodeName.equals("mod-amount-fine")) {
				amount.setFineDenominator(Double.parseDouble(value));
			} else {
				return false;
			}

			return true;
		} catch (Exception e) {

		}

		return false;
	}

	protected String getModSourceString() {
		switch (getModulationSource()) {
		case A:
			return "\uE000";

		case B:
			return "\uE001";

		case C:
			return "\uE002";

		case D:
			return "\uE003";

		default:
		}

		return "";
	}

	@Override
	public String getFullNameWithGroup() {
		BasicParameterModel bpm = EditBufferModel.get().findParameter(getParameterID());
		boolean changed = bpm.value.value.getValue() != bpm.originalValue.getValue();
		return getGroupName() + "   \u2013   " + getName().getLongName() + (changed ? " *" : "");
	}

	public void applyModulation(Initiator initiator, double diff) {
		getValue().applyModulation(initiator, amount.getQuantizedClipped() * diff);
	}

	public void modulationAmountInc(boolean fine) {
		amount.inc(Initiator.EXPLICIT_USER_ACTION, fine);
	}

	public void modulationAmountDec(boolean fine) {
		amount.dec(Initiator.EXPLICIT_USER_ACTION, fine);
	}
}
