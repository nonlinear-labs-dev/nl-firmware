package com.nonlinearlabs.NonMaps.client.dataModel.editBuffer;

import com.google.gwt.core.client.GWT;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.dataModel.DoubleDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.IntegerDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.StringDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.Updater;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.BeltParameterLayout;

public class ModulateableParameter extends BasicParameterModel {
	
	public enum ModSource {
		None, A, B, C, D
	}

	public DoubleDataModelEntity modAmount = new DoubleDataModelEntity();
	public EnumDataModelEntity<ModSource> modSource = new EnumDataModelEntity<ModSource>(ModSource.class, ModSource.None);
	public StringDataModelEntity modAmountStringizer = new StringDataModelEntity();
	public IntegerDataModelEntity modAmountCoarse = new IntegerDataModelEntity();
	public IntegerDataModelEntity modAmountFine = new IntegerDataModelEntity();
	public DoubleDataModelEntity ogModAmount = new DoubleDataModelEntity();
	public IntegerDataModelEntity mcParameterID = new IntegerDataModelEntity();
	public EnumDataModelEntity<ModSource> ogModSource = new EnumDataModelEntity<ModSource>(ModSource.class, ModSource.None);
	
	
	@Override
	public boolean isChanged() {
		return super.isChanged() || isModAmountChanged() || isModAmountChanged();
	}

	public boolean isModChanged() {
		boolean ret = isModAmountChanged() || isModSourceChanged();
		return ret;
	}
	

	public boolean isMCPosChanged() {
		if(!mcParameterID.getValue().toString().isEmpty()) {
			BasicParameterModel mcBPM = EditBufferModel.get().findParameter(mcParameterID.getValue());
			if(mcBPM != null && mcBPM instanceof MacroControlParameter) {
				return mcBPM.isChanged();
			}
		}
		return false;
	}
	
	public boolean isModAmountChanged() {
		return ogModAmount.getValue() != modAmount.getValue();
	}
	
	public boolean isModSourceChanged() {
		return ogModSource.getValue() != modSource.getValue();
	}
	
	@Override
	public Updater getUpdater(Node c) {
		return new ModulateableParameterUpdater(c, this);
	}


	public boolean isChanged(BeltParameterLayout.Mode mode) {
		switch(mode) {
		case mcAmount:
			return isModAmountChanged();
		case mcSource:
			return isModSourceChanged();
		case mcValue:
			return isMCPosChanged();
		case modulateableParameter:
		case paramValue:
		case unmodulateableParameter:
			super.isChanged();
		case mcUpper:
		case mcLower:
		default:
			break;
		}
		return false;
	}
}
