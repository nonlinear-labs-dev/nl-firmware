package com.nonlinearlabs.client.dataModel;

import com.google.gwt.core.client.JavaScriptObject;
import com.nonlinearlabs.client.tools.NLMath;

public class ValueDataModelEntity extends Notifier<ValueDataModelEntity> implements DataModelEntityBase {

	public class ValueMetaData extends Notifier<ValueMetaData> {
		public DoubleDataModelEntity defaultValue = new DoubleDataModelEntity();
		public StringDataModelEntity scaling = new StringDataModelEntity();
		public IntegerDataModelEntity coarseDenominator = new IntegerDataModelEntity();
		public IntegerDataModelEntity fineDenominator = new IntegerDataModelEntity();
		public BooleanDataModelEntity bipolar = new BooleanDataModelEntity();
		public BooleanDataModelEntity isBoolean = new BooleanDataModelEntity();

		private JavaScriptObject stringizer;

		public ValueMetaData() {
			updateStringizer("return $wnd.formatDimensionDigits(cpValue * 100, '%', withUnit, 3);");

			defaultValue.onChange(e -> notifyChanges());
			scaling.onChange(e -> {
				if (!e.isEmpty())
					updateStringizer(e);
				return notifyChanges();
			});
			coarseDenominator.onChange(e -> notifyChanges());
			fineDenominator.onChange(e -> notifyChanges());
			bipolar.onChange(e -> notifyChanges());
		}

		private native void updateStringizer(String body) /*-{
															this.@com.nonlinearlabs.client.dataModel.ValueDataModelEntity.ValueMetaData::stringizer = new Function(
															"cpValue", "withUnit", body);
															}-*/;

		@Override
		public ValueMetaData getValue() {
			return this;
		}

		public String getDecoratedValue(boolean withUnit, double cpValue) {
			if (stringizer == null)
				return "";

			return stringize(withUnit, cpValue);
		}

		private native String stringize(boolean withUnit, double cpValue) /*-{
																			var stringizer = this.@com.nonlinearlabs.client.dataModel.ValueDataModelEntity.ValueMetaData::stringizer;
																			var scaledText = stringizer(cpValue, withUnit);
																			return scaledText;
																			}-*/;

		public double quantize(double v, boolean fine) {
			double steps = fine ? fineDenominator.getValue() : coarseDenominator.getValue();
			steps = steps != 0 ? steps : 1;
			v *= steps;
			v = Math.round(v);
			return v / steps;
		}

		public double clip(double v) {
			if (bipolar.isTrue())
				return NLMath.clamp(v, -1.0, 1.0);
			return NLMath.clamp(v, 0.0, 1.0);
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

	public String getDecoratedValue(boolean withUnit, double value, boolean fine) {
		double quantized = metaData.quantize(value, fine);
		double clipped = metaData.clip(quantized);
		return metaData.getDecoratedValue(withUnit, clipped);
	}

	public double getQuantizedAndClipped(boolean fine) {
		return metaData.clip(metaData.quantize(value.getValue(), fine));
	}

	public double getQuantizedAndClipped(double v, boolean b) {
		return metaData.clip(metaData.quantize(v, b));
	}

	public double getQuantized(boolean fine) {
		return metaData.quantize(value.getValue(), fine);
	}

	public String getDecoratedValue(boolean withUnit, boolean fine) {
		double quantized = metaData.quantize(value.getValue(), fine);
		double clipped = metaData.clip(quantized);
		return metaData.getDecoratedValue(withUnit, clipped);
	}

	public double getClippedValue() {
		return metaData.clip(value.getValue());
	}

	public boolean isValueCoarseQuantized() {
		return getQuantizedAndClipped(false) == getQuantizedAndClipped(true);
	}

	public double getIncDecValue(boolean fine, int inc) {

		if (!fine && !isValueCoarseQuantized()) {
			double fineValue = getQuantizedAndClipped(true);
			double coarseValue = getQuantizedAndClipped(false);

			if (coarseValue < fineValue && inc == -1) {
				inc = 0;
			} else if (coarseValue > fineValue && inc == 1) {
				inc = 0;
			}
		}

		double controlVal = getClippedValue();
		double denominator = fine ? metaData.fineDenominator.getValue() : metaData.coarseDenominator.getValue();
		double unRounded = controlVal * denominator;
		double rounded = Math.round(unRounded);
		return metaData.clip((rounded + inc) / denominator);
	}

}