package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.Checksum;
import com.nonlinearlabs.client.ColorTable;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;

public class ModulatableParameter extends Parameter {

	private MacroControls modulationSource = MacroControls.NONE;

	public ModulatableParameter(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new ModulateableParameterName(this));
	}

	public ModulatableParameter(MapsLayout parent, String name, int parameterID) {
		super(parent, parameterID);
		addChild(new ModulateableParameterName(this, name));
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
		ParameterId mySourceID = presenter.modulation.modulationSource.toParameterId();
		int selectedParameterID = EditBufferModel.get().selectedParameter.getValue();
		return mySourceID.getNumber() == selectedParameterID;
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
		case E:
			return ModulateableParameterModel.ModSource.E;
		case F:
			return ModulateableParameterModel.ModSource.F;
		}
		return ModulateableParameterModel.ModSource.None;
	}

	public void setModulationSource(MacroControls src) {
		EditBufferUseCases.get().setModulationSource(getParameterNumber(), toModSource(src));
	}

	@Override
	public String getFullNameWithGroup() {
		return presenter.fullNameWithGroup;
	}

	public void modulationAmountInc(boolean fine, VoiceGroup vg) {
		EditBufferUseCases.get().incModulationAmount(getParameterNumber(), fine);
	}

	public void modulationAmountDec(boolean fine, VoiceGroup vg) {
		EditBufferUseCases.get().decModulationAmount(getParameterNumber(), fine);
	}
}
