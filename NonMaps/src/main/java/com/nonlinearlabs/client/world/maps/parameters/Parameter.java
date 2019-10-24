package com.nonlinearlabs.client.world.maps.parameters;

import java.util.Arrays;
import java.util.List;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.Checksum;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.Tracer;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.setup.Setup;
import com.nonlinearlabs.client.dataModel.setup.Setup.BooleanValues;
import com.nonlinearlabs.client.dataModel.setup.Setup.EditParameter;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.useCases.IncrementalChanger;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.overlay.ContextMenu;
import com.nonlinearlabs.client.world.overlay.Overlay;
import com.nonlinearlabs.client.world.overlay.belt.parameters.ParameterContextMenu;
import com.nonlinearlabs.client.world.pointer.TouchPinch;

public abstract class Parameter extends LayoutResizingVertical {

	public enum Initiator {
		EXPLICIT_USER_ACTION, INDIRECT_USER_ACTION, MODULATION
	}

	private int parameterID;
	protected IncrementalChanger currentParameterChanger = null;

	public Parameter(MapsLayout parent, int id) {
		super(parent);
		parameterID = id;
	}

	@Override
	public void getStateHash(Checksum crc) {
		super.getStateHash(crc);
		crc.eat(isSelected());
		crc.eat(getParameterID());
		crc.eat(Setup.get().systemSettings.highlightChangedParameters.getValue().toString());
		crc.eat(Setup.get().systemSettings.forceHighlightChangedParameters.getValue().toString());
		crc.eat(getParameterModel().isChanged());
		getParameterModel().getHash(crc);
	}

	BasicParameterModel getParameterModel() {
		return EditBufferModel.findParameter(getParameterID());
	}

	public final int getParameterID() {
		return parameterID;
	}

	static private List<Integer> HardwareSourceIDS() {
		return Arrays.asList(254, 259, 264, 269, 274, 279, 284, 289);
	}

	private boolean shouldHightlightChanged() {
		BasicParameterModel bpm = EditBufferModel.findParameter(getParameterID());
		boolean highlight = Setup.get().systemSettings.highlightChangedParameters.isTrue();
		boolean forceHighlight = Setup.get().systemSettings.forceHighlightChangedParameters.isTrue();

		if (HardwareSourceIDS().contains(getParameterID()))
			return false;

		if (!bpm.isChanged())
			return false;

		return highlight || forceHighlight;

	}

	protected RGB getRoundingColor() {
		if (isSelected()) {
			return getColorSliderHighlight();
		} else if (shouldHightlightChanged()) {
			return RGB.changedBorder();
		}
		return null;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {

		getPixRect().drawRoundedRect(ctx, getBackgroundRoundings(), toXPixels(4), 0, getParameterBackgroundColor(),
				null);

		super.draw(ctx, invalidationMask);

		RGB c = getRoundingColor();
		if (c != null) {
			getPixRect().drawRoundedRect(ctx, getBackgroundRoundings(), toXPixels(4), toXPixels(1), null, c);
		}
	}

	private RGB getParameterBackgroundColor() {
		return isSelected() ? getColorObjectBackgroundSelected() : new Gray(52);
	}

	protected int getBackgroundRoundings() {
		int numChildrenInParent = getParent().getChildren().size();

		if (numChildrenInParent == 1)
			return Rect.ROUNDING_ALL;

		if (getParent().getChildren().get(0) == this)
			return Rect.ROUNDING_TOP;

		if (getParent().getChildren().get(numChildrenInParent - 1) == this)
			return Rect.ROUNDING_BOTTOM;

		return Rect.ROUNDING_NONE;
	}

	public void setDefault() {
		EditBufferUseCases.get().setToDefault(getParameterID());
	}

	public void select(Initiator initiator) {
		getSelectionRoot().select(initiator, this);
	}

	public boolean isSelected() {
		return getSelectionRoot().getSelection() == this;
	}

	@Override
	public Control click(Position eventPoint) {
		boolean isAnyChildVisible = isAnyChildVisible();

		if (!isAnyChildVisible)
			return null;

		if (!isSelected())
			select(Initiator.EXPLICIT_USER_ACTION);

		return this;
	}

	@Override
	public Control doubleClick() {
		setDefault();
		return this;
	}

	@Override
	public Control mouseDown(Position eventPoint) {

		switch (Setup.get().localSettings.editParameter.getValue()) {
		case always:
			select(Initiator.EXPLICIT_USER_ACTION);

			if (isBoolean())
				toggleBoolean();
			else
				startMouseEdit();
			return this;

		case if_selected:
			if (isSelected()) {
				if (isBoolean())
					toggleBoolean();
				else
					startMouseEdit();
				return this;
			}

			break;

		case never:
			break;

		default:
			break;
		}

		return super.mouseDown(eventPoint);
	}

	private void startMouseEdit() {
		currentParameterChanger = EditBufferUseCases.get().startUserEdit(getParameterID(), Millimeter.toPixels(100));
	}

	@Override
	public boolean wantsMouseUpAnimation() {
		return false;
	}

	@Override
	public Control pinch(Position eventPoint, double touchDist, TouchPinch pinch) {
		if (isSelected()) {
			Tracer.log("Parameter.pinch");
			return NonMaps.theMaps.getNonLinearWorld();
		}
		return super.pinch(eventPoint, touchDist, pinch);
	}

	@Override
	public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
		boolean noDrag = (Setup.get().localSettings.editParameter.getValue() == EditParameter.never)
				|| getWorld().isSpaceDown();

		if (isSelected() && !noDrag) {

			double xPix = newPoint.getX() - oldPoint.getX();
			double yPix = oldPoint.getY() - newPoint.getY();
			double pix = xPix;

			if (Math.abs(yPix) > Math.abs(xPix))
				pix = yPix;

			if (currentParameterChanger != null)
				currentParameterChanger.changeBy(fine, pix);

			return this;
		} else if (noDrag) {
			return getWorld().mouseDrag(oldPoint, newPoint, fine);
		}

		return null;
	}

