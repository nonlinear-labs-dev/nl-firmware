package com.nonlinearlabs.NonMaps.client.world;

import com.google.gwt.canvas.dom.client.Context2d;

public class TextCropper {
	public static String crop(String text, Context2d ctx, double maxWidth) {
		int min = 0;
		int max = text.length();

		if (ctx.measureText(text).getWidth() <= maxWidth)
			return text;

		return binarySearchLength(text, min, max, ctx, maxWidth);
	}

	private static String binarySearchLength(String text, int min, int max, Context2d ctx, double maxWidth) {
		if (max == 0)
			return "";

		if ((min + 1) == max)
			return text.substring(0, min) + "..";

		int halfIdx = min + (max - min) / 2;
		String half = text.substring(0, halfIdx);
		String toMeasure = half + "..";

		if (ctx.measureText(toMeasure).getWidth() <= maxWidth)
			min = halfIdx;
		else
			max = halfIdx;

		return binarySearchLength(text, min, max, ctx, maxWidth);
	}

}
