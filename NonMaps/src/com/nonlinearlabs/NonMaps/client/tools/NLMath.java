package com.nonlinearlabs.NonMaps.client.tools;

public class NLMath {
	public static double clamp(double modRight, double d, double e) {
		return Math.max(d, Math.min(e, modRight));
	}

	public static double quantize(double v, double numSteps) {
		return Math.round(v * numSteps) / numSteps;
	}
}
