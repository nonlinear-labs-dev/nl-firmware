package com.nonlinearlabs.NonMaps.client.dataModel.value;


public class ModulationAmount extends QuantizedClippedValue {

	public ModulationAmount(ChangeListener listener) {
		super(listener);
		setBipolar(true);
	}

}
