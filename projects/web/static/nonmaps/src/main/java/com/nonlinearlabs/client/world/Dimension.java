package com.nonlinearlabs.client.world;

public class Dimension {
	private double width;
	private double height;

	public Dimension() {
	}

	public Dimension(double width, double height) {
		setWidth(width);
		setHeight(height);
	}

	public boolean setHeight(double height) {
		if (this.height != height) {
			this.height = height;
			return true;
		}
		return false;
	}

	public boolean setWidth(double width) {
		if (this.width != width) {
			this.width = width;
			return true;
		}
		return false;
	}

	public double getWidth() {
		return width;
	}

	public double getHeight() {
		return height;
	}

	public void set(Dimension dimension) {
		this.width = dimension.width;
		this.height = dimension.height;
	}

	@Override
	public String toString() {
		return "width=" + width + ", height=" + height;
	}

	public void set(double w, double h) {
		this.width = w;
		this.height = h;
	}

	public void makeNonNegative() {
		this.width = Math.max(this.width, 0);
		this.height = Math.max(this.height, 0);
	}

	Dimension copy() {
		return new Dimension(width, height);
	}

	void ceil() {
		width = Math.ceil(width);
		height = Math.ceil(height);
	}

	public void add(double x, double y) {
		width += x;
		height += y;
	}

	public void round() {
		width = Math.round(width);
		height = Math.round(height);
	}

	public void add(Dimension dim) {
		width += dim.width;
		height += dim.height;
	}

	public void negate() {
		width *= -1;
		height *= -1;
	}
}
