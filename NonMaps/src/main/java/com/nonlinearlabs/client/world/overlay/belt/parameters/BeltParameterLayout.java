package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.event.dom.client.KeyCodes;
import com.google.gwt.event.dom.client.KeyDownEvent;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.useCases.IncrementalChanger;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.OverlayControl;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.belt.Belt;
import com.nonlinearlabs.client.world.overlay.belt.parameters.recall.MCAmountRecallArea;
import com.nonlinearlabs.client.world.overlay.belt.parameters.recall.MCPositionRecallArea;
import com.nonlinearlabs.client.world.overlay.belt.parameters.recall.ParameterRecallArea;
import com.nonlinearlabs.client.world.overlay.belt.parameters.recall.RecallArea;
import com.nonlinearlabs.client.world.overlay.layouter.HarmonicLayouter;
import com.nonlinearlabs.client.world.pointer.TouchPinch;

public class BeltParameterLayout extends OverlayLayout {

	public enum Mode {
		unmodulateableParameter, modulateableParameter, paramValue, mcValue, mcAmount, mcLower, mcUpper, mcSource
	}

	private Mode mode = Mode.modulateableParameter;
	private IncrementalChanger currentIncrementalChanger;

	private OverlayControl modulationButtons;
	private Sliders slider;
	private OverlayControl mcSourceDisplay;
	private OverlayControl editorMode;
	private OverlayControl valueDisplay;
	private OverlayControl parameterName;
	private OverlayControl dottedLine;
	private OverlayControl infoButton;
	private OverlayControl contextMenu;

	private MCRadioButton mcPositionRadioButton;
	private MCRadioButton mcAmountRadioButton;
	private MCRadioButton mcLowerBoundRadioButton;
	private MCRadioButton mcUpperBoundRadioButton;
	private ParameterClippingLabel mcUpperClip;
	private ParameterClippingLabel mcLowerClip;

	private RecallArea currentRecall;

	public BeltParameterLayout(Belt parent) {
		super(parent);

		addChild(modulationButtons = new ModulationButtons(this));
		addChild(mcSourceDisplay = new MCSourceDisplay(this));
		addChild(slider = new Sliders(this));
		addChild(editorMode = new EditorModeButton(this));
		addChild(valueDisplay = new ValueDisplay(this));
		addChild(parameterName = new CurrentValuesName(this));
		addChild(dottedLine = new DottedLine(this));
		addChild(infoButton = new InfoButton(this));

		addChild(contextMenu = new ContextMenuButton(this));

		addChild(mcPositionRadioButton = new MCRadioButton(this, Mode.mcValue, "MC_Pos_Aktiv.svg",
				"MC_Pos_Inaktiv.svg"));
		addChild(
				mcAmountRadioButton = new MCRadioButton(this, Mode.mcAmount, "MC_Amt_Aktiv.svg", "MC_Amt_Inaktiv.svg"));
		addChild(mcLowerBoundRadioButton = new MCRadioButton(this, Mode.mcLower, "MC_Low_Aktiv.svg",
				"MC_Low_Inaktiv.svg"));
		addChild(mcUpperBoundRadioButton = new MCRadioButton(this, Mode.mcUpper, "MC_High_Aktiv.svg",
				"MC_High_Inaktiv.svg"));

		addChild(mcUpperClip = new ParameterClippingLabel(this, Mode.mcUpper));
		addChild(mcLowerClip = new ParameterClippingLabel(this, Mode.mcLower));

		addChild(currentRecall = new ParameterRecallArea(this));

		EditBufferPresenterProvider.get().onChange(p -> {
			// todo: handle mode change if modulateable / unmodulateable

			return true;
		});
	}

	public Mode getMode() {
		return mode;
	}

	@Override
	public Belt getParent() {
		return (Belt) super.getParent();
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		fixMode();
		super.draw(ctx, invalidationMask);
	}

