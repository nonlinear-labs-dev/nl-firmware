package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.Checksum;
import com.nonlinearlabs.client.ColorTable;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulatableParameterWithoutName extends Parameter {

	public ModulatableParameterWithoutName(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
	}

	public ModulatableParameterWithoutName(MapsLayout parent, String name, int parameterID) {
		super(parent, parameterID);
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

	@Override
	public String getFullNameWithGroup() {
		return presenter.fullNameWithGroup;
	}

}
