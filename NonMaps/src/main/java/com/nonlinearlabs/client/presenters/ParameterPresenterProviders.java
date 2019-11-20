package com.nonlinearlabs.client.presenters;

import java.util.HashMap;
import java.util.function.Function;

import com.nonlinearlabs.client.dataModel.Notifier;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterFactory;

public class ParameterPresenterProviders {
	private final class NotifierRelay extends Notifier<ParameterPresenter> {
		private final int parameterId;

		private NotifierRelay(int parameterId) {
			this.parameterId = parameterId;
			register(parameterId, VoiceGroup.I, v -> notifyChanges());
			register(parameterId, VoiceGroup.II, v -> notifyChanges());
		}

		public ParameterPresenter getValue() {
			return findMap(EditBufferModel.get().voiceGroup.getValue()).get(parameterId).getValue();
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

	public void register(int parameterId, VoiceGroup vg, Function<ParameterPresenter, Boolean> cb) {
		findMap(vg).get(parameterId).onChange(cb);
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
		if (ParameterFactory.isGlobalParameter(parameterId)) {
			register(parameterId, VoiceGroup.Global, cb);
		} else {
			if (!voiceGroupRelatedParameterSubcribtions.containsKey(parameterId))
				voiceGroupRelatedParameterSubcribtions.put(parameterId, new NotifierRelay(parameterId));

			voiceGroupRelatedParameterSubcribtions.get(parameterId).onChange(cb);

		}
	}
}
