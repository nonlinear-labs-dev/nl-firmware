package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.google.gwt.core.client.GWT;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.EditBufferPresenter.SingleSoundFBToPoly;
import com.nonlinearlabs.client.presenters.EditBufferPresenter.SingleSoundFXToOut;
import com.nonlinearlabs.client.presenters.EditBufferPresenter.SingleSoundPolyToFX;
import com.nonlinearlabs.client.tools.Pair;
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

	protected class FBContainer extends OverlayLayout
	{
		private SVGImage fb;
		private SVGImage vgI;
		private SVGImage vgII;
		private SerialArrow serialArrow;
		private MappedSvgImage<SingleSoundFBToPoly> fbToPoly;

		public FBContainer(OverlayLayout parent) {
			super(parent);
			addChild(fb = new StaticImage(this, "FB.svg"));
			addChild(vgI = new FX_I_Indicator(this));
			addChild(vgII = new FX_II_Indicator(this));
			addChild(serialArrow = new SerialArrow(this));
			addChild(fbToPoly = new MappedSvgImage<SingleSoundFBToPoly>(this, 
									new Pair<SingleSoundFBToPoly, String>(SingleSoundFBToPoly.FXI_To_Poly, "LT-to-RC.svg"),
									new Pair<SingleSoundFBToPoly, String>(SingleSoundFBToPoly.FXII_To_Poly, "LB-to-RC.svg"),
									new Pair<SingleSoundFBToPoly, String>(SingleSoundFBToPoly.FXI_FXII_To_Poly, "LT-to-RC+LB-to-RC.svg")
			));

			EditBufferPresenterProvider.get().onChange(ebp -> {
				setVisible(ebp.isSingleSoundFeedbackToPolyActive);
				fbToPoly.update(ebp.fbToPolyArrowState);
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
	}

	protected class PolyToFXContainer extends OverlayLayout {
		private SVGImage vgI;
		private SVGImage vgII;
		private MappedSvgImage<SingleSoundPolyToFX> polyToFBArrow;
		private SVGImage serial;
		private MappedSvgImage<SingleSoundFXToOut> fxToOut;

		PolyToFXContainer(OverlayLayout parent) {
			super(parent);
			addChild(polyToFBArrow = new MappedSvgImage<SingleSoundPolyToFX>(this, 
												new Pair<SingleSoundPolyToFX, String>(SingleSoundPolyToFX.Poly_To_FXI, "LC-to-RT--feedback.svg"),
												new Pair<SingleSoundPolyToFX, String>(SingleSoundPolyToFX.Poly_To_FXII, "LC-to-RB--feedback.svg"),
												new Pair<SingleSoundPolyToFX, String>(SingleSoundPolyToFX.Poly_To_FXI_FXII, "LC-to-RT+LC-to-RB--feedback.svg")));

			addChild(vgI = new FX_I_Indicator(this));
			addChild(vgII = new FX_II_Indicator(this));
			addChild(serial = new SerialArrow(this));
			addChild(fxToOut = new MappedSvgImage<SingleSoundFXToOut>(this,
												new Pair<SingleSoundFXToOut, String>(SingleSoundFXToOut.FXI, "LT-to-RC.svg"),
												new Pair<SingleSoundFXToOut, String>(SingleSoundFXToOut.FXII, "LB-to-RC.svg"),
												new Pair<SingleSoundFXToOut, String>(SingleSoundFXToOut.FXI_FXII, "LT-to-RC+LB-to-RC.svg")));
		
		
			EditBufferPresenterProvider.get().onChange(ebp -> {
				fxToOut.update(ebp.fxToOut);
				polyToFBArrow.update(ebp.polyToFX);
				return true;
			});	
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
			fxToOut.doLayout(arrowWidth + vgWidth, 0, outArrowWidth, h);
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
		GWT.log("SingleSoundLayout::doLayout");
		super.doLayout(x, y, w, h);
		double left = settings.getOuterBox().getLeft();
		double right = settings.getOuterBox().getRight();
		double width = w/12;
		fbContainer.doLayout(left - width, 0, width, h);
		double polyContainerWidth = w/8;
		polyToFXContainer.doLayout(right, 0, polyContainerWidth, h);
	}
}
