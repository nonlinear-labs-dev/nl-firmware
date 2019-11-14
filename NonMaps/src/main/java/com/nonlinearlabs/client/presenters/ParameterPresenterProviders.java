package com.nonlinearlabs.client.presenters;

import java.util.HashMap;
import java.util.function.Function;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;

public class ParameterPresenterProviders {
	public static ParameterPresenterProviders theInstance = new ParameterPresenterProviders();

	public static ParameterPresenterProviders get() {
		return theInstance;
	}

	private HashMap<Integer, ParameterPresenterProvider> voiceGroupIProviders = new HashMap<Integer, ParameterPresenterProvider>();
	private HashMap<Integer, ParameterPresenterProvider> voiceGroupIIProviders = new HashMap<Integer, ParameterPresenterProvider>();
	private HashMap<Integer, ParameterPresenterProvider> globalProviders = new HashMap<Integer, ParameterPresenterProvider>();

	private ParameterPresenterProviders() {

	}

	public void register(int parameterId, VoiceGroup vg, Function<ParameterPresenter, Boolean> cb) {
		ParameterPresenterProvider p = findMap(vg).get(parameterId);

		if (p == null) {
			p = new ParameterPresenterProvider(parameterId, vg);
			findMap(vg).put(parameterId, p);
		}

		p.onChange(cb);
	}

	private HashMap<Integer, ParameterPresenterProvider> findMap(VoiceGroup vg) {
		switch (vg) {
		case Global:
			return globalProviders;

		case I:
			return voiceGroupIProviders;

		case II:
			return voiceGroupIIProviders;
		}
		return null;
	}

	public void registerForCurrentVoiceGroup(int parameterId, Function<ParameterPresenter, Boolean> cb) {
	}
}
