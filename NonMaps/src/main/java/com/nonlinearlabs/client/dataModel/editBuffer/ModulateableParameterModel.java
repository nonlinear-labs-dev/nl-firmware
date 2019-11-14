package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.Checksum;
import com.nonlinearlabs.client.dataModel.DoubleDataModelEntity;
import com.nonlinearlabs.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.client.dataModel.Updater;
import com.nonlinearlabs.client.dataModel.ValueDataModelEntity;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BooleanValues;

public class ModulateableParameterModel extends BasicParameterModel {

	public enum ModSource {
		None, A, B, C, D;

		public int toInt() {
			return ordinal();
		}

		public int toParameterId() {
			return 242 + ordinal();
		}

		public static ModSource fromParameterId(int i) {
			switch (i) {
			case 243:
				return A;

			case 244:
				return B;

			case 245:
				return C;

			case 246:
				return D;
			}
			return None;
		}
	}

	public ValueDataModelEntity modAmount = new ValueDataModelEntity();
	public EnumDataModelEntity<ModSource> modSource = new EnumDataModelEntity<ModSource>(ModSource.class,
			ModSource.None);
	public DoubleDataModelEntity ogModAmount = new DoubleDataModelEntity();
	public EnumDataModelEntity<ModSource> ogModSource = new EnumDataModelEntity<ModSource>(ModSource.class,
			ModSource.None);

	public ModulateableParameterModel(int id) {
		super(id);
		modAmount.metaData.bipolar.setValue(BooleanValues.on);
		modAmount.onChange(e -> notifyChanges());
		modSource.onChange(e -> notifyChanges());
		ogModAmount.onChange(e -> notifyChanges());
		ogModSource.onChange(e -> notifyChanges());
	}

	@Override
	public Updater createUpdater(Node c) {
		return new ModulateableParameterModelUpdater(c, this);
	}

	@Override
	public void getHash(Checksum crc) {
		crc.eat(modAmount.getQuantizedAndClipped(true));
		crc.eat(modSource.hashCode());
		super.getHash(crc);
	}
}
