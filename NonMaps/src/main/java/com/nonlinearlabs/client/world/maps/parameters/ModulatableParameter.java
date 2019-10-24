package com.nonlinearlabs.client.world.maps.parameters;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.Checksum;
import com.nonlinearlabs.client.ColorTable;
import com.nonlinearlabs.client.ServerProxy;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel;
import com.nonlinearlabs.client.useCases.ModulateableParameterUseCases;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.MacroControlParameter;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;
import com.nonlinearlabs.client.world.maps.parameters.value.ModulationAmount;

public class ModulatableParameter extends Parameter {

	private MacroControls modulationSource = MacroControls.NONE;

	public ModulatableParameter(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new ParameterName(this));
	}

	public ModulatableParameter(MapsLayout parent, String name, int parameterID) {
		super(parent, parameterID);
		addChild(new ParameterName(this, name));
	}

	public MacroControls getModulationSource() {
		return modulationSource;
	}

	@Override
	public void getStateHash(Checksum crc) {
		super.getStateHash(crc);
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
	protected RGB getRoundingColor() {
		if (isSelectedParameterMyMacroControl()) {
			return ColorTable.getMacroControlTargetColor();
		} else {
			return super.getRoundingColor();
		}
	}

	public ModulateableParameterModel.ModSource toModSource(MacroControls i) {
		switch (i) {
		case NONE:
			return ModulateableParameterModel.ModSource.None;
		case A:
			return ModulateableParameterModel.ModSource.A;
		case B:
			return ModulateableParameterModel.ModSource.B;
		case C:
			return ModulateableParameterModel.ModSource.C;
		case D:
			return ModulateableParameterModel.ModSource.D;
		}
		return ModulateableParameterModel.ModSource.None;
	}

	public void setModulationSource(MacroControls src) {
		ModulateableParameterUseCases.setModulationSource(getParameterID(), toModSource(src));
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
		BasicParameterModel bpm = EditBufferModel.findParameter(getParameterID());
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
