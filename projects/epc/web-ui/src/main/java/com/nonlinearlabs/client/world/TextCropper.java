package com.nonlinearlabs.client.world;

import com.google.gwt.canvas.dom.client.Context2d;

public class TextCropper {
	public static String crop(String text, Context2d ctx, double maxWidth) {
		int min = 0;
		int max = text.length();

		if (ctx.measureText(text).getWidth() <= maxWidth)
			return text;

		return customAlgorithmAlaJustus(text, min, max, ctx, maxWidth);
	}

	private static String customAlgorithmAlaJustus(String text, int min, int textLen, Context2d ctx, double maxWidth) {
		String edgeCaseString = edgeCases(min, textLen, text);
		if (edgeCaseString != null) {
			return edgeCaseString;
		}

		String spaceSplitString = spaceBasedCropping(text, textLen, ctx, maxWidth * 0.95);
		if (spaceSplitString != null) {
			return spaceSplitString;
		}

		final String padding = "..";
		StringBuilder front = new StringBuilder();
		int i = 0;
		while (ctx.measureText(front.toString() + padding).getWidth() < maxWidth / 2) {
			front.append(text.charAt(i++));
		}
		StringBuilder back = new StringBuilder();
		i = text.length() - 1;
		while (ctx.measureText(back.toString() + padding).getWidth() < maxWidth / 2) {
			back.insert(0, text.charAt(i--));
		}

		String ret = front.toString() + ".." + back.toString();
		return ret;
	}

	private static String edgeCases(int min, int max, String text) {
		if (max == 0)
			return "";
		if (text.length() < min)
			return text;
		if ((min + 1) == max)
			return text.substring(0, min) + "..";
		return null;
	}

	private static String spaceBasedCropping(String text, int max, Context2d ctx, double maxWidth) {

		int lastSpace = text.lastIndexOf(" ");

		if (lastSpace >= (text.length() / 3) * 2) {
			String partAfterSpace = text.substring(lastSpace + 1);

			if (ctx.measureText(partAfterSpace).getWidth() >= maxWidth / 2)
				return null;

			StringBuilder front = new StringBuilder();
			for (int i = 0; ctx.measureText(front + partAfterSpace + "..").getWidth() < maxWidth; i++) {
				front.append(text.charAt(i));
			}
			return front + ".." + partAfterSpace;
		}
		return null;
	}
}
