package com.nonlinearlabs.NonMaps.client.world.maps;

import com.nonlinearlabs.NonMaps.client.Animator;
import com.nonlinearlabs.NonMaps.client.Animator.DoubleClientData.Client;

public class NonDimension {
	private double width = 0;
	private double height = 0;

	public NonDimension() {
	}

	public NonDimension(double width, double height) {
		set(width, height);
	}

	boolean set(double width, double height) {
		boolean changed = setWidth(width);
		changed |= setHeight(height);
		return changed;
	}

	public boolean setWidth(double width) {
		if (this.width != width) {
			this.width = width;
			return true;
		}
		return false;
	}

	public boolean setHeight(double height) {
		if (this.height != height) {
			this.height = height;
			return true;
		}
		return false;
	}

	boolean set(NonDimension dim) {
		return set(dim.getWidth(), dim.getHeight());
	}

	public double getHeight() {
		return height;
	}

	public double getWidth() {
		return width;
	}

	@Override
	public String toString() {
		return "width=" + width + ", height=" + height;
	}

	void clip(NonDimension min, NonDimension max) {
		width = Math.min(max.getWidth(), Math.max(width, min.getWidth()));
		height = Math.min(max.getHeight(), Math.max(height, min.getHeight()));
	}

	public NonPosition getNegated() {
		return new NonPosition(-getWidth(), -getHeight());
	}

	void enlargeBy(double amount) {
		double w = getWidth();
		double h = getHeight();
		double newW = w + amount;
		double newH = h * newW / w;
		set(newW, newH);
	}

	@Override
	public int hashCode() {
		return 1 + 17 * Double.hashCode(getWidth()) + 31 * Double.hashCode(getHeight());
	}

	public void animateTo(NonDimension dimension, Animator theAnimation) {
		theAnimation.addSubAnimation(width, dimension.getWidth(), new Client() {

			@Override
			public void animate(double v) {
				width = v;
			}
		});

		theAnimation.addSubAnimation(height, dimension.getHeight(), new Client() {

			@Override
			public void animate(double v) {
				height = v;
			}
		});

	}
}
