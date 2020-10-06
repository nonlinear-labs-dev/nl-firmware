package com.nonlinearlabs.client.world.maps.parameters.VoiceGroupMaster;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.ColorTable;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroup;

public class VoiceGroupMaster extends ParameterGroup {

	public VoiceGroupMaster(MapsLayout parent) {
		super(parent, "Part");
		addChild(new LabelModuleHeader(this) {

			@Override
			public RGB getColorFont() {
				return RGB.lighterGray();
			}

			@Override
			public void draw(Context2d ctx, int invalidationMask) {
				super.draw(ctx, invalidationMask);
				
				if(SetupModel.get().systemSettings.syncSplit.getBool() && EditBufferModel.get().soundType.getValue() == SoundType.Split) {
					Rect pos = getPixRect();
					String text = "->";
					double w = ctx.measureText(text).getWidth();
					ctx.fillText(text, pos.getRight() - w, pos.getCenterPoint().getY());
				}	
			}
		});
		addChild(new VoiceGroupMasterControls(this));
	}

	@Override
	public RGB getColorSliderHighlight() {
		return RGB.lightGray();
	}

	@Override
	public RGB getColorSliderBackground() {
		return ColorTable.getColorSliderBackground();
	}

	@Override
	public RGB getColorModuleBackground() {
		return ColorTable.getColorAUXBackground();
	}

	@Override
	public RGB getColorModuleHeaderBackground() {
		return RGB.lightGray();
	}

	@Override
	public RGB getColorFontValueDisplay() {
		return RGB.lightGray();
	}

	@Override
	public RGB getColorFontValueDisplaySelected() {
		return RGB.lightGray();
	}

	@Override
	public RGB getColorObjectBackgroundSelected() {
		return ColorTable.getColorAUXSelectedBackground();
	}

	@Override
	public RGB getColorObjectContourSelected() {
		return RGB.lightGray();
	}

	@Override
	public RGB getColorObjectContour() {
		return ColorTable.getColorContour();
	}

	@Override
	public RGB getColorIndicator() {
		return ColorTable.getColorIndicator();
	}

}
