package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.Renameable;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.presenters.ParameterPresenterProviders;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.RenameDialog;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayControl;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public class SingleSoundSettings extends OverlayLayout {

	class NamedValueEdit extends OverlayLayout {

		Label label;
		ValueEdit edit;
		ParameterPresenter presenter;

		protected NamedValueEdit(Control parent, ParameterId param) {
			super(parent);
			ParameterPresenterProviders.get().register(param, p -> {
				presenter = p;
				return true;
			});

			label = addChild(new Label(this) {

				@Override
				public String getDrawText(Context2d ctx) {
					return presenter.longName;
				}
			});

			edit = addChild(new ValueEdit(this, param, true));
		}

		@Override
		public void doLayout(double x, double y, double w, double h) {
			super.doLayout(x, y, w, h);
			edit.doLayout(0, 0, w, h / 2);
			label.doLayout(0, h / 2, w, h / 2);
		}
	}

	private class StaticParameterLabel extends Label {

		private String text;

		public StaticParameterLabel(OverlayLayout parent, String text) {
			super(parent);
			this.text = text;
		}

		@Override
		public String getDrawText(Context2d ctx) {
			return text;
		}
	}

	private class ParameterValue extends ValueEdit {

		public ParameterValue(OverlayLayout parent, int number) {
			super(parent, new ParameterId(number, EditBufferModel.get().voiceGroup.getValue()), true);
		}

		public ParameterValue(OverlayLayout parent, VoiceGroup vg, int number) {
			super(parent, new ParameterId(number, vg), false);
		}
	}

	private class PresetName extends Label {
		PresetName(OverlayLayout parent) {
			super(parent);
		}

		@Override
		public String getDrawText(Context2d ctx) {
			return EditBufferModel.get().loadedPresetName.getValue();
		}

		@Override
		public void draw(Context2d ctx, int invalidationMask) {
			getPixRect().drawRoundedArea(ctx, Millimeter.toPixels(0.5), 1, new Gray(68), new Gray(86));
			super.draw(ctx, invalidationMask);
		}
	}

	PresetName presetName;

	StaticParameterLabel masterLabel;
	StaticParameterLabel tuneLabel;
	ParameterValue masterValue;
	ParameterValue tuneValue;


	protected SingleSoundSettings(Control parent) {
		super(parent);
		presetName = addChild(new PresetName(this));
		tuneLabel = addChild(new StaticParameterLabel(this, "Master Tune"));
		masterLabel = addChild(new StaticParameterLabel(this, "Master Volume"));
		tuneValue = addChild(new ParameterValue(this, VoiceGroup.Global, 248));
		masterValue = addChild(new ParameterValue(this, VoiceGroup.Global, 247));
	}

	public Rect getOuterBoxPixRect() {
		Rect newRect = getPixRect().copy();
		double reduceBy = newRect.getWidth() / 4;
		double verticalMargin = Millimeter.toPixels(3);
		newRect.applyPadding(reduceBy, verticalMargin, reduceBy, verticalMargin);
		return newRect;
	}

	public Rect getOuterBox() {
		Rect newRect = getPositionRelativeToParent((OverlayControl)getParent()).copy();
		double reduceBy = newRect.getWidth() / 4;
		double verticalMargin = Millimeter.toPixels(3);
		newRect.applyPadding(reduceBy, verticalMargin, reduceBy, verticalMargin);
		return newRect;
	}

	private Rect getInnerBox() {
		Rect outer = getOuterBox();
		outer.reduceWidthBy(Millimeter.toPixels(8));
		outer.reduceHeightBy(Millimeter.toPixels(2));
		return outer;
	}

	private Rect getInnerBoxPixRect() {
		Rect outer = getOuterBoxPixRect();
		outer.reduceWidthBy(Millimeter.toPixels(8));
		outer.reduceHeightBy(Millimeter.toPixels(2));
		return outer;
	}

	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		
		
		double middleLine = getPixRect().getHeight() / 2;
		double lineOffset = Millimeter.toPixels(5);

		double boxWidth = getInnerBox().getReducedBy(Millimeter.toPixels(4)).getWidth();
		
		double boxStart = (w - boxWidth) / 2;

		double entryWidth = (boxWidth / 2);
		double entryHeight = Millimeter.toPixels(8);
		double labelHeight = Millimeter.toPixels(10);
		double entryOffset = Millimeter.toPixels(1);

		presetName.doLayout(boxStart, lineOffset, boxWidth, h / 5);
		masterLabel.doLayout(boxStart, middleLine - lineOffset, entryWidth, labelHeight);
		tuneLabel.doLayout(boxStart, middleLine + lineOffset, entryWidth, labelHeight);
		masterValue.doLayout(boxStart + entryWidth, middleLine - lineOffset + entryOffset, entryWidth, entryHeight);
		tuneValue.doLayout(boxStart + entryWidth, middleLine + lineOffset + entryOffset, entryWidth, entryHeight);
	}

	@Override
	public void draw(Context2d ctx, int invalidationFlag)
	{
		double margin = Millimeter.toPixels(1);
		getOuterBoxPixRect().drawRoundedArea(ctx, margin, 1, new Gray(0x66), new Gray(0x66));
		getInnerBoxPixRect().drawRoundedArea(ctx, margin, 1, new Gray(30), new Gray(30));
		super.draw(ctx, invalidationFlag);
	}

}
