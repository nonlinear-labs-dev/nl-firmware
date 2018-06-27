package com.nonlinearlabs.NonMaps.client.dataModel;

public class ValueDataModelEntity extends Notifier<ValueDataModelEntity> implements DataModelEntityBase {

	public class ValueMetaData extends Notifier<ValueMetaData> {
		public DoubleDataModelEntity defaultValue = new DoubleDataModelEntity();
		public StringDataModelEntity scaling = new StringDataModelEntity();
		public IntegerDataModelEntity coarseDenominator = new IntegerDataModelEntity();
		public IntegerDataModelEntity fineDenominator = new IntegerDataModelEntity();
		public BooleanDataModelEntity bipolar = new BooleanDataModelEntity();
		public BooleanDataModelEntity isBoolean = new BooleanDataModelEntity();

		public ValueMetaData() {
			defaultValue.onChange(e -> notifyChanges());
			scaling.onChange(e -> notifyChanges());
			coarseDenominator.onChange(e -> notifyChanges());
			fineDenominator.onChange(e -> notifyChanges());
			bipolar.onChange(e -> notifyChanges());
		}

		@Override
		public ValueMetaData getValue() {
			return this;
		}
	}

	public ValueMetaData metaData = new ValueMetaData();
	public DoubleDataModelEntity value = new DoubleDataModelEntity();

	public ValueDataModelEntity() {
		metaData.onChange(e -> notifyChanges());
		value.onChange(e -> notifyChanges());
	}

	@Override
	public ValueDataModelEntity getValue() {
		return this;
	}

	@Override
	public void fromString(String str) {
		value.fromString(str);
	}
}