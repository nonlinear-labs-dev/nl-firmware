package com.nonlinearlabs.NonMaps.client.tools;

public class NLMath {
	public static float clamp(float val, float min, float max) {
	    return Math.max(min, Math.min(max, val));
	}
}
