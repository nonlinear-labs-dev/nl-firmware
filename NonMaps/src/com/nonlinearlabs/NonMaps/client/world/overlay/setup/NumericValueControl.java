package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.core.client.JavaScriptObject;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter.Initiator;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.value.QuantizedClippedValue;

public abstract class NumericValueControl extends SettingsControl implements QuantizedClippedValue.ChangeListener {

	protected CenteredSetupLabel middle;
	protected QuantizedClippedValue value = new QuantizedClippedValue(this);
	private QuantizedClippedValue.IncrementalChanger changer = null;
	private JavaScriptObject stringizer;

	public NumericValueControl(Control parent, String initText) {
		super(parent);
		addChild(middle = new CenteredSetupLabel(this, initText));
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		boolean drawArrows = middle.getPixRect().getWidth() >= Millimeter.toPixels(50);
		middle.getPixRect().drawValueEditSliderBackgound(ctx, drawArrows, getColorFont());
		super.draw(ctx, invalidationMask);
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double yMargin = h / 4;
		middle.doLayout(0, yMargin, w, h - 2 * yMargin);
	}

	void setText(String txt) {
		middle.setText(txt);
	}

	public void update(Node s) {
		for (Node child = s.getFirstChild(); child != null; child = child.getNextSibling()) {
			value.update(child);

			try {
				if (child.getNodeName().equals("scaling")) {
					createStringizer(ServerProxy.getText(child));
					setText(stringize(true, value.getQuantizedClipped()));
				}
			} catch (Exception e) {
			}
		}
	}

	private native void createStringizer(String body) /*-{
														this.@com.nonlinearlabs.NonMaps.client.world.overlay.setup.NumericValueControl::stringizer = new Function(
														"cpValue", "withUnit", body);
														}-*/;

	private native String stringize(boolean withUnit, double cpValue) /*-{
																		var stringizer = this.@com.nonlinearlabs.NonMaps.client.world.overlay.setup.NumericValueControl::stringizer;
																		var scaledText = stringizer(cpValue, withUnit);
																		return scaledText;
																		}-*/;

	@Override
	public Control mouseDown(Position eventPoint) {
		changer = value.startUserEdit(Millimeter.toPixels(100));
		return this;
	}

	@Override
	public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
		if (changer != null) {
			double xPix = newPoint.getX() - oldPoint.getX();
			double yPix = oldPoint.getY() - newPoint.getY();
			double pix = xPix;

			if (Math.abs(yPix) > Math.abs(xPix))
				pix = yPix;

			changer.changeBy(fine, pix);
		}
		return this;
	}

	@Override
	public void onClippedValueChanged(Initiator initiator, double oldClippedValue, double newClippedValue) {
	}

	@Override
	public void onRawValueChanged(Initiator initiator, double oldRawValue, double newRawValue) {

	}

	@Override
	public void onQuantizedValueChanged(Initiator initiator, double oldQuantizedValue, double newQuantizedValue) {
		setText(stringize(true, newQuantizedValue));

		if (initiator == Initiator.EXPLICIT_USER_ACTION)
			sendToServer(newQuantizedValue);
	}

	protected abstract void sendToServer(double newQuantizedValue);

	@Override
	public Control mouseUp(Position eventPoint) {
		return this;
	}

}