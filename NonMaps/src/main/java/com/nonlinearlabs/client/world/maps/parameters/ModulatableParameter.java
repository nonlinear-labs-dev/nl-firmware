package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.Checksum;
import com.nonlinearlabs.client.ColorTable;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel;
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
		int mySourceID = presenter.modulation.modulationSource.toParameterId();
		int selectedParameterID = EditBufferModel.get().selectedParameter.getValue();
		return mySourceID == selectedParameterID;
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
		EditBufferUseCases.get().setModulationSource(getParameterID(), toModSource(src));
	}

	@Override
	public String getFullNameWithGroup() {
		return presenter.fullNameWithGroup;
	}

	public void modulationAmountInc(boolean fine) {
		EditBufferUseCases.get().incModulationAmount(getParameterID(), fine);
	}

	public void modulationAmountDec(boolean fine) {
		EditBufferUseCases.get().decModulationAmount(getParameterID(), fine);
	}
}
