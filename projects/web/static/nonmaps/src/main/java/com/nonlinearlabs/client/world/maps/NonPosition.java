package com.nonlinearlabs.client.world.maps;

import com.nonlinearlabs.client.Animator;
import com.nonlinearlabs.client.Animator.DoubleClientData.Client;

public class NonPosition {
	private double x = 0;
	private double y = 0;

	public NonPosition() {
	}

	public NonPosition(double x, double y) {
		set(x, y);
	}

	public NonPosition(NonPosition nonPosition) {
		set(nonPosition.getX(), nonPosition.getY());
	}

	public boolean set(double x, double y) {
		boolean changed = setX(x);
		changed |= setY(y);
		return changed;
	}

	public boolean setX(double x) {
		if (this.x != x) {
			this.x = x;
			return true;
		}
		return false;
	}

	boolean setY(double y) {
		if (this.y != y) {
			this.y = y;
			return true;
		}
		return false;
	}

	public double getX() {
		return x;
	}

	public double getY() {
		return y;
	}

	boolean set(NonPosition pos) {
		boolean changed = setX(pos.getX());
		changed |= setY(pos.getY());
		return changed;
	}

	boolean add(NonPosition dist) {
		boolean changed = setX(getX() + dist.getX());
		changed |= setY(getY() + dist.getY());
		return changed;
	}

	public NonPosition copy() {
		return new NonPosition(x, y);
	}

	@Override
	public String toString() {
		return "x=" + x + ", y=" + y;
	}

	public NonPosition getNegated() {
		return new NonPosition(-getX(), -getY());
	}

	public void moveBy(NonDimension dim) {
		setX(getX() + dim.getWidth());
		setY(getY() + dim.getHeight());
	}

	@Override
	public int hashCode() {
		return 1 + 17 * Double.hashCode(getX()) + 31 * Double.hashCode(getY());
	}

	public void animateTo(NonPosition position, Animator theAnimation) {
		theAnimation.addSubAnimation(x, position.getX(), new Client() {

			@Override
			public void animate(double v) {
				x = v;
			}
		});

		theAnimation.addSubAnimation(y, position.getY(), new Client() {

			@Override
			public void animate(double v) {
				y = v;
			}
		});
	}

	public void moveBy(double x, double y) {
		this.x += x;
		this.y += y;
	}

	public void add(NonDimension dist) {
		moveBy(dist.getWidth(), dist.getHeight());
	}

	public void add(double x, double y) {
		this.x += x;
		this.y += y;

	}

	public void snapTo(double gridSize) {
		x = quantize(x, gridSize);
		y = quantize(y, gridSize);
	}

	private double quantize(double v, double gridSize) {
		v = gridSize * (Math.round(v / gridSize));
		return v;
	}

	public double distanceTo(NonPosition other) {
		return Math.hypot(Math.abs(other.y - y), Math.abs(other.x - x));
	}

}
