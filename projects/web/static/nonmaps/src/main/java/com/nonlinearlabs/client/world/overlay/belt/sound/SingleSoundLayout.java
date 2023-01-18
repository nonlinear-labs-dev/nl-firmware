package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;
import com.nonlinearlabs.client.presenters.EditBufferPresenter.SerialFX;

public class SingleSoundLayout extends SoundLayout {


	protected class StaticImage extends SVGImage {
		StaticImage(OverlayLayout parent, String path) {
			super(parent, path);
		}

		@Override
		public int getSelectedPhase() {
			return 0;
		}
	}

	protected class VoiceGroup_I_Indicator extends SVGImage {
	
		VoiceGroup_I_Indicator(OverlayLayout parent) {
			super(parent, "FX-I-active.svg", "FX-I-inactive.svg");
		
			EditBufferPresenterProvider.get().onChange(e -> {
				invalidate(INVALIDATION_FLAG_UI_CHANGED);
				return true;
			});
		}

		@Override
		public int getSelectedPhase() {
			VoiceGroup currentVG = EditBufferPresenterProvider.getPresenter().voiceGroupEnum;
			return currentVG == VoiceGroup.I ? 0 : 1;
		}
	}

	protected class VoiceGroup_II_Indicator extends SVGImage {
	
		VoiceGroup_II_Indicator(OverlayLayout parent) {
			super(parent, "FX-II-active.svg", "FX-II-inactive.svg");
		
			EditBufferPresenterProvider.get().onChange(e -> {
				invalidate(INVALIDATION_FLAG_UI_CHANGED);
				return true;
			});
		}

		@Override
		public int getSelectedPhase() {
			VoiceGroup currentVG = EditBufferPresenterProvider.getPresenter().voiceGroupEnum;
			return currentVG == VoiceGroup.I ? 1 : 0;
		}
	}

	protected class SerialArrow extends SVGImage {
		int selectedPhase = 0;

		SerialArrow(OverlayLayout parent) {
			super(parent, "CT-to-CB.svg", "CB-to-CT.svg");
			setVisible(false);

			EditBufferPresenterProvider.get().onChange(ebp -> {
				switch(ebp.serialFX) {
					case None:
						setVisible(false);
					break;
					case FX_I_IN_II:
						selectedPhase = 0;
						invalidate(INVALIDATION_FLAG_UI_CHANGED);
						setVisible(true);
					break;
					case FX_II_IN_I:
						selectedPhase = 1;
						invalidate(INVALIDATION_FLAG_UI_CHANGED);
						setVisible(true);
						break;
				}
				return true;
			});
		}

		@Override
		public int getSelectedPhase() {
			return selectedPhase;
		}
	}

	protected class FBToPolyArrows extends SVGImage {
		int selectedPhase = 0;

		FBToPolyArrows(OverlayLayout parent) {
			super(parent, "LT-to-RC+LB-to-RC.svg", "LT-to-RC.svg", "LB-to-RC.svg");

			EditBufferPresenterProvider.get().onChange(ebp -> {
				switch(ebp.fbToPolyArrowState) {
					case FXI_To_Poly:
					selectedPhase = 1;
					break;
					case FXII_To_Poly:
					selectedPhase = 2;
					break;
					case FXI_FXII_To_Poly:
					selectedPhase = 0;
					break;
				}
				invalidate(INVALIDATION_FLAG_UI_CHANGED);
				return true;
			});
		}

		@Override
		public int getSelectedPhase() {
			return selectedPhase;
		}
	} 

	protected class FBContainer extends OverlayLayout
	{
		private SVGImage fb;
		private SVGImage vgI;
		private SVGImage vgII;
		private SerialArrow serialArrow;
		private SVGImage fbToPoly;

		public FBContainer(OverlayLayout parent) {
			super(parent);
			addChild(fb = new StaticImage(this, "FB.svg"));
			addChild(vgI = new VoiceGroup_I_Indicator(this));
			addChild(vgII = new VoiceGroup_II_Indicator(this));
			addChild(serialArrow = new SerialArrow(this));
			addChild(fbToPoly = new FBToPolyArrows(this));
		}

		@Override
		public void doLayout(double x, double y, double w, double h)
		{
			super.doLayout(x, y, w, h);
			double fbHeight = fb.getPictureHeight();
			double fbWidth = fb.getPictureWidth();
			fb.doLayout(0, (h / 2) - (fbHeight / 2), fbWidth, fbHeight);

			//voicegroup indicators
			double yOffset = Millimeter.toPixels(2);			
			double vgHeight = vgI.getPictureHeight();
			double vgWidth = vgI.getPictureWidth();
			vgI.doLayout(fbWidth, (h / 2) - (vgHeight * 2) + yOffset, vgWidth, vgHeight);
			vgII.doLayout(fbWidth, (h / 2) + (vgHeight) - yOffset, vgWidth, vgHeight);

			//serial arrow
			double serialArrowHeight = serialArrow.getPictureHeight();
			double serialArrowWidth = serialArrow.getPictureWidth();
			serialArrow.doLayout(fbWidth / 2, (h / 2) - (serialArrowHeight / 2) + Millimeter.toPixels(3), serialArrowHeight, serialArrowWidth);

			//fx to poly FB arrows
			double arrowHeight = fbToPoly.getPictureHeight();
			double arrowWidth = fbToPoly.getPictureWidth();
			fbToPoly.doLayout(vgWidth + fbWidth / 1.5, h / 2 - arrowHeight / 3 - yOffset, arrowHeight, arrowWidth);
		}

		@Override
		public void draw(Context2d ctx, int invalidationFlags)
		{
			super.draw(ctx, invalidationFlags);
		}
	}



	private SingleSoundSettings settings = null;
	private FBContainer fbContainer = null;

	protected SingleSoundLayout(OverlayLayout parent) {
		super(parent);
		settings = new SingleSoundSettings(this);
		setSettings(settings);
		addChild(fbContainer = new FBContainer(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h)
	{
		super.doLayout(x, y, w, h);
		double left = settings.getOuterBox().getLeft();
		double width = w/12;
		fbContainer.doLayout(left - width, 0, width, h);
	}

	@Override
	public void draw(Context2d ctx, int invalidationFlags)
	{
		super.draw(ctx, invalidationFlags);
	}
}
