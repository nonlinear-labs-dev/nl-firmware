package com.nonlinearlabs.client.presenters;

import java.util.HashMap;
import java.util.function.Function;

import com.nonlinearlabs.client.dataModel.Notifier;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterFactory;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;

public class ParameterPresenterProviders {
	private final class NotifierRelay extends Notifier<ParameterPresenter> {
		private final int parameterNumber;

		private NotifierRelay(int parameterNumber) {
			this.parameterNumber = parameterNumber;

			register(new ParameterId(parameterNumber, VoiceGroup.I), v -> {
				notifyChanges();
				return true;
			});

			register(new ParameterId(parameterNumber, VoiceGroup.II), v -> {
				notifyChanges();
				return true;
			});
		}

		public ParameterPresenter getValue() {
			return findMap(EditBufferModel.get().voiceGroup.getValue()).get(parameterNumber).getValue();
		}

	}

	public static ParameterPresenterProviders theInstance = new ParameterPresenterProviders();

	public static ParameterPresenterProviders get() {
		return theInstance;
	}

	private HashMap<Integer, ParameterPresenterProvider> voiceGroupIProviders = new HashMap<Integer, ParameterPresenterProvider>();
	private HashMap<Integer, ParameterPresenterProvider> voiceGroupIIProviders = new HashMap<Integer, ParameterPresenterProvider>();
	private HashMap<Integer, ParameterPresenterProvider> globalProviders = new HashMap<Integer, ParameterPresenterProvider>();

	private HashMap<Integer, Notifier<ParameterPresenter>> voiceGroupRelatedParameterSubcribtions = new HashMap<Integer, Notifier<ParameterPresenter>>();

	private ParameterPresenterProviders() {
		for (int id : ParameterFactory.getAllParameters()) {
			if (ParameterFactory.isGlobalParameter(id)) {
				findMap(VoiceGroup.Global).put(id, new ParameterPresenterProvider(id, VoiceGroup.Global));
			} else {
				findMap(VoiceGroup.I).put(id, new ParameterPresenterProvider(id, VoiceGroup.I));
				findMap(VoiceGroup.II).put(id, new ParameterPresenterProvider(id, VoiceGroup.II));
			}
		}

		EditBufferModel.get().voiceGroup.onChange(vg -> {
			voiceGroupRelatedParameterSubcribtions.forEach((a, b) -> {
				b.notifyChanges();
			});
			return true;
		});
	}

	public void register(ParameterId id, Function<ParameterPresenter, Boolean> cb) {
		findMap(id.getVoiceGroup()).get(id.getNumber()).onChange(cb);
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

	public void registerForCurrentVoiceGroup(int parameterNumber, Function<ParameterPresenter, Boolean> cb) {
		if (ParameterFactory.isGlobalParameter(parameterNumber)) {
			register(new ParameterId(parameterNumber, VoiceGroup.Global), cb);
		} else {
			if (!voiceGroupRelatedParameterSubcribtions.containsKey(parameterNumber))
				voiceGroupRelatedParameterSubcribtions.put(parameterNumber, new NotifierRelay(parameterNumber));

			voiceGroupRelatedParameterSubcribtions.get(parameterNumber).onChange(cb);

		}
	}
}