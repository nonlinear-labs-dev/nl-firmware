package com.nonlinearlabs.client.tools;

public class NLMath {
	public static double clamp(double v, double min, double max) {
		return Math.max(min, Math.min(max, v));
	}

	public static double quantize(double v, double numSteps) {
		return Math.round(v * numSteps) / numSteps;
	}
}
