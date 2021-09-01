package com.nonlinearlabs.client.world.maps;

import com.nonlinearlabs.client.Animator;

public class NonRect {
	private final NonPosition position = new NonPosition();
	private final NonDimension dimension = new NonDimension();

	public NonRect() {
	}

	public NonRect(NonPosition p, NonDimension d) {
		position.set(p);
		dimension.set(d);
	}

	public NonRect(double x, double y, double w, double h) {
		position.set(x, y);
		dimension.set(w, h);
	}

	public boolean setPosition(NonPosition pos) {
		return position.set(pos);
	}

	private boolean setDimension(NonDimension dim) {
		return dimension.set(dim);
	}

	public void setDimension(double w, double h) {
		dimension.set(w, h);
	}

	public boolean set(NonPosition pos, NonDimension dim) {
		boolean changed = setPosition(pos);
		changed |= setDimension(dim);
		return changed;
	}

	public NonDimension getDimension() {
		return dimension;
	}

	public NonPosition getPosition() {
		return position;
	}

	public boolean moveTo(NonPosition pos) {
		return position.set(pos);
	}

	public boolean moveBy(NonPosition dist) {
		return position.add(dist);
	}

	public boolean moveTo(double x, double y) {
		return position.set(x, y);
	}

	public double getLeft() {
		return position.getX();
	}

	public double getTop() {
		return position.getY();
	}

	public double getRight() {
		return position.getX() + dimension.getWidth();
	}

	public double getBottom() {
		return position.getY() + dimension.getHeight();
	}

	public double getWidth() {
		return dimension.getWidth();
	}

	public double getHeight() {
		return dimension.getHeight();
	}

	@Override
	public String toString() {
		return "pos=" + position + "; dim=" + dimension;
	}

	public NonPosition getCenterPoint() {
		return new NonPosition(getLeft() + getWidth() / 2, getTop() + getHeight() / 2);
	}

	public void enlargeBy(double amount) {
		getDimension().enlargeBy(amount);
	}

	public void clip(NonDimension min, NonDimension max) {
		getDimension().clip(min, max);
	}

	@Override
	public int hashCode() {
		return 1 + 17 * position.hashCode() + 31 * dimension.hashCode();
	}

	public void set(NonRect non) {
		setPosition(non.getPosition());
		setDimension(non.getDimension());
	}

	public void animateTo(NonRect target, Animator theAnimation) {
		getPosition().animateTo(target.getPosition(), theAnimation);
		getDimension().animateTo(target.getDimension(), theAnimation);
	}

	public NonRect copy() {
		return new NonRect(getPosition(), getDimension());
	}

	public void moveBy(NonDimension dist) {
		position.add(dist);
	}

	public void moveBy(double x, double y) {
		position.add(x, y);
	}

	public boolean contains(NonPosition pos) {
		return getLeft() <= pos.getX() && getRight() >= pos.getX() && getTop() <= pos.getY()
				&& getBottom() >= pos.getY();
	}

	public void enlargeToHeight(double d) {
		NonRect old = copy();
		double factor = d / getHeight();

		getDimension().setHeight(getHeight() * factor);
		getDimension().setWidth(getWidth() * factor);

		double xDiff = (getWidth() - old.getWidth()) / 2;
		double yDiff = (getHeight() - old.getHeight()) / 2;

		getPosition().moveBy(-xDiff, -yDiff);
	}

	public void setLeft(double l) {
		position.setX(l);
	}

	public void setTop(double t) {
		position.setY(t);
	}

	public void setBottom(double b) {
		dimension.setHeight(b - getTop());
	}

	public void setRight(double r) {
		dimension.setWidth(r - getLeft());
	}
}
