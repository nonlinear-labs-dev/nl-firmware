package com.nonlinearlabs.NonMaps.client.dataModel;

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
