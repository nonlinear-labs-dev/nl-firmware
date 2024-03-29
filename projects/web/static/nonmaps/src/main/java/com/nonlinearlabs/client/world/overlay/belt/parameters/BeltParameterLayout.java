package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.event.dom.client.KeyCodes;
import com.google.gwt.event.dom.client.KeyDownEvent;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel.ModSource;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.useCases.IncrementalChanger;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.overlay.Label;
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

	int lastSelectedParameterNumber = -1;

	private final class MCUpperBoundButton extends MCRadioButton {
		private MCUpperBoundButton(BeltParameterLayout parent) {
			super(parent, Mode.mcUpper, new String[] { "MC_High_Aktiv.svg", "MC_High_Inaktiv.svg" });
		}

		@Override
		public boolean isChanged() {
			return EditBufferPresenterProvider.getPresenter().selectedParameter.modulation.isModAmountChanged;
		}
	}

	private final class MCLowerBoundButton extends MCRadioButton {
		private MCLowerBoundButton(BeltParameterLayout parent) {
			super(parent, Mode.mcLower, new String[] { "MC_Low_Aktiv.svg", "MC_Low_Inaktiv.svg" });
		}

		@Override
		public boolean isChanged() {
			return EditBufferPresenterProvider.getPresenter().selectedParameter.modulation.isModAmountChanged;
		}
	}

	private final class MCAmountButton extends MCRadioButton {
		private MCAmountButton(BeltParameterLayout parent) {
			super(parent, Mode.mcAmount, new String[] { "MC_Amt_Aktiv.svg", "MC_Amt_Inaktiv.svg" });
		}

		@Override
		public boolean isChanged() {
			return EditBufferPresenterProvider.getPresenter().selectedParameter.modulation.isModAmountChanged;
		}
	}

	private final class MCPosButton extends MCRadioButton {
		private MCPosButton(BeltParameterLayout parent) {
			super(parent, Mode.mcValue, new String[] { "MC_Pos_Aktiv.svg", "MC_Pos_Inaktiv.svg" });
		}

		@Override
		public boolean isChanged() {
			return EditBufferPresenterProvider.getPresenter().selectedParameter.modulation.isMCPosChanged;
		}
	}

	public enum Mode {
		unmodulateableParameter, modulateableParameter, paramValue, mcValue, mcAmount, mcLower, mcUpper, mcSource;

		public boolean isOneOf(final Mode... m) {
			for (Mode v : m)
				if (this == v)
					return true;

			return false;
		}

	}

	private final class ThisParameterIsDisabledInfoText extends Label {

		public ThisParameterIsDisabledInfoText(OverlayLayout parent) {
			super(parent);
		}

		@Override
		public String getDrawText(Context2d ctx) {
			return "Only available with Layer Sounds";
		}

		@Override
		protected double getFontHeight(Rect pixRect) {
			return Millimeter.toPixels(8);
		}

	}

	private Mode mode = Mode.modulateableParameter;
	private IncrementalChanger currentIncrementalChanger;

	private OverlayControl modulationButtons;
	private Sliders slider;
	private OverlayControl thisParameterIsDisabled;
	private OverlayControl mcSourceDisplay;
	private OverlayControl editorMode;
	private OverlayControl valueDisplay;
	private SplitValueDisplay splitValueDisplay;
	private CurrentValuesName parameterName;
	private OverlayControl dottedLine;
	private OverlayControl infoButton;
	private OverlayControl contextMenu;
	private OverlayControl ccDisplay;

	private MCRadioButton mcPositionRadioButton;
	private MCRadioButton mcAmountRadioButton;
	private MCRadioButton mcLowerBoundRadioButton;
	private MCRadioButton mcUpperBoundRadioButton;
	private ParameterClippingLabel mcUpperClip;
	private ParameterClippingLabel mcLowerClip;

	private RecallArea currentRecall;
	private SyncParameterButton syncSplitParameter;

	private FineButton fineButton;

	public BeltParameterLayout(final Belt parent) {
		super(parent);

		addChild(modulationButtons = new ModulationButtons(this));
		addChild(mcSourceDisplay = new MCSourceDisplay(this));
		addChild(slider = new Sliders(this));
		addChild(editorMode = new EditorModeButton(this));
		addChild(valueDisplay = new ValueDisplay(this));
		addChild(splitValueDisplay = new SplitValueDisplay(this));
		addChild(parameterName = new CurrentValuesName(this));
		addChild(dottedLine = new DottedLine(this));
		addChild(infoButton = new InfoButton(this));

		addChild(contextMenu = new ContextMenuButton(this));

		addChild(mcPositionRadioButton = new MCPosButton(this));
		addChild(mcAmountRadioButton = new MCAmountButton(this));
		addChild(mcLowerBoundRadioButton = new MCLowerBoundButton(this));
		addChild(mcUpperBoundRadioButton = new MCUpperBoundButton(this));

		addChild(thisParameterIsDisabled = new ThisParameterIsDisabledInfoText(this));

		addChild(mcUpperClip = new ParameterClippingLabel(this, Mode.mcUpper));
		addChild(mcLowerClip = new ParameterClippingLabel(this, Mode.mcLower));
		addChild(currentRecall = new ParameterRecallArea(this));
		addChild(syncSplitParameter = new SyncParameterButton(this));

		addChild(ccDisplay = new SendCCDisplay(this));
		addChild(fineButton = new FineButton(this));

		EditBufferPresenterProvider.get().onChange(p -> {
			if (p.selectedParameter.id.getNumber() != lastSelectedParameterNumber) {
				lastSelectedParameterNumber = p.selectedParameter.id.getNumber();

				if (p.selectedParameter.modulation.isModulateable)
					setMode(Mode.modulateableParameter);
				else
					setMode(Mode.unmodulateableParameter);
			} else {
				fixMode();
			}

			showAndHideChildren();
			return true;
		});

		SetupModel.get().systemSettings.syncSplit.onChange(b -> {
			showAndHideChildren();
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
	public void draw(final Context2d ctx, final int invalidationMask) {
		fixMode();
		super.draw(ctx, invalidationMask);
	}

	private void fixMode() {

		if (isParameterDisabled() != thisParameterIsDisabled.isVisible()) {
			showAndHideChildren();
		}

		if (!isModulateable()) {
			setMode(Mode.unmodulateableParameter);
			return;
		}

		if (isModulateable() && mode == Mode.unmodulateableParameter) {
			setMode(Mode.modulateableParameter);
			return;
		}

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
	public void doLayout(final double x, final double y, final double w, final double h) {
		super.doLayout(x, y, w, h);

		final double undoWidth = Millimeter.toPixels(30);
		final double undoRedoMargin = Millimeter.toPixels(4.5);

		final double half = h / 2.0;
		final double third = h / 3.0;
		final double buttonDim = Millimeter.toPixels(10);

		final double editorModeLeft = (Millimeter.toPixels(70) / 4) + (buttonDim / 2);
		final double modSrcDim = Millimeter.toPixels(10);
		final double modulationButtonsDimX = Millimeter.toPixels(25);
		final double modulationButtonsDimY = h - Millimeter.toPixels(3);

		final double modulationButtonsLeft = Millimeter.toPixels(19.5 - 22.5 / 2);
		final double sliderLeft = modulationButtonsLeft + modulationButtonsDimX + Millimeter.toPixels(10);

		modulationButtons.doLayout(modulationButtonsLeft, (h - modulationButtonsDimY) * 0.5, modulationButtonsDimX,
				modulationButtonsDimY);

		mcSourceDisplay.doLayout(undoRedoMargin + undoWidth * 0.75 - modSrcDim / 2, (h - modSrcDim) / 2, modSrcDim,
				modSrcDim);

		if (isParameterSplitPoint()) {
			editorMode.doLayout(w - editorModeLeft - (buttonDim * 0.5), (h - buttonDim) / 2, buttonDim, buttonDim);
		} else {
			editorMode.doLayout(w - editorModeLeft, (h - buttonDim) / 2, buttonDim, buttonDim);
		}

		final double clipW = 20;
		mcLowerClip.doLayout(sliderLeft - clipW, third, clipW, third);
		slider.doLayout(sliderLeft, third, w - sliderLeft - sliderLeft, third);
		mcUpperClip.doLayout(sliderLeft + w - sliderLeft - sliderLeft, third, clipW, third);

		double ccW = Millimeter.toPixels(35);
		double ccHeight = third / 2;
		ccDisplay.doLayout(sliderLeft, third - ccHeight, ccW, ccHeight);

		final double upperElementsY = Millimeter.toPixels(0.5);

		final double sliderWidth = slider.getRelativePosition().getWidth();
		final double margin = Millimeter.toPixels(2.5);
		final double modulationButtonWidth = 1.5 * mcPositionRadioButton.getSelectedImage().getImgWidth();

		final HarmonicLayouter layouter = new HarmonicLayouter();

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

		final double modAndParamValueYValue = h / 2.3;

		for (final HarmonicLayouter.Result r : layouter.layout(sliderWidth)) {
			if (r.record.attached != null) {
				final OverlayControl c = (OverlayControl) r.record.attached;
				c.doLayout(walkerX, 0, r.width, modAndParamValueYValue);
				if (c == valueDisplay) {
					splitValueDisplay.doLayout(walkerX, 0, r.width, modAndParamValueYValue);
					syncSplitParameter.doLayout(walkerX + r.width,
							splitValueDisplay.getRelativePosition().getCenterPoint().getY() - (buttonDim / 1.8),
							buttonDim, buttonDim);
				}
			}
			walkerX += r.width;
		}

		thisParameterIsDisabled.doLayout(sliderLeft, third - upperElementsY, slider.getRelativePosition().getWidth(),
				third);

		parameterName.doLayout(sliderLeft, 2 * third - upperElementsY, slider.getRelativePosition().getWidth(), third);

		final double dottedLineInset = 5;
		final double lineWidth = slider.getRelativePosition().getLeft()
				- mcSourceDisplay.getRelativePosition().getRight() + dottedLineInset;

		dottedLine.doLayout(mcSourceDisplay.getRelativePosition().getRight() - dottedLineInset, 0, lineWidth, h);
		infoButton.doLayout(undoRedoMargin + undoWidth / 4 - modSrcDim / 2, (h - modSrcDim) / 2, modSrcDim, modSrcDim);
		
		double fineHeight = fineButton.getPictureHeight();
		double fineWidth = fineButton.getPictureWidth();
		double quarterWidthOfName = parameterName.getRelativePosition().getWidth() / 4;
		double nameStart = parameterName.getPositionRelativeToParent(this).getLeft();
		
		fineButton.doLayout(nameStart + quarterWidthOfName - fineWidth, parameterName.getPositionRelativeToParent(this).getCenterPoint().getY() - (fineHeight / 2), fineWidth, fineHeight);
		fineButton.setVisible(currentRecall.isVisible());

		contextMenu.doLayout(undoRedoMargin + undoWidth * 0.75 - modSrcDim / 2, (h - modSrcDim) / 2, modSrcDim,
				modSrcDim);

		ccDisplay.setVisible(!ccDisplay.getPixRect().intersects(valueDisplay.getPixRect()));
	}

	protected void showAndHideChildren() {
		instantiateRecall();

		final boolean isEnabled = !isParameterDisabled();

		modulationButtons.setVisible(
				isOneOf(Mode.mcValue, Mode.mcAmount, Mode.mcSource, Mode.mcLower, Mode.mcUpper, Mode.paramValue)
						&& isEnabled);
		mcSourceDisplay.setVisible(isOneOf(Mode.modulateableParameter) && isEnabled);
		editorMode.setVisible(isEnabled);
		slider.setVisible(isEnabled);
		thisParameterIsDisabled.setVisible(!isEnabled);
		parameterName.setVisible(true);

		final boolean modAssigned = isModulationAssigned();

		mcPositionRadioButton.setVisible(modAssigned
				&& isOneOf(Mode.mcValue, Mode.mcAmount, Mode.mcSource, Mode.mcLower, Mode.mcUpper, Mode.paramValue)
				&& isEnabled);
		mcAmountRadioButton.setVisible(modAssigned
				&& isOneOf(Mode.mcValue, Mode.mcAmount, Mode.mcSource, Mode.mcLower, Mode.mcUpper, Mode.paramValue)
				&& isEnabled);
		mcLowerBoundRadioButton.setVisible(modAssigned
				&& isOneOf(Mode.mcValue, Mode.mcAmount, Mode.mcSource, Mode.mcLower, Mode.mcUpper, Mode.paramValue)
				&& isEnabled);
		mcUpperBoundRadioButton.setVisible(modAssigned
				&& isOneOf(Mode.mcValue, Mode.mcAmount, Mode.mcSource, Mode.mcLower, Mode.mcUpper, Mode.paramValue)
				&& isEnabled);

		boolean isSplitPoint = isParameterSplitPoint();
		boolean valueDisplayEnabled = isOneOf(Mode.mcValue, Mode.mcAmount, Mode.mcSource, Mode.mcLower, Mode.mcUpper,
				Mode.paramValue, Mode.modulateableParameter, Mode.unmodulateableParameter) && isEnabled;

		boolean dualSplitPointDisplay = isSplitPoint && isSyncDisabled();
		boolean isInModAspect = isOneOf(Mode.mcValue, Mode.mcAmount, Mode.mcSource, Mode.mcUpper, Mode.mcLower);
		syncSplitParameter.setVisible(isSplitPoint);
		splitValueDisplay.setVisible(dualSplitPointDisplay && valueDisplayEnabled && !isInModAspect);
		valueDisplay.setVisible((!dualSplitPointDisplay && valueDisplayEnabled) || (dualSplitPointDisplay && isInModAspect));

		dottedLine.setVisible(isOneOf(Mode.modulateableParameter) && isEnabled);
		infoButton.setVisible(isOneOf(Mode.modulateableParameter, Mode.unmodulateableParameter));

		final ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		final boolean ctxMenuVisible = isOneOf(Mode.unmodulateableParameter) && p.showContextMenu;

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

	protected void toggleMcEditMode(final Mode m) {
		if (mode != m)
			setMode(m);
		else
			setMode(Mode.paramValue);
	}

	public void setMode(final Mode m) {
		setMode(m, false);
	}

	public void setMode(final Mode m, final boolean force) {
		if (mode != m || force) {
			mode = m;
			showAndHideChildren();
			requestLayout();
		}
	}

	public boolean isParameterSplitPoint() {
		final ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		return p.id.getNumber() == 356;
	}

	public boolean isSyncDisabled() {
		return !SetupModel.get().systemSettings.syncSplit.getBool();
	}

	public boolean isModulationAssigned() {
		final ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		return p.modulation.isModulated;
	}

	public boolean isParameterDisabled() {
		final ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		return p.disabled;
	}

	public boolean isParameterHidden() {
		final ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		return p.disabled;
	}

	public boolean isModulateable() {
		final ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		return p.modulation.isModulateable;
	}

	public boolean isOneOf(final Mode... m) {
		return isModeOneOf(mode, m);
	}

	public boolean isModeOneOf(final Mode inQuestion, final Mode... m) {
		for (final Mode u : m)
			if (inQuestion == u)
				return true;

		return false;
	}

	public void setValueChanger(final IncrementalChanger changer) {
		currentIncrementalChanger = changer;
	}

	@Override
	public Control mouseDrag(final Position oldPoint, final Position newPoint, final boolean fine) {
		
		boolean newFine = fine || getNonMaps().getNonLinearWorld().isFineActive();

		if (currentIncrementalChanger != null) {
			final double amount = newPoint.getX() - oldPoint.getX();
			currentIncrementalChanger.changeBy(newFine, amount);
		}
		return this;
	}

	@Override
	public Control pinch(final Position eventPoint, final double touchDist, final TouchPinch pinch) {
		boolean newFine = getNonMaps().getNonLinearWorld().isFineActive();

		if (currentIncrementalChanger != null) {
			final double maxDiff = pinch.getMaxTouchDistance();
			currentIncrementalChanger.changeBy(newFine, maxDiff);
		} else {
			mouseDown(eventPoint);
		}

		return this;
	}

	@Override
	public Control mouseDown(final Position eventPoint) {
		startEdit();
		return this;
	}

	public IncrementalChanger startEdit() {
		return startEdit(slider.getParamSliderVisibleWidth());
	}

	public void startEdit(final Mode mode, final double width) {
		setMode(mode);
		startEdit(width);
	}

	public IncrementalChanger startEdit(final double width) {
		final ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		if (isParameterDisabled() || isParameterHidden())
			return null;

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
	public Control doubleClick(Position pos) {
		final ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		final ModSource selectedMC = p.modulation.modulationSource;
		final boolean isValidMacro = selectedMC != ModSource.None;

		switch(mode)
		{
			case mcAmount:
			case mcLower:
			case mcUpper:
				EditBufferUseCases.get().setModulationAmount(p.id, 0, true);
				return this;
			case mcValue:
				if(isValidMacro)
					EditBufferUseCases.get().setToDefault(selectedMC.toParameterId());
				return this;
			case mcSource:
			case modulateableParameter:
			case paramValue:
			case unmodulateableParameter:
			default:
				EditBufferUseCases.get().setToDefault(p.id);
				return this;
		}
	}

	@Override
	public Control onKey(final KeyDownEvent event) {
		boolean fine = getNonMaps().getNonLinearWorld().isFineActive();
		if (event.getNativeKeyCode() == KeyCodes.KEY_K) {
			startEdit().inc(fine);
			return this;
		} else if (event.getNativeKeyCode() == KeyCodes.KEY_M && !event.isControlKeyDown()) {
			startEdit().dec(fine);
			return this;
		}

		return null;
	}

	@Override
	public Control wheel(final Position eventPoint, final double amount, final boolean fine) {
		boolean newFine = fine || getNonMaps().getNonLinearWorld().isFineActive();
		if (amount > 0)
			startEdit().inc(newFine);
		else if (amount < 0)
			startEdit().dec(newFine);

		return this;
	}

	public String getCurrentValuesName(final double width) {
		final ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
		return p.fullNameWithGroup;
	}

	public String[] getDecoratedValue(final boolean b) {
		final ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
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
