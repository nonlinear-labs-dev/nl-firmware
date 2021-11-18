package com.nonlinearlabs.client.world;

public class RGBA extends RGB {

	private double a;

	public RGBA(int r, int g, int b, double a) {
		super(r, g, b);
		this.a = a;
	}

	public RGBA(RGB rgb, double a) {
		this(rgb.r, rgb.g, rgb.b, a);
	}

	public RGBA clone() {
		return new RGBA(r, g, b, a);
	}

	@Override
	public String toString() {
		return "rgba(" + r + "," + g + "," + b + "," + a + ")";
	}

	public static RGBA redTransparent() {
		return new RGBA(255, 0, 0, 0.5);
	}

	@Override
	public RGBA brighter(int amount) {
		return new RGBA(r + amount, g + amount, b + amount, a);
	}

	@Override
	public RGBA darker(int amount) {
		return brighter(-amount);
	}

	public static RGB transparent() {
		return new RGBA(0, 0, 0, 0);
	}

}
