package com.nonlinearlabs.NonMaps.client.presenters;

import java.util.HashMap;
import java.util.function.Function;

public class ParameterPresenterProviders {
	public static ParameterPresenterProviders theInstance = new ParameterPresenterProviders();

	public static ParameterPresenterProviders get() {
		return theInstance;
	}

	private HashMap<Integer, ParameterPresenterProvider> providers = new HashMap<Integer, ParameterPresenterProvider>();

	private ParameterPresenterProviders() {

	}

	public void register(int parameterId, Function<ParameterPresenter, Boolean> cb) {
		ParameterPresenterProvider p = providers.get(parameterId);

		if (p == null) {
			p = new ParameterPresenterProvider(parameterId);
			providers.put(parameterId, p);
		}

		p.onChange(cb);
	}
}
