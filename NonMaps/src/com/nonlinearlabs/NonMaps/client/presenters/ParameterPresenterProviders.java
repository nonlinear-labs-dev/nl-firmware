package com.nonlinearlabs.NonMaps.client.presenters;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;

public class ParameterPresenterProviders {
	public static ParameterPresenterProviders theInstance = new ParameterPresenterProviders();

	public static ParameterPresenterProviders get() {
		return theInstance;
	}

	private HashMap<Integer, ParameterPresenterProvider> providers = new HashMap<Integer, ParameterPresenterProvider>();
	private ParameterPresenterProvider selectedParameter;
	private List<Function<ParameterPresenter, Boolean>> selectedParameterCallBacks;
	
	private ParameterPresenterProviders() {
		selectedParameter = new ParameterPresenterProvider(EditBufferModel.get().selectedParameter.getValue());
		
		selectedParameterCallBacks = new ArrayList<Function<ParameterPresenter, Boolean>>();
		
		EditBufferModel.get().selectedParameter.onChange(id -> {
			selectedParameter = new ParameterPresenterProvider(id);
			for(Function<ParameterPresenter, Boolean> d: selectedParameterCallBacks) {
				selectedParameter.onChange(d);
			}
			return true;
		});
	}
	
	public void registerSelected(Function<ParameterPresenter, Boolean> cb) {
		selectedParameterCallBacks.add(cb);
		selectedParameter.onChange(cb);
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