	private void fixMode() {
		if (!isModulationAssigned()) {
			if (isOneOf(Mode.mcValue, Mode.mcAmount, Mode.mcLower, Mode.mcUpper))
				setMode(Mode.mcSource);

			if (mcPositionRadioButton.isIntendedVisible())
				showAndHideChildren();
		} else {
			if (!mcPositionRadioButton.isIntendedVisible())
				showAndHideChildren();
		}
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		double undoWidth = Millimeter.toPixels(30);
		double undoRedoMargin = Millimeter.toPixels(4.5);

		double third = h / 3.0;
		final double buttonDim = Millimeter.toPixels(10);

		double editorModeLeft = (Millimeter.toPixels(70) / 4) + (buttonDim / 2);
		double modSrcDim = Millimeter.toPixels(10);
		double modulationButtonsDimX = Millimeter.toPixels(25);
		double modulationButtonsDimY = Millimeter.toPixels(25);

		double modulationButtonsLeft = Millimeter.toPixels(19.5 - 22.5 / 2);
		double sliderLeft = modulationButtonsLeft + modulationButtonsDimX + Millimeter.toPixels(10);

		modulationButtons.doLayout(modulationButtonsLeft, (h - modulationButtonsDimY) * 0.5, modulationButtonsDimX,
				modulationButtonsDimY);
		mcSourceDisplay.doLayout(undoRedoMargin + undoWidth * 0.75 - modSrcDim / 2, (h - modSrcDim) / 2, modSrcDim,
				modSrcDim);
		editorMode.doLayout(w - editorModeLeft, (h - buttonDim) / 2, buttonDim, buttonDim);

		double clipW = 20;
		mcLowerClip.doLayout(sliderLeft - clipW, third, clipW, third);
		slider.doLayout(sliderLeft, third, w - sliderLeft - sliderLeft, third);
		mcUpperClip.doLayout(sliderLeft + w - sliderLeft - sliderLeft, third, clipW, third);

		double upperElementsY = Millimeter.toPixels(0.5);

		double sliderWidth = slider.getRelativePosition().getWidth();
		double margin = Millimeter.toPixels(2.5);
		double modulationButtonWidth = 1.5 * mcPositionRadioButton.getSelectedImage().getImgWidth();

		HarmonicLayouter layouter = new HarmonicLayouter();

		layouter.push(null, margin, margin, 0, 0);
		layouter.push(null, 0, sliderWidth, 0, 2);
		layouter.push(mcPositionRadioButton, modulationButtonWidth, modulationButtonWidth, 1, 2);
		layouter.push(null, margin, margin, 0, 2);
		layouter.push(mcAmountRadioButton, modulationButtonWidth, modulationButtonWidth, 1, 2);
		layouter.push(null, margin, margin, 0, 2);
		layouter.push(currentRecall, modulationButtonWidth, modulationButtonWidth, 1, 2);
		layouter.push(valueDisplay, modulationButtonWidth, sliderWidth * 0.7, 2, 1);
		layouter.push(null, modulationButtonWidth, modulationButtonWidth, 0, 2);
		layouter.push(null, margin, margin, 0, 2);
		layouter.push(mcLowerBoundRadioButton, modulationButtonWidth, modulationButtonWidth, 1, 2);
		layouter.push(null, margin, margin, 0, 2);
		layouter.push(mcUpperBoundRadioButton, modulationButtonWidth, modulationButtonWidth, 1, 2);
		layouter.push(null, 0, sliderWidth, 0, 2);
		layouter.push(null, margin, margin, 0, 0);

		double walkerX = sliderLeft;

		double modAndParamValueYValue = h / 2.3;

		for (HarmonicLayouter.Result r : layouter.layout(sliderWidth)) {
			if (r.record.attached != null) {
				OverlayControl c = (OverlayControl) r.record.attached;
				c.doLayout(walkerX, 0, r.width, modAndParamValueYValue);
			}
			walkerX += r.width;
		}

		parameterName.doLayout(sliderLeft, 2 * third - upperElementsY, slider.getRelativePosition().getWidth(), third);

		double dottedLineInset = 5;
		double lineWidth = slider.getRelativePosition().getLeft() - mcSourceDisplay.getRelativePosition().getRight()
				+ dottedLineInset;

		dottedLine.doLayout(mcSourceDisplay.getRelativePosition().getRight() - dottedLineInset, 0, lineWidth, h);
		infoButton.doLayout(undoRedoMargin + undoWidth / 4 - modSrcDim / 2, (h - modSrcDim) / 2, modSrcDim, modSrcDim);
		contextMenu.doLayout(undoRedoMargin + undoWidth * 0.75 - modSrcDim / 2, (h - modSrcDim) / 2, modSrcDim,
				modSrcDim);
	}

