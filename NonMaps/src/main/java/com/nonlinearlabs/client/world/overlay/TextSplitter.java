package com.nonlinearlabs.client.world.overlay;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;

import com.google.gwt.canvas.dom.client.Context2d;

public class TextSplitter {
	private LinkedList<String> words;
	private ArrayList<String> lines = new ArrayList<String>();
	private boolean widthToSmall = false;

	public TextSplitter(Context2d ctx, double width, String text) {
		String[] newLined = text.split("\\n");

		for (String line : newLined) {
			words = new LinkedList<String>(Arrays.asList(line.split("\\s")));

			while (!words.isEmpty())
				lines.add(eatWords(ctx, width, words));
		}
	}

	public ArrayList<String> getLines() {
		return lines;
	}

	private String eatWords(Context2d ctx, double width, LinkedList<String> words) {
		String lineText = "";
		double spaceWidth = ctx.measureText(" ").getWidth();

		while (!words.isEmpty()) {
			String word = words.removeFirst();
			double w = ctx.measureText(word).getWidth();
			if (w < width) {
				if (!lineText.isEmpty())
					lineText += " ";
				lineText += word;
				width -= w;
				width -= spaceWidth;
			} else {
				if (lineText.isEmpty()) {
					int half = word.length() / 2;
					if (half == 0) {
						if (!lineText.isEmpty())
							lineText += " ";
						lineText += word;
						width -= w;
						width -= spaceWidth;
					} else {
						String first = word.substring(0, half);
						String second = word.substring(half);
						words.addFirst(second);
						words.addFirst(first);
						widthToSmall = true;
					}
				} else {
					words.addFirst(word);
					break;
				}
			}
		}

		return lineText;
	}

	public boolean widthToSmall() {
		return widthToSmall;
	}
}
