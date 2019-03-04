package com.nonlinearlabs.NonMaps.client.dataModel.editBuffer;

import com.nonlinearlabs.NonMaps.client.dataModel.DoubleDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.IntegerDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.StringDataModelEntity;

public class ModulateableParameter extends BasicParameterModel {
	
	public enum ModSource {
		None, A, B, C, D
	}

	public DoubleDataModelEntity modAmount = new DoubleDataModelEntity();
	public EnumDataModelEntity<ModSource> modSource = new EnumDataModelEntity<ModSource>(ModSource.class, ModSource.None);
	public StringDataModelEntity modAmountStringizer = new StringDataModelEntity();
	public IntegerDataModelEntity modAmountCoarse = new IntegerDataModelEntity();
	public IntegerDataModelEntity modAmountFine = new IntegerDataModelEntity();
	public DoubleDataModelEntity ogModAmount = new DoubleDataModelEntity();
	public IntegerDataModelEntity mcParameterID = new IntegerDataModelEntity();
	public EnumDataModelEntity<ModSource> ogModSource = new EnumDataModelEntity<ModSource>(ModSource.class, ModSource.None);
	
	
	@Override
	public boolean isChanged() {
		return super.isChanged() || modSource.getValue() != ogModSource.getValue() || modAmount.getValue() != ogModAmount.getValue();
	}


	public boolean isMCPosChanged() {
		if(!mcParameterID.getValue().toString().isEmpty()) {
			BasicParameterModel mcBPM = EditBufferModel.get().findParameter(mcParameterID.getValue());
			if(mcBPM != null && mcBPM instanceof MacroControlParameter) {
				return mcBPM.isChanged();
			}
		}
		return false;
	}
}