	protected void showAndHideChildren() {
		instantiateRecall();

		modulationButtons.setVisible(
				isOneOf(Mode.mcValue, Mode.mcAmount, Mode.mcSource, Mode.mcLower, Mode.mcUpper, Mode.paramValue));
		mcSourceDisplay.setVisible(isOneOf(Mode.modulateableParameter));
		editorMode.setVisible(true);
		slider.setVisible(true);
		parameterName.setVisible(true);

		boolean modAssigned = isModulationAssigned();

		mcPositionRadioButton.setVisible(modAssigned
				&& isOneOf(Mode.mcValue, Mode.mcAmount, Mode.mcSource, Mode.mcLower, Mode.mcUpper, Mode.paramValue));
		mcAmountRadioButton.setVisible(modAssigned
				&& isOneOf(Mode.mcValue, Mode.mcAmount, Mode.mcSource, Mode.mcLower, Mode.mcUpper, Mode.paramValue));
		mcLowerBoundRadioButton.setVisible(modAssigned
				&& isOneOf(Mode.mcValue, Mode.mcAmount, Mode.mcSource, Mode.mcLower, Mode.mcUpper, Mode.paramValue));
		mcUpperBoundRadioButton.setVisible(modAssigned
				&& isOneOf(Mode.mcValue, Mode.mcAmount, Mode.mcSource, Mode.mcLower, Mode.mcUpper, Mode.paramValue));

		valueDisplay.setVisible(isOneOf(Mode.mcValue, Mode.mcAmount, Mode.mcSource, Mode.mcLower, Mode.mcUpper,
				Mode.paramValue, Mode.modulateableParameter, Mode.unmodulateableParameter));

		dottedLine.setVisible(isOneOf(Mode.modulateableParameter));
		infoButton.setVisible(isOneOf(Mode.modulateableParameter, Mode.unmodulateableParameter));

		ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		boolean ctxMenuVisible = isOneOf(Mode.unmodulateableParameter) && p.showContextMenu;

		contextMenu.setVisible(ctxMenuVisible);
		currentRecall.setVisibleForMode(mode);
	}

	private void instantiateRecall() {
		switch (mode) {
		case mcAmount:
			if (!(currentRecall instanceof MCAmountRecallArea)) {
				removeChild(currentRecall);
				addChild(currentRecall = new MCAmountRecallArea(this));
			}
			break;
		case mcValue:
			if (!(currentRecall instanceof MCPositionRecallArea)) {
				removeChild(currentRecall);
				addChild(currentRecall = new MCPositionRecallArea(this));
			}
			break;
		case modulateableParameter:
		case paramValue:
		case unmodulateableParameter:
			if (!(currentRecall instanceof ParameterRecallArea)) {
				removeChild(currentRecall);
				addChild(currentRecall = new ParameterRecallArea(this));
			}
			break;
		case mcLower:
		case mcUpper:
		case mcSource:
		default:
			break;
		}
	}

	protected void toggleMcEditMode(Mode m) {
		if (mode != m)
			setMode(m);
		else
			setMode(Mode.paramValue);
	}

	public void setMode(Mode m) {
		setMode(m, false);
	}

