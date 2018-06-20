package com.nonlinearlabs.NonMaps.client.world;

public class RGB {

	int r;
	int g;
	int b;

	public RGB(int r, int g, int b) {
		this.r = clip(r);
		this.g = clip(g);
		this.b = clip(b);
	}

	private int clip(int in) {
		return Math.max(0, Math.min(255, in));
	}

	public String toString() {
		return "rgb(" + r + "," + g + "," + b + ")";
	}

	public RGB brighter(int amount) {
		return new RGB(r + amount, g + amount, b + amount);
	}

	public RGB darker(int amount) {
		return brighter(-amount);
	}

	public static RGB blue() {
		return new RGB(107, 115, 153);
	}

	public static RGB black() {
		return new RGB(0, 0, 0);
	}

	public static RGB red() {
		return new RGB(255, 0, 0);
	}

	public static RGB lightGray() {
		return new RGB(155, 159, 163);
	}

	public static RGB floatingWindowHeaderBorder() {
		return new RGB(118, 130, 154);
	}

	public static RGB floatingWindowHeaderBackground() {
		return new RGB(22, 22, 30);
	}
}
