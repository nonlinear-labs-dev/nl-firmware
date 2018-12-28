package com.nonlinearlabs.NonMaps.client.presenters;

import java.util.LinkedList;
import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.dataModel.presetManager.PresetSearch.SearchQueryCombination;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.BooleanValues;

public class PresetSearchProvider {
	public static PresetSearchProvider theInstance = new PresetSearchProvider();

	public static PresetSearchProvider get() {
		return theInstance;
	}

	private LinkedList<Function<PresetSearch, Boolean>> clients = new LinkedList<Function<PresetSearch, Boolean>>();
	private PresetSearch presenter = new PresetSearch();

	private PresetSearchProvider() {
		getModel().query.onChange(q -> {
			presenter.query = q;
			notifyClients();
			return true;
		});

		getModel().combination.onChange(q -> {
			if (q == SearchQueryCombination.AND) {
				presenter.andCombination = true;
				presenter.orCombination = false;
			} else if (q == SearchQueryCombination.OR) {
				presenter.andCombination = false;
				presenter.orCombination = true;
			}
			notifyClients();
			return true;
		});

		getModel().results.onChange(r -> {
			int s = r.size();
			switch (s) {
			case 0:
				presenter.numMatches = "No Matches.";
				getModel().matchCount.setValue(0);
				break;

			case 1:
				presenter.numMatches = "1 Match.";
				getModel().matchCount.setValue(1);
				break;

			default:
				presenter.numMatches = s + " Matches.";
				getModel().matchCount.setValue(Integer.valueOf(s));
			}
			notifyClients();
			return true;
		});

		getModel().searchInNames.onChange(r -> {
			presenter.searchInNames = r == BooleanValues.on;
			notifyClients();
			return true;
		});

		getModel().searchInComments.onChange(r -> {
			presenter.searchInComments = r == BooleanValues.on;
			notifyClients();
			return true;
		});

		getModel().searchInDeviceNames.onChange(r -> {
			presenter.searchInDeviceNames = r == BooleanValues.on;
			notifyClients();
			return true;
		});

		getModel().zoomToMatches.onChange(r -> {
			presenter.zoomToAllMatches = r == BooleanValues.on;
			notifyClients();
			return true;
		});
	}

	public com.nonlinearlabs.NonMaps.client.dataModel.presetManager.PresetSearch getModel() {
		return com.nonlinearlabs.NonMaps.client.dataModel.presetManager.PresetSearch.get();
	}

	protected void notifyClients() {
		clients.removeIf(listener -> !listener.apply(presenter));
	}

	public void register(Function<PresetSearch, Boolean> cb) {
		clients.add(cb);
		cb.apply(presenter);
	}
}
