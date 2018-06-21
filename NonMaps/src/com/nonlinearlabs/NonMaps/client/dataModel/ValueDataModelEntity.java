package com.nonlinearlabs.NonMaps.client.dataModel;

public class ValueDataModelEntity extends DoubleDataModelEntity {

	public class ValueMetaData {
		public DoubleDataModelEntity defaultValue = new DoubleDataModelEntity();
		public StringDataModelEntity scaling = new StringDataModelEntity();
		public IntegerDataModelEntity coarseDenominator = new IntegerDataModelEntity();
		public IntegerDataModelEntity fineDenominator = new IntegerDataModelEntity();
		public BooleanDataModelEntity bipolar = new BooleanDataModelEntity();
	}

	public ValueMetaData metaData = new ValueMetaData();

	public ValueDataModelEntity() {
	}
}