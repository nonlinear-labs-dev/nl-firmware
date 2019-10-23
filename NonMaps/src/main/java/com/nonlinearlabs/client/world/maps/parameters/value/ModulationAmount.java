package com.nonlinearlabs.client.world.maps.parameters.value;

import com.google.gwt.core.client.JavaScriptObject;

public class ModulationAmount extends QuantizedClippedValue {

	private JavaScriptObject stringizer;

	public ModulationAmount(ChangeListener listener) {
		super(listener);
		setBipolar(true);
	}

	public String getDecoratedValue(boolean withUnit) {
		return getDecoratedValue(withUnit, getQuantizedClipped());
	}

	public String getDecoratedValue(boolean withUnit, double cpValue) {
		if (stringizer == null)
			return defaultStringize(withUnit, cpValue * 100);

		return stringize(withUnit, cpValue);
	}

	private native String defaultStringize(boolean withUnit, double value) /*-{
																			var intermediate = Number(value).toPrecision(3);
																			var ret = parseFloat(intermediate);
																			
																			if (withUnit)
																			return ret + "%";
																			
																			return ret + "";
																			}-*/;

	public void setStringizer(String txt) {
		if (stringizer == null)
			createStringizer(txt);
	}

	private native void createStringizer(String body) /*-{
														this.@com.nonlinearlabs.client.world.maps.parameters.value.ModulationAmount::stringizer = new Function(
														"cpValue", "withUnit", body);
														}-*/;

	private native String stringize(boolean withUnit, double cpValue) /*-{
																		var stringizer = this.@com.nonlinearlabs.client.world.maps.parameters.value.ModulationAmount::stringizer;
																		var scaledText = stringizer(cpValue, withUnit);
																		return scaledText;
																		}-*/;
}
