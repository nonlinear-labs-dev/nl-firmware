package com.nonlinearlabs.NonMaps.client.useCases;

import com.nonlinearlabs.NonMaps.client.NonMaps;

public class PresetRecall {	
	private static PresetRecall instance = new PresetRecall();
	
	public static PresetRecall get() {
		return instance;
	}
	
	public void doRecallCurrentSelectedParameter() {
		NonMaps.get().getServerProxy().recallCurrentParameterFromPreset();
	}
	
}
