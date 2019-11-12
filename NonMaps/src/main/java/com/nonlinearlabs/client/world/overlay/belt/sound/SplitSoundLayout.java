package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public class SplitSoundLayout extends SoundLayout {

	protected SplitSoundLayout(OverlayLayout parent) {
		super(parent);
		setSettings(new SplitSoundSettings(this));
	}

	private class SplitSoundSettings extends OverlayLayout {
		SplitSoundSettings(SplitSoundLayout parent) {
			super(parent);
			addChild(new VoiceGroupSoundSettings(VoiceGroup.I, this));
			addChild(new SplitPoint(this));
			addChild(new VoiceGroupSoundSettings(VoiceGroup.II, this));
		}

		@Override
		public void doLayout(double x, double y, double w, double h) {
			super.doLayout(x, y, w, h);
			double margin = Millimeter.toPixels(2);
			double parts = 20;
			double unit = (w - 2 * margin) / parts;
			double splitPointHeight = Math.min(h, Millimeter.toPixels(20));
			getChildren().get(0).doLayout(0 * unit, margin, 8 * unit, h - 2 * margin);
			getChildren().get(1).doLayout(9 * unit, (h - splitPointHeight) / 2, 2 * unit, splitPointHeight);
			getChildren().get(2).doLayout(12 * unit, margin, 8 * unit, h - 2 * margin);
		}
	}

	private class SplitPoint extends OverlayLayout {

		public SplitPoint(OverlayLayout parent) {
			super(parent);

			addChild(new SplitPointLabel(this));
			addChild(new SplitPointValue(this));
		}

		@Override
		public void doLayout(double x, double y, double w, double h) {
			super.doLayout(x, y, w, h);
			getChildren().get(0).doLayout(0, 0, w, h / 3);
			getChildren().get(1).doLayout(0, h / 2, w, h / 3);
		}

		private class SplitPointLabel extends Label {

			public SplitPointLabel(OverlayLayout parent) {
				super(parent);
			}

			@Override
			public String getDrawText(Context2d ctx) {
				return "Split Point";
			}
		}

		private class SplitPointValue extends ValueEdit {

			public SplitPointValue(OverlayLayout parent) {
				super(parent, EditBufferModel.findParameter(248));
			}
		}

	}

	private class VoiceGroupSoundSettings extends OverlayLayout {
		VoiceGroup group;

		VoiceGroupSoundSettings(VoiceGroup g, SplitSoundSettings parent) {
			super(parent);
			group = g;
			addChild(new VoiceGroupLabel(this));
			addChild(new PresetName(this));
			addChild(new TuneLabel(this));
			addChild(new TuneReference(this));
			addChild(new VolumeLabel(this));
			addChild(new Volume(this));
		}

		@Override
		public void doLayout(double x, double y, double w, double h) {
			super.doLayout(x, y, w, h);
			double margin = Millimeter.toPixels(3);
			double parts = 20;
			double xunit = (w - 2 * margin) / parts;
			double yunit = (h - 2 * margin) / parts;

			getChildren().get(0).doLayout(margin + 0 * xunit, margin, xunit, 5 * yunit);
			getChildren().get(1).doLayout(margin + 3 * xunit, margin, 17 * xunit, 5 * yunit);
			getChildren().get(2).doLayout(margin + 0 * xunit, margin + 8 * yunit, 10 * xunit, 5 * yunit);
			getChildren().get(3).doLayout(margin + 10 * xunit, margin + 8 * yunit, 10 * xunit, 5 * yunit);
			getChildren().get(4).doLayout(margin + 0 * xunit, margin + 15 * yunit, 10 * xunit, 5 * yunit);
			getChildren().get(5).doLayout(margin + 10 * xunit, margin + 15 * yunit, 10 * xunit, 5 * yunit);
		}

		@Override
		public void draw(Context2d ctx, int invalidationMask) {
			double margin = Millimeter.toPixels(1);
			getPixRect().drawRoundedArea(ctx, margin, 1, new Gray(100), new Gray(100));

			double contentLeft = getChildren().get(1).getPixRect().getLeft();
			double contentRight = getPixRect().getRight();
			Rect contentRect = new Rect(contentLeft - 2 * margin, getPixRect().getTop() + 1 * margin,
					contentRight - contentLeft + 1 * margin, getPixRect().getHeight() - 2 * margin);
			contentRect.drawRoundedArea(ctx, margin, 1, new Gray(30), new Gray(30));
			super.draw(ctx, invalidationMask);
		}

		private class VoiceGroupLabel extends Label {

			VoiceGroupLabel(VoiceGroupSoundSettings parent) {
				super(parent);
			}

			@Override
			public String getDrawText(Context2d ctx) {
				return group == VoiceGroup.I ? "I" : "II";
			}

			@Override
			protected double getFontHeight(Rect pixRect) {
				return pixRect.getHeight();
			}
		}

		private class TuneLabel extends Label {

			TuneLabel(VoiceGroupSoundSettings parent) {
				super(parent);
			}

			@Override
			public String getDrawText(Context2d ctx) {
				return "Tune";
			}
		}

		private class VolumeLabel extends Label {

			VolumeLabel(VoiceGroupSoundSettings parent) {
				super(parent);
			}

			@Override
			public String getDrawText(Context2d ctx) {
				return "Volume";
			}
		}

		private class PresetName extends Label {
			PresetName(VoiceGroupSoundSettings parent) {
				super(parent);
			}

			@Override
			public String getDrawText(Context2d ctx) {
				return EditBufferModel.getPresetNameOfVoiceGroup(group);
			}

			@Override
			public void draw(Context2d ctx, int invalidationMask) {
				getPixRect().drawRoundedArea(ctx, Millimeter.toPixels(0.5), 1, new Gray(68), new Gray(86));
				super.draw(ctx, invalidationMask);
			}
		}

		private class TuneReference extends ValueEdit {
			TuneReference(VoiceGroupSoundSettings parent) {
				super(parent, EditBufferModel.findParameter(248));
			}
		}

		private class Volume extends ValueEdit {
			Volume(VoiceGroupSoundSettings parent) {
				super(parent, EditBufferModel.findParameter(248));
			}
		}
	}

}