	@Override
	public void onMouseLost() {
		if (currentParameterChanger != null) {
			currentParameterChanger.finish();
			currentParameterChanger = null;
		}
		super.onMouseLost();
	}

	private boolean isAnyChildVisible() {
		boolean isAnyChildVisible = false;

		for (MapsControl c : getChildren()) {
			isAnyChildVisible |= c.isVisible();

			if (isAnyChildVisible)
				break;
		}
		return isAnyChildVisible;
	}

	protected ParameterEditor getSelectionRoot() {
		return ParameterEditor.get();
	}

	public void inc(boolean fine) {
		EditBufferUseCases.get().incParameter(getParameterID(), fine);
	}

	public void dec(boolean fine) {
		EditBufferUseCases.get().decParameter(getParameterID(), fine);
	}

	public String getParameterGroupID() {
		return getParameterGroup().getID();
	}

	public boolean isBoolean() {
		return getParameterModel().value.metaData.isBoolean.getBool();
	}

	private void toggleBoolean() {
		EditBufferUseCases.get().toggleBoolean(getParameterID());
	}

	public boolean shouldHaveHandleOnly() {
		return false;
	}

	public ContextMenu createContextMenu(Overlay o) {
		return new ParameterContextMenu(o, this);
	}

	public boolean hasContextMenu() {
		return true;
	}

	public boolean dimHandleAtDefaultValue() {
		return false;
	}

	public boolean isOracle() {
		return true;
	}

	public static Parameter findInParents(MapsControl c) {
		if (c instanceof Parameter)
			return (Parameter) c;

		return findInParents(c.getParent());
	}

	public ParameterGroupIface getParameterGroup() {
		Control p = getParent();
		while (p != null) {
			if (p instanceof ParameterGroupIface)
				return (ParameterGroupIface) p;

			p = p.getParent();
		}
		return null;
	}

	@Override
	public Control onContextMenu(Position pos) {
		select(Initiator.EXPLICIT_USER_ACTION);
		boolean showContextMenus = Setup.get().localSettings.contextMenus.getValue() == BooleanValues.on;

		if (showContextMenus) {
			if (hasContextMenu()) {
				Overlay o = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay();
				return o.setContextMenu(pos, createContextMenu(o));
			}
		}
		return super.onContextMenu(pos);
	}

	public String getFullNameWithGroup() {
		BasicParameterModel bpm = EditBufferModel.findParameter(getParameterID());
		return getGroupName() + "   \u2013   " + bpm.longName.getValue() + (bpm.isChanged() ? " *" : "");
	}

	public String getGroupName() {
		ParameterGroupIface group = (ParameterGroupIface) getParameterGroup();
		return group.getName().getLongName();
	}

	public boolean isLocked() {
		return getParameterModel().isLocked();
	}

}
