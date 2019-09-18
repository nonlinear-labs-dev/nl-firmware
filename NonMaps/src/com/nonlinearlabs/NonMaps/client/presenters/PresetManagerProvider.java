package com.nonlinearlabs.NonMaps.client.presenters;

import java.util.LinkedList;
import java.util.Map;
import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.presetManager.Bank;

public class PresetManagerProvider {
	public static PresetManagerProvider theInstance = new PresetManagerProvider();

	public static PresetManagerProvider get() {
		return theInstance;
	}

	private LinkedList<Function<PresetManagerPresenter, Boolean>> clients;
	private PresetManagerPresenter pm = null;

	public PresetManagerProvider() {
		pm = new PresetManagerPresenter();
		clients = new LinkedList<Function<PresetManagerPresenter, Boolean>>();
		com.nonlinearlabs.NonMaps.client.dataModel.presetManager.PresetManager.get().getBanks()
				.onChange(b -> onBanksChanged(b));
	}

	private Boolean onBanksChanged(Map<String, Bank> dmBanks) {
		boolean removed = pm.banks.removeIf(b -> !dmBanks.containsKey(b.uuid));

		int numBanksBefore = pm.banks.size();
		dmBanks.forEach((key, bank) -> {
			try {
				if (!pm.banks.stream().anyMatch((b) -> b.uuid.equals(key))) {
					pm.banks.add(new PresetManagerPresenter.Bank(bank));
				}
			} catch (Exception e) {
			}
		});

		int numBanksAfter = pm.banks.size();
		boolean inserted = numBanksAfter != numBanksBefore;
		boolean changed = inserted || removed;

		if (changed)
			notifyClients();

		return true;
	}

	protected void notifyClients() {
		clients.removeIf(listener -> !listener.apply(pm));
	}

	public void register(Function<PresetManagerPresenter, Boolean> cb) {
		clients.add(cb);
		cb.apply(pm);
	}

	public PresetManagerPresenter getPresenter() {
		return pm;
	}
}
