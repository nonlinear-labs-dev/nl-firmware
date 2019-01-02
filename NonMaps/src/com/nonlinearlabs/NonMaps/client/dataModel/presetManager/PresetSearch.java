package com.nonlinearlabs.NonMaps.client.dataModel.presetManager;

import java.util.HashSet;

import com.nonlinearlabs.NonMaps.client.dataModel.BooleanDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.StringDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.StringSetDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.BooleanValues;

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
}
