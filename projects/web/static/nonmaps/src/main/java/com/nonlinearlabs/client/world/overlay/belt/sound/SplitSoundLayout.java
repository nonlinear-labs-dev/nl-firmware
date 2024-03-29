package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.core.client.GWT;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.Renameable;
import com.nonlinearlabs.client.ServerProxy;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BooleanValues;
import com.nonlinearlabs.client.presenters.EditBufferPresenter;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.EditBufferPresenter.GenericArrowEnum;
import com.nonlinearlabs.client.tools.Pair;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.IPreset;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.RGBA;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.RenameDialog;
import com.nonlinearlabs.client.world.maps.presets.bank.preset.ChoosePresetPartDialog;
import com.nonlinearlabs.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.client.world.overlay.DragProxy;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class SplitSoundLayout extends SoundLayout {
	private OverlayLayout leftC, rightC;
	private SplitSoundSettings settings;

	protected SplitSoundLayout(OverlayLayout parent) {
		super(parent);
		settings = new SplitSoundSettings(this);
		setSettings(settings);
		addChild(leftC = new SplitSoundLeftContainer(this));
		addChild(rightC = new SplitSoundRightContainer(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double settingsWidth = soundSettings.getRelativePosition().getWidth();
		double containerW = (w - settingsWidth) / 2;
		leftC.doLayout(0, y, containerW, h);
		rightC.doLayout(w - containerW, y, containerW, h);
	}

	private class SplitSoundLeftContainer extends OverlayLayout {
		private MappedSvgImage<GenericArrowEnum> partIToFXArrows;
		private MappedSvgImage<GenericArrowEnum> ItoOut;
		private MappedSvgImage<Boolean> fb_from_I_Into_I, fb_from_I_Into_II;
		private SVGImage fxI_I, fxI_II;
		private SerialArrow serialI;

		public SplitSoundLeftContainer(OverlayLayout parent) {
			super(parent);

			addChild(partIToFXArrows = new MappedSvgImage<GenericArrowEnum>(this, 
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.PartI, "RC-to-LT--feedback.svg"), 
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.PartII, "RC-to-LB--feedback.svg"),
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.PartI_PartII, "RC-to-LT+RC-to-LB--feedback.svg"),
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.None, null)));

			addChild(ItoOut = new MappedSvgImage<GenericArrowEnum>(this,
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.PartI, "RT-to-LC.svg"),
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.PartII, "RB-to-LC.svg"),
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.PartI_PartII, "RT-to-LC+RB-to-LC.svg"),
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.None, null)));


			addChild(fxI_I = new FX_I_Indicator(this));
			addChild(fxI_II = new FX_II_Indicator(this));

			addChild(serialI = new SerialArrow(this));

			addChild(fb_from_I_Into_I = new MappedSvgImage<Boolean>(this, new Pair<Boolean, String>(true, "LT-to-RT.svg"), new Pair<Boolean, String>(false, null)));
			addChild(fb_from_I_Into_II = new MappedSvgImage<Boolean>(this, new Pair<Boolean, String>(true, "LB-to-RB.svg"), new Pair<Boolean, String>(false, null)));
			
			EditBufferPresenterProvider.get().onChange(ebp -> {
				partIToFXArrows.update(ebp.splitToFXArrow_I);
				
				fb_from_I_Into_I.update(ebp.split_fb_I_into_I);
				fb_from_I_Into_II.update(ebp.split_fb_I_into_II);
				ItoOut.update(ebp.splitToOut);

				return true;
			});
		}

		@Override
		public void doLayout(double x, double y, double w, double h) {
			super.doLayout(x, y, w, h);

			double margin = Millimeter.toPixels(2);

			double toFXArrowWidth = partIToFXArrows.getPictureWidth();
			partIToFXArrows.doLayout(w - toFXArrowWidth, margin, toFXArrowWidth, h - 2 * margin);
		
			double fx_width_I = fxI_I.getPictureWidth();
			double fx_height_I = fxI_I.getPictureHeight();
			double fx_width_II = fxI_II.getPictureWidth();
			double fx_height_II = fxI_II.getPictureHeight();

			double fxI_x_pos = w - toFXArrowWidth - fx_width_I;
			double yPosOffset = h / 6;
			double yBase = h / 2 - (yPosOffset / 4);

			fxI_I.doLayout(fxI_x_pos, 0 + yPosOffset, fx_width_I, fx_height_I);
			fxI_II.doLayout(fxI_x_pos, yBase + yPosOffset, fx_width_II, fx_height_II);
			serialI.doLayout(fxI_x_pos, 0, Math.max(fx_width_I, fx_width_II), h);

			double toOutWidth = ItoOut.getPictureWidth();
			ItoOut.doLayout(fxI_x_pos - toOutWidth, 0, toOutWidth, h);

			double fb_height_I = fb_from_I_Into_I.getPictureHeight();
			double fb_height_II = fb_from_I_Into_II.getPictureHeight();

			double upper_offset = Millimeter.toPixels(0.3);
			fb_from_I_Into_I.doLayout(w - toFXArrowWidth, yPosOffset - upper_offset, toFXArrowWidth, fb_height_I);
			fb_from_I_Into_II.doLayout(w - toFXArrowWidth, h - yPosOffset - fb_height_II, toFXArrowWidth, fb_height_II);			
		}
	}

	private class SplitSoundRightContainer extends OverlayLayout {
		private MappedSvgImage<GenericArrowEnum> partIIToFXArrows;
		private MappedSvgImage<GenericArrowEnum> IItoOut;
		private SVGImage fxII_I, fxII_II;
		private SerialArrow serialII;
		private MappedSvgImage<Boolean> fb_from_II_Into_I, fb_from_II_Into_II;

		public SplitSoundRightContainer(OverlayLayout parent) {
			super(parent);	

			addChild(partIIToFXArrows = new MappedSvgImage<GenericArrowEnum>(this, 
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.PartI, "LC-to-RB--feedback.svg"),
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.PartII, "LC-to-RT--feedback.svg"),
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.PartI_PartII, "LC-to-RT+LC-to-RB--feedback.svg"),
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.None, null)));

			
											
			addChild(IItoOut = new MappedSvgImage<GenericArrowEnum>(this,
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.PartI, "LT-to-RC.svg"),
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.PartII, "LB-to-RC.svg"),
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.PartI_PartII, "LT-to-RC+LB-to-RC.svg"),
											new Pair<GenericArrowEnum, String>(GenericArrowEnum.None, null)));
			
			addChild(fxII_I = new FX_I_Indicator(this));
			addChild(fxII_II = new FX_II_Indicator(this));

			addChild(serialII = new SerialArrow(this));


			addChild(fb_from_II_Into_I = new MappedSvgImage<Boolean>(this, new Pair<Boolean, String>(true, "RT-to-LT.svg"), new Pair<Boolean, String>(false, null)));
			addChild(fb_from_II_Into_II = new MappedSvgImage<Boolean>(this, new Pair<Boolean, String>(true, "RB-to-LB.svg"), new Pair<Boolean, String>(false, null)));
		
			EditBufferPresenterProvider.get().onChange(ebp -> {
				partIIToFXArrows.update(ebp.splitToFXArrow_II);
				
				fb_from_II_Into_I.update(ebp.split_fb_II_into_I);
				fb_from_II_Into_II.update(ebp.split_fb_II_into_II);

				IItoOut.update(ebp.splitToOut);
				
				return true;
			});
		}

		@Override
		public void doLayout(double x, double y, double w, double h) {
			super.doLayout(x, y, w, h);

			double margin = Millimeter.toPixels(2);

			double toFXArrowWidth = partIIToFXArrows.getPictureWidth();
			partIIToFXArrows.doLayout(0, margin, toFXArrowWidth, h - 2 * margin);
		
			double fx_width_I = fxII_I.getPictureWidth();
			double fx_height_I = fxII_I.getPictureHeight();
			double fx_width_II = fxII_II.getPictureWidth();
			double fx_height_II = fxII_II.getPictureHeight();
			
			double fxI_x_pos = 0 + toFXArrowWidth;
			double yPosOffset = h / 6;
			double yBase = h / 2 - (yPosOffset / 4);

			fxII_I.doLayout(fxI_x_pos, 0 + yPosOffset, fx_width_I, fx_height_I);
			fxII_II.doLayout(fxI_x_pos, yBase + yPosOffset, fx_width_II, fx_height_II);
			serialII.doLayout(fxI_x_pos, 0, Math.max(fx_width_I, fx_width_II), h);

			double toOutWidth = IItoOut.getPictureWidth();			
			IItoOut.doLayout(fxI_x_pos + Math.max(fx_width_I, fx_width_II), 0, toOutWidth, h);

			double fb_height_I = fb_from_II_Into_I.getPictureHeight();
			double fb_height_II = fb_from_II_Into_II.getPictureHeight();
			double upper_offset = Millimeter.toPixels(0.3);
			fb_from_II_Into_I.doLayout(0, yPosOffset - upper_offset, toFXArrowWidth, fb_height_I);
			fb_from_II_Into_II.doLayout(0, h - yPosOffset - fb_height_I, toFXArrowWidth, fb_height_II);
		}
	}

	private class SplitSoundSettings extends OverlayLayout {
		VoiceGroupSoundSettings vgI, vgII;
		SplitPoint splitPoint;
		
		SplitSoundSettings(SplitSoundLayout parent) {
			super(parent);
			addChild(vgI = new VoiceGroupSoundSettings(VoiceGroup.I, this));
			addChild(splitPoint = new SplitPoint(this));
			addChild(vgII = new VoiceGroupSoundSettings(VoiceGroup.II, this));
		}

		@Override
		public void doLayout(double x, double y, double w, double h) {
			super.doLayout(x, y, w, h);
			double margin = Millimeter.toPixels(2);
			double splitPointHeight = Math.min(h, Millimeter.toPixels(30));
			double splitPointWidth = Math.min(w / 4, Millimeter.toPixels(40));
			double settingWidth = (w - splitPointWidth - 2 * margin) / 2;

			vgI.doLayout(0, margin, settingWidth, h - 2 * margin);
			splitPoint.doLayout(settingWidth + margin, (h - splitPointHeight) / 2, splitPointWidth,
					splitPointHeight);
			vgII.doLayout(w - settingWidth, margin, settingWidth, h - 2 * margin);
		}

		@Override
		public void draw(Context2d ctx, int flags) {
			if (SetupModel.get().systemSettings.syncSplit.isFalse()) {
				Rect left = splitPoint.getRectOfVG(VoiceGroup.I);
				Rect right = splitPoint.getRectOfVG(VoiceGroup.II);
				Rect startLeft = vgI.getPixRect();
				Rect endRight = vgII.getPixRect();
				RGB bgI = EditBufferPresenterProvider.getPresenter().voiceGroupI_BackgroundColor;
				RGB bgII = EditBufferPresenterProvider.getPresenter().voiceGroupII_BackgroundColor;

				ctx.setFillStyle(bgI.toString());
				ctx.fillRect(startLeft.getRight(), left.getCenterPoint().getY() - 2,
						left.getLeft() - startLeft.getRight(), 4);

				ctx.setFillStyle(bgII.toString());
				ctx.fillRect(right.getRight(), right.getCenterPoint().getY() - 2, endRight.getLeft() - right.getRight(),
						4);

				if (!EditBufferPresenterProvider.getPresenter().splitOverlap) {
					RGB color = EditBufferModel.get().voiceGroup.getValue() == VoiceGroup.I ? bgI : bgII;
					ctx.setFillStyle(color.toString());
					double startY = left.getCenterPoint().getY();
					double endY = right.getCenterPoint().getY();
					ctx.fillRect(left.getCenterPoint().getX() - 2, startY, 4, endY - startY);
				}
			}
			super.draw(ctx, flags);
		}
	}

	private class SplitPoint extends OverlayLayout {

		private boolean syncEnabled = false;

		public SplitPoint(OverlayLayout parent) {
			super(parent);

			SetupModel.get().systemSettings.syncSplit.onChange(enabled -> {
				onSettingChanged(enabled.equals(BooleanValues.on));
				return true;
			});

		}

		public Rect getRectOfVG(VoiceGroup vg) {
			if (!syncEnabled) {
				if (vg == VoiceGroup.I) {
					return getChildren().get(1).getPixRect();
				} else {
					return getChildren().get(2).getPixRect();
				}
			} else {
				return getChildren().get(1).getPixRect();
			}
		}

		private void onSettingChanged(boolean isSyncEnabled) {
			syncEnabled = isSyncEnabled;

			removeAll();

			if (isSyncEnabled) {
				addChild(new SplitPointLabel(this, "Split Point"));
				addChild(new SplitPointValue(this));
			} else {
				addChild(new SplitPointLabel(this, "Split Points"));
				addChild(new SplitPointValue(this, VoiceGroup.I));
				addChild(new SplitPointValue(this, VoiceGroup.II));
			}

			requestLayout();
		}

		@Override
		public void doLayout(double x, double y, double w, double h) {
			super.doLayout(x, y, w, h);
			double quarterHeight = h / 4;

			if (syncEnabled) {
				getChildren().get(0).doLayout(0, 0, w, quarterHeight * 0.75);
				getChildren().get(1).doLayout(0, quarterHeight * 1.5, w, quarterHeight);
			} else {
				getChildren().get(0).doLayout(0, 0, w, quarterHeight * 0.75);
				getChildren().get(1).doLayout(0, quarterHeight, w, quarterHeight);
				getChildren().get(2).doLayout(0, quarterHeight * 2.4, w, quarterHeight);
			}
		}

		private class SplitPointLabel extends Label {

			private String text;

			public SplitPointLabel(OverlayLayout parent, String text) {
				super(parent);
				this.text = text;
			}

			@Override
			public String getDrawText(Context2d ctx) {
				return text;
			}
		}

		private class SplitPointValue extends ValueEdit {

			public SplitPointValue(OverlayLayout parent) {
				super(parent, new ParameterId(356, EditBufferModel.get().voiceGroup.getValue()), true);
			}

			public SplitPointValue(OverlayLayout parent, VoiceGroup vg) {
				super(parent, new ParameterId(356, vg), false);
			}
		}
	}

	private class VoiceGroupSoundSettings extends OverlayLayout {
		VoiceGroup group;

		VoiceGroupLabel m_voiceGroupLabel;
		PresetName m_presetName;
		VolumeLabel m_volumeLabel;
		Volume m_volumeValue;
		TuneLabel m_tuneLabel;
		TuneReference m_tuneValue;

		VoiceGroupSoundSettings(VoiceGroup g, SplitSoundSettings parent) {
			super(parent);
			group = g;
			m_voiceGroupLabel = addChild(new VoiceGroupLabel(this));
			m_presetName = addChild(new PresetName(this));
			m_volumeLabel = addChild(new VolumeLabel(this));
			m_volumeValue = addChild(new Volume(this));
			m_tuneLabel = addChild(new TuneLabel(this));
			m_tuneValue = addChild(new TuneReference(this));
		}

		@Override
		public void doLayout(double x, double y, double w, double h) {
			super.doLayout(x, y, w, h);
			double margin = Millimeter.toPixels(3);
			double parts = 20;
			double xunit = (w - 2 * margin) / parts;
			double yunit = (h - 2 * margin) / parts;

			double middleLine = h / 2;
			double labelHeight = ((h / 20) * 9) - margin * 2;

			double voiceGroupLabelWidth = 3 * xunit;
			double presetNameWidth = 16 * xunit;
			double parameterLabelWidth = 6 * xunit;
			double parameterValueWidth = 10 * xunit;

			double vgLabelMargin = margin;
			if (xunit < margin) {
				vgLabelMargin /= 2;
			}

			if (group == VoiceGroup.I) {
				m_voiceGroupLabel.doLayout(vgLabelMargin, middleLine - labelHeight / 2, voiceGroupLabelWidth,
						labelHeight);
				m_presetName.doLayout(margin + 4 * xunit, margin, presetNameWidth, 5 * yunit);
				m_volumeLabel.doLayout(margin + 4 * xunit, margin + 8 * yunit, parameterLabelWidth, 5 * yunit);
				m_volumeValue.doLayout(margin + 10 * xunit, margin + 8 * yunit, parameterValueWidth, 5 * yunit);
				m_tuneLabel.doLayout(margin + 4 * xunit, margin + 15 * yunit, parameterLabelWidth, 5 * yunit);
				m_tuneValue.doLayout(margin + 10 * xunit, margin + 15 * yunit, parameterValueWidth, 5 * yunit);
			} else {
				m_tuneValue.doLayout(margin, margin + 15 * yunit, parameterValueWidth, 5 * yunit);
				m_tuneLabel.doLayout(margin + parameterValueWidth, margin + 15 * yunit, parameterLabelWidth, 5 * yunit);
				m_volumeValue.doLayout(margin, margin + 8 * yunit, parameterValueWidth, 5 * yunit);
				m_volumeLabel.doLayout(margin + parameterValueWidth, margin + 8 * yunit, parameterLabelWidth,
						5 * yunit);
				m_presetName.doLayout(margin, margin, presetNameWidth, 5 * yunit);
				m_voiceGroupLabel.doLayout(w - vgLabelMargin - voiceGroupLabelWidth, middleLine - labelHeight / 2,
						voiceGroupLabelWidth, labelHeight);
			}
		}

		@Override
		public void draw(Context2d ctx, int invalidationMask) {
			double margin = Millimeter.toPixels(1);
			EditBufferPresenter presenter = EditBufferPresenterProvider.getPresenter();
			RGB bgColor = (group == VoiceGroup.I) ? presenter.voiceGroupI_BackgroundColor
					: presenter.voiceGroupII_BackgroundColor;

			getPixRect().drawRoundedArea(ctx, margin, 1, bgColor, bgColor);

			double contentLeft = getChildren().get(1).getPixRect().getLeft();
			double contentRight = getPixRect().getRight();
			if (group == VoiceGroup.I) {
				Rect contentRect = new Rect(contentLeft - 2 * margin, getPixRect().getTop() + 1 * margin,
						contentRight - contentLeft + 1 * margin, getPixRect().getHeight() - 2 * margin);
				contentRect.drawRoundedArea(ctx, margin, 1, new Gray(30), new Gray(30));
			} else {
				Rect r = getPixRect().getReducedBy(margin * 2).copy();
				double spaceBetwheenLabelAndLabel = m_voiceGroupLabel.getPixRect().getLeft()
						- m_presetName.getPixRect().getRight();
				r.set(r.getLeft(), r.getTop(),
						r.getWidth() - (m_voiceGroupLabel.getPixRect().getWidth() + spaceBetwheenLabelAndLabel),
						r.getHeight());
				r.drawRoundedArea(ctx, margin, 1, new Gray(30), new Gray(30));
			}

			if (isDropTarget) {
				getPixRect().getReducedBy(1).drawRoundedArea(ctx, margin, 1, RGBA.transparent(), RGB.red());
			}

			super.draw(ctx, invalidationMask);
		}

		@Override
		public Control click(Position eventPoint) {
			EditBufferUseCases.get().selectVoiceGroup(group);
			return this;
		}

		private class VoiceGroupLabel extends Label {

			VoiceGroupLabel(VoiceGroupSoundSettings parent) {
				super(parent);
			}

			@Override
			protected String crop(Context2d ctx, Rect pixRect, String text) {
				return text;
			}

			@Override
			public String getDrawText(Context2d ctx) {
				return group == VoiceGroup.I ? "\uE071" : "\uE072";
			}

			@Override
			protected double getFontHeight(Rect pixRect) {
				return pixRect.getHeight();
			}

			@Override
			public RGB getColorFont() {
				if (group == VoiceGroup.I)
					return EditBufferPresenterProvider.getPresenter().voiceGroupI_ForegroundColor;
				return EditBufferPresenterProvider.getPresenter().voiceGroupII_ForegroundColor;
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
				return EditBufferModel.get().getPresetNameOfVoiceGroupWithSuffix(group);
			}

			@Override
			public Control doubleClick(Position p) {
				RenameDialog.open(new Renameable() {

					@Override
					public void setName(String newName) {
						EditBufferUseCases.get().renamePart(newName, group);
					}

					@Override
					public String getTitleName() {
						if (group == VoiceGroup.I)
							return "\uE071";
						else
							return "\uE072";
					}

					@Override
					public String getEntityName() {
						return "Part";
					}

					@Override
					public String getCurrentName() {
						return EditBufferPresenterProvider.getPresenter().currentPartName;
					}
				});
				return this;
			}

			@Override
			public void draw(Context2d ctx, int invalidationMask) {
				getPixRect().drawRoundedArea(ctx, Millimeter.toPixels(0.5), 1, new Gray(68), new Gray(86));
				super.draw(ctx, invalidationMask);
			}
		}

		private class TuneReference extends ValueEdit {
			TuneReference(VoiceGroupSoundSettings parent) {
				super(parent, new ParameterId(360, group), false);
			}
		}

		private class Volume extends ValueEdit {
			Volume(VoiceGroupSoundSettings parent) {
				super(parent, new ParameterId(358, group), false);
			}
		}

		@Override
		public Control drag(Position pos, DragProxy dragProxy) {
			if (!getPixRect().contains(pos))
				return null;

			if (dragProxy.getOrigin() instanceof IPreset) {
				setIsDropTarget(true);
				return this;
			}
			return super.drag(pos, dragProxy);
		}

		@Override
		public void dragLeave() {
			setIsDropTarget(false);
			super.dragLeave();
		}

		private boolean isDropTarget = false;

		private void setIsDropTarget(boolean isDropTarget) {
			if (this.isDropTarget != isDropTarget) {
				this.isDropTarget = isDropTarget;
				invalidate(INVALIDATION_FLAG_UI_CHANGED);
			}
		}

		private ChoosePresetPartDialog choosePresetPart = null;

		@Override
		public Control drop(Position pos, DragProxy dragProxy) {

			if (dragProxy.getOrigin() instanceof IPreset)
				if (dragProxy.getOrigin() instanceof Preset) {
					Preset p = (Preset) dragProxy.getOrigin();
					if(!p.isDual()) {
						NonMaps.get().getServerProxy().getVoiceGroupsWhereFXIsUnused(p, unused_vg_string -> {
							if(unused_vg_string == "II") {
								EditBufferUseCases.get().loadPresetPartIntoPart(p.getUUID(), VoiceGroup.I, group);
							} else if(unused_vg_string == "I") {
								EditBufferUseCases.get().loadPresetPartIntoPart(p.getUUID(), VoiceGroup.II, group);
							} else {
								choosePresetPart = new ChoosePresetPartDialog(p, group);
							}
						});
					}
					else {
						choosePresetPart = new ChoosePresetPartDialog(p, group);
					}
				}

			setIsDropTarget(false);
			return this;
		}
	}

}
