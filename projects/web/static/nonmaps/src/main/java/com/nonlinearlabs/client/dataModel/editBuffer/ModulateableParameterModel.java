package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.Checksum;
import com.nonlinearlabs.client.dataModel.DoubleDataModelEntity;
import com.nonlinearlabs.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.client.dataModel.Updater;
import com.nonlinearlabs.client.dataModel.ValueDataModelEntity;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BooleanValues;

public class ModulateableParameterModel extends BasicParameterModel {

	public enum ModSource {
		None, A, B, C, D, E, F;

		public int toInt() {
			return ordinal();
		}

		public String toString() {
			switch (this) {
				case A:
					return "\uE000";

				case B:
					return "\uE001";

				case C:
					return "\uE002";

				case D:
					return "\uE003";

				case E:
					return "\uE200";

				case F:
					return "\uE201";

				default:
			}

			return "";
		}

		public ParameterId toParameterId() {
			switch (this) {
				case E:
					return new ParameterId(369, VoiceGroup.Global);

				case F:
					return new ParameterId(371, VoiceGroup.Global);

				default:
					break;
			}
			return new ParameterId(242 + ordinal(), VoiceGroup.Global);
		}

		public static ModSource fromParameterId(ParameterId i) {
			switch (i.getNumber()) {
				case 243:
					return A;

				case 244:
					return B;

				case 245:
					return C;

				case 246:
					return D;

				case 369:
					return E;

				case 371:
					return F;
			}
			return None;
		}
	}

	public ValueDataModelEntity modAmount = new ValueDataModelEntity();
	public ValueDataModelEntity modBase = new ValueDataModelEntity();
	public EnumDataModelEntity<ModSource> modSource = new EnumDataModelEntity<ModSource>(ModSource.class,
			ModSource.None);
	public DoubleDataModelEntity ogModAmount = new DoubleDataModelEntity();
	public EnumDataModelEntity<ModSource> ogModSource = new EnumDataModelEntity<ModSource>(ModSource.class,
			ModSource.None);

	public ModulateableParameterModel(ParameterId id) {
		super(id);
		modAmount.metaData.bipolar.setValue(BooleanValues.on);
		modAmount.onChange(e -> notifyChanges());
		modSource.onChange(e -> notifyChanges());
		modBase.onChange(e -> notifyChanges());
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
		crc.eat(modBase.getQuantizedAndClipped(true));
		crc.eat(modSource.hashCode());
		super.getHash(crc);
	}
}
