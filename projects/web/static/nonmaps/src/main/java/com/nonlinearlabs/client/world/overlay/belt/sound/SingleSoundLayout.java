package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

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
			addChild(vgI = new FX_I_Indicator(this));
			addChild(vgII = new FX_II_Indicator(this));
			addChild(serialArrow = new SerialArrow(this));
			addChild(fbToPoly = new FBToPolyArrows(this));

			EditBufferPresenterProvider.get().onChange(ebp -> {
				setVisible(ebp.isSingleSoundFeedbackToPolyActive);
				return true;
			});
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

	protected class PolyToFBArrow extends SVGImage {
		private int selectedPhase = 0;

		PolyToFBArrow(OverlayLayout parent) {
			super(parent, "LC-to-RT--feedback.svg", "LC-to-RB--feedback.svg", "LC-to-RT+LC-to-RB--feedback.svg");

			EditBufferPresenterProvider.get().onChange(ebp -> {
				
				switch(ebp.polyToFX) {
					case Poly_To_FXI:
						selectedPhase = 0;
						break;
					case Poly_To_FXII:
						selectedPhase = 1;
						break;
					case Poly_To_FXI_FXII:
						selectedPhase = 2;
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

	protected class FXToOutArrows extends SVGImage {
		private int selectedPhase = 0;

		FXToOutArrows(OverlayLayout parent) {
			super(parent, "LT-to-RC.svg", "LB-to-RC.svg", "LT-to-RC+LB-to-RC.svg");

			EditBufferPresenterProvider.get().onChange(ebp -> {
				switch(ebp.fxToOut){
					case FXI:
						selectedPhase = 0;
						break;
					case FXII:
						selectedPhase = 1;
						break;
					case FXI_FXII:
						selectedPhase = 2;
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

	protected class PolyToFXContainer extends OverlayLayout {
		private SVGImage vgI;
		private SVGImage vgII;
		private SVGImage polyToFBArrow;
		private SVGImage serial;
		private SVGImage fxToOut;

		PolyToFXContainer(OverlayLayout parent) {
			super(parent);
			addChild(polyToFBArrow = new PolyToFBArrow(this));
			addChild(vgI = new FX_I_Indicator(this));
			addChild(vgII = new FX_II_Indicator(this));
			addChild(serial = new SerialArrow(this));
			addChild(fxToOut = new FXToOutArrows(this));
		}

		@Override
		public void doLayout(double x, double y, double w, double h) {
			super.doLayout(x, y, w, h);

			//arrows from poly to fx
			double arrowWidth = polyToFBArrow.getPictureWidth();
			polyToFBArrow.doLayout(0, 0, arrowWidth, h);

			//voicegroup
			double yOffset = Millimeter.toPixels(2);			
			double vgHeight = vgI.getPictureHeight();
			double vgWidth = vgI.getPictureWidth();
			vgI.doLayout(arrowWidth, (h / 2) - (vgHeight * 2) + yOffset, vgWidth, vgHeight);
			vgII.doLayout(arrowWidth, (h / 2) + (vgHeight) - yOffset, vgWidth, vgHeight);

			//serial
			double xOffset = Millimeter.toPixels(3);
			double serialArrowHeight = serial.getPictureHeight();
			double serialArrowWidth = serial.getPictureWidth();
			serial.doLayout(arrowWidth - (serialArrowWidth / 2) + xOffset, (h / 2) - (serialArrowHeight / 2) + Millimeter.toPixels(3), serialArrowHeight, serialArrowWidth);

			//fx to out
			double outArrowWidth = fxToOut.getPictureWidth();
			double outArrowHeight = fxToOut.getPictureHeight();
			fxToOut.doLayout(arrowWidth + vgWidth, 0, outArrowWidth, h);
		}


		@Override
		public void draw(Context2d ctx, int i) {
			super.draw(ctx, i);
		}
	}

	private SingleSoundSettings settings = null;
	private FBContainer fbContainer = null;
	private PolyToFXContainer polyToFXContainer = null;

	protected SingleSoundLayout(OverlayLayout parent) {
		super(parent);
		settings = new SingleSoundSettings(this);
		setSettings(settings);
		addChild(fbContainer = new FBContainer(this));
		addChild(polyToFXContainer = new PolyToFXContainer(this));
	}

	@Override
	public void doLayout(double x, double y, double w, double h)
	{
		super.doLayout(x, y, w, h);
		double left = settings.getOuterBox().getLeft();
		double right = settings.getOuterBox().getRight();
		double width = w/12;
		fbContainer.doLayout(left - width, 0, width, h);
		double polyContainerWidth = w/8;
		polyToFXContainer.doLayout(right, 0, polyContainerWidth, h);
	}

	@Override
	public void draw(Context2d ctx, int invalidationFlags)
	{
		super.draw(ctx, invalidationFlags);
	}
}
