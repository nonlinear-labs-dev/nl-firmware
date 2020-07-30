package com.nonlinearlabs.client.dataModel.presetManager;

import java.util.HashSet;

import com.nonlinearlabs.client.dataModel.BooleanDataModelEntity;
import com.nonlinearlabs.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.client.dataModel.StringDataModelEntity;
import com.nonlinearlabs.client.dataModel.StringSetDataModelEntity;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BooleanValues;

public class PresetSearch {

	private static PresetSearch theSearch = new PresetSearch();

	public static PresetSearch get() {
		return theSearch;
	}

	public enum SearchQueryCombination {
		AND, OR
	}

	public void reset() {
		query.setValue("");
		results.setValue(new HashSet<String>());
		currentFilterMatch.setValue("");
		loadPreset.setValue(false);
	}

	public BooleanDataModelEntity searchActive = new BooleanDataModelEntity(BooleanValues.off);
	public StringDataModelEntity query = new StringDataModelEntity();
	public BooleanDataModelEntity searchInNames = new BooleanDataModelEntity(BooleanValues.on);
	public BooleanDataModelEntity searchInComments = new BooleanDataModelEntity(BooleanValues.on);
	public BooleanDataModelEntity searchInDeviceNames = new BooleanDataModelEntity(BooleanValues.on);
	public BooleanDataModelEntity zoomToMatches = new BooleanDataModelEntity(BooleanValues.on);
	public EnumDataModelEntity<SearchQueryCombination> combination = new EnumDataModelEntity<SearchQueryCombination>(
			SearchQueryCombination.class, SearchQueryCombination.AND);
	public StringSetDataModelEntity results = new StringSetDataModelEntity();
	public StringDataModelEntity currentFilterMatch = new StringDataModelEntity();
	public BooleanDataModelEntity loadPreset = new BooleanDataModelEntity(BooleanValues.off);
}
