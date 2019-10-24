package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.Checksum;
import com.nonlinearlabs.client.dataModel.DoubleDataModelEntity;
import com.nonlinearlabs.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.client.dataModel.IntegerDataModelEntity;
import com.nonlinearlabs.client.dataModel.Updater;
import com.nonlinearlabs.client.dataModel.ValueDataModelEntity;
import com.nonlinearlabs.client.world.overlay.belt.parameters.BeltParameterLayout;

public class ModulateableParameterModel extends BasicParameterModel {

	public ModulateableParameterModel(int id) {
		super(id);
	}

	public enum ModSource {
		None, A, B, C, D;

		public int toInt() {
			return ordinal();
		}
	}

	public ValueDataModelEntity modAmount = new ValueDataModelEntity();
	public EnumDataModelEntity<ModSource> modSource = new EnumDataModelEntity<ModSource>(ModSource.class,
			ModSource.None);
	public DoubleDataModelEntity ogModAmount = new DoubleDataModelEntity();
	public IntegerDataModelEntity mcParameterID = new IntegerDataModelEntity();
	public EnumDataModelEntity<ModSource> ogModSource = new EnumDataModelEntity<ModSource>(ModSource.class,
			ModSource.None);

	@Override
	public boolean isChanged() {
		boolean src = isModSourceChanged();
		boolean amt = isModAmountChanged();
		boolean val = isValueChanged();
		return src || amt || val;
	}

	public boolean isMCPosChanged() {
		if (!mcParameterID.getValue().toString().isEmpty()) {
			BasicParameterModel mcBPM = EditBufferModel.findParameter(mcParameterID.getValue());
			if (mcBPM != null && mcBPM instanceof MacroControlParameterModel) {
				return mcBPM.isChanged();
			}
		}
		return false;
	}

	public boolean isModAmountChanged() {
		int denominator = modAmount.metaData.fineDenominator.getValue();
		int ogRounded = (int) (ogModAmount.getValue() * denominator);
		int nowRounded = (int) (modAmount.value.getValue() * denominator);
		return ogRounded != nowRounded;
	}

	public boolean isModSourceChanged() {
		return ogModSource.getValue() != modSource.getValue();
	}

	@Override
	public Updater getUpdater(Node c) {
		return new ModulateableParameterUpdater(c, this);
	}

	public boolean isChanged(BeltParameterLayout.Mode mode) {
		switch (mode) {
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

	@Override
	public void getHash(Checksum crc) {
		crc.eat(modAmount.getQuantizedAndClipped(true));
		crc.eat(modSource.hashCode());
		super.getHash(crc);
	}
}