	public void setMode(Mode m, boolean force) {
		if (mode != m || force) {
			mode = m;
			showAndHideChildren();
			requestLayout();
		}
	}

	public boolean isModulationAssigned() {
		ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		return p.modulation.isModulated;
	}

	public boolean isOneOf(Mode... m) {
		return isModeOneOf(mode, m);
	}

	public boolean isModeOneOf(Mode inQuestion, Mode... m) {
		for (Mode u : m)
			if (inQuestion == u)
				return true;

		return false;
	}

	public void setValueChanger(IncrementalChanger changer) {
		currentIncrementalChanger = changer;
	}

	@Override
	public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
		if (currentIncrementalChanger != null) {
			double amount = newPoint.getX() - oldPoint.getX();
			currentIncrementalChanger.changeBy(fine, amount);
		}
		return this;
	}

	@Override
	public Control pinch(Position eventPoint, double touchDist, TouchPinch pinch) {
		if (currentIncrementalChanger != null) {
			double maxDiff = pinch.getMaxTouchDistance();
			currentIncrementalChanger.changeBy(true, maxDiff);
		} else {
			mouseDown(eventPoint);
		}

		return this;
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		startEdit();
		return this;
	}

	public IncrementalChanger startEdit() {
		return startEdit(slider.getParamSliderVisibleWidth());
	}

	public void startEdit(Mode mode, double width) {
		setMode(mode);
		startEdit(width);
	}

	public IncrementalChanger startEdit(double width) {
		ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		switch (mode) {
		case mcAmount:
			currentIncrementalChanger = EditBufferUseCases.get().startEditMCAmount(p.id, width);
			break;

		case unmodulateableParameter:
		case modulateableParameter:
		case paramValue:
		case mcSource:
			currentIncrementalChanger = EditBufferUseCases.get().startEditParameterValue(p.id, width);
			break;

		case mcValue:
			currentIncrementalChanger = EditBufferUseCases.get().startEditMacroControlValue(p.id, width);
			break;

		case mcLower:
			currentIncrementalChanger = EditBufferUseCases.get().startEditModulationAmountLowerBound(p.id, width);
			break;

		case mcUpper:
			currentIncrementalChanger = EditBufferUseCases.get().startEditModulationAmountUpperBound(p.id, width);
			break;
		}
		return currentIncrementalChanger;
	}

	@Override
	public void onMouseLost() {
		if (currentIncrementalChanger != null) {
			currentIncrementalChanger.finish();
			currentIncrementalChanger = null;
		}
		super.onMouseLost();
	}

	@Override
	public Control doubleClick() {
		startEdit().setToDefault();
		return this;
	}

	@Override
	public Control onKey(final KeyDownEvent event) {
		if (event.getNativeKeyCode() == KeyCodes.KEY_K) {
			startEdit().inc(event.isShiftKeyDown());
			return this;
		} else if (event.getNativeKeyCode() == KeyCodes.KEY_M) {
			startEdit().dec(event.isShiftKeyDown());
			return this;
		}

		return null;
	}

	@Override
	public Control wheel(Position eventPoint, double amount, boolean fine) {
		if (amount > 0)
			startEdit().inc(fine);
		else if (amount < 0)
			startEdit().dec(fine);

		return this;
	}

	public String getCurrentValuesName(double width) {
		ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		return p.fullNameWithGroup;
	}

	public String[] getDecoratedValue(boolean b) {
		ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		switch (mode) {
		case mcAmount:
			return p.modulation.amountDisplayValues;

		case mcLower:
			return p.modulation.amountLowerBoundDisplayValues;

		case mcUpper:
			return p.modulation.amountUpperBoundDisplayValues;

		case mcValue:
			return p.modulation.macroControlDisplayValues;

		case mcSource:
		case modulateableParameter:
		case paramValue:
		case unmodulateableParameter:
		default:
			return p.displayValues;
		}
	}

}
