package com.nonlinearlabs.NonMaps.client.dataModel;

public class ModulateableParameter extends BasicParameter {
	public enum ModSource {
		None, A, B, C, D
	}

	public DoubleDataModelEntity modAmount = new DoubleDataModelEntity();
	public EnumDataModelEntity<ModSource> modSource = new EnumDataModelEntity<ModSource>(ModSource.class, ModSource.None);
}
