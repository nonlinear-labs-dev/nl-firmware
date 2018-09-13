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
}
