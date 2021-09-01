package com.nonlinearlabs.client.useCases;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.ParameterPresenter;

public class PresetRecall {
	private static PresetRecall instance = new PresetRecall();

	public static PresetRecall get() {
		return instance;
	}

	public void doRecallCurrentSelectedParameter() {
		ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		NonMaps.get().getServerProxy().recallCurrentParameterFromPreset(p.id);
	}

}
