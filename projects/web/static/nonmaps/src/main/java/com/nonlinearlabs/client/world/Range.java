package com.nonlinearlabs.client.world;

public class Range {
	private double left;
	private double right;

	public Range(double left, double right) {
		this.left = left;
		this.right = right;
	}

	public double getLeft() {
		return left;
	}

	public double getRight() {
		return right;
	}

	public boolean contains(double n) {
		return (n >= left && n <= right);
	}

	public boolean outOfRange(double n) {
		return !contains(n);
	}

	public boolean outOfRange(Range r) {
		return r.outOfRange(left) || r.outOfRange(right);
	}

	public double clip(double n) {
		return Math.max(left, Math.min(right, n));
	}

	public void normalize() {
		if (left > right) {
			double tmp = left;
			left = right;
			right = tmp;
		}
	}

	public void clipTo(Range bounds) {
		left = bounds.clip(left);
		right = bounds.clip(right);
	}
}