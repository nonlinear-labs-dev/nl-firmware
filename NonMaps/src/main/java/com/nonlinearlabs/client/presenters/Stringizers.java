package com.nonlinearlabs.client.presenters;

import java.util.HashMap;

import com.google.gwt.core.client.JavaScriptObject;

public class Stringizers {
	public static Stringizers theInstance = new Stringizers();

	public static Stringizers get() {
		return theInstance;
	}

	private HashMap<String, JavaScriptObject> stringizers = new HashMap<String, JavaScriptObject>();
	private JavaScriptObject tmpStringizer;

	private native void createStringizer(String body) /*-{
														this.@com.nonlinearlabs.client.presenters.Stringizers::tmpStringizer = new Function(
														"cpValue", "withUnit", body);
														}-*/;

	private native String stringize(double cpValue) /*-{
													var stringizer = this.@com.nonlinearlabs.client.presenters.Stringizers::tmpStringizer;
													var scaledText = stringizer(cpValue, true);
													return scaledText;
													}-*/;

	public void registerStringizer(String value) {
		if (value != null && !value.isEmpty() && !stringizers.containsKey(value)) {
			createStringizer(value);
			stringizers.put(value, tmpStringizer);
		}
	}

	public String stringize(String stringizer, double v) {
		registerStringizer(stringizer);
		tmpStringizer = stringizers.get(stringizer);
		if (tmpStringizer == null)
			return "---";

		return stringize(v);
	}
}
