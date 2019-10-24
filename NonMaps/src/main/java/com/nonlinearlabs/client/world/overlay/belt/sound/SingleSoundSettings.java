package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public class SingleSoundSettings extends OverlayLayout {

	class PresetName extends Label {

		public PresetName(OverlayLayout parent) {
			super(parent);
		}

		@Override
		public String getDrawText(Context2d ctx) {
			return EditBufferModel.loadedPresetName.getValue();
		}
	};

	class NamedValueEdit extends OverlayLayout {

		Label label;
		ValueEdit edit;

		protected NamedValueEdit(Control parent, BasicParameterModel param) {
			super(parent);

			label = addChild(new Label(this) {

				@Override
				public String getDrawText(Context2d ctx) {
					return param.longName.getValue();
				}
			});

			edit = addChild(new ValueEdit(this, param));
		}

		@Override
		public void doLayout(double x, double y, double w, double h) {
			super.doLayout(x, y, w, h);
			edit.doLayout(0, 0, w, h / 2);
			label.doLayout(0, h / 2, w, h / 2);
		}
	}

	PresetName presetName;
	NamedValueEdit tune;
	NamedValueEdit volume;

	protected SingleSoundSettings(Control parent) {
		super(parent);

		presetName = addChild(new PresetName(this));
		tune = addChild(new NamedValueEdit(this, EditBufferModel.findParameter(248)));
		volume = addChild(new NamedValueEdit(this, EditBufferModel.findParameter(247)));
	}

	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		int parts = 20;
		double partSize = w / parts;
		presetName.doLayout(0, 0, w, h / 3);
		tune.doLayout(0, h / 3, 9 * partSize, 2 * h / 3);
		volume.doLayout(11 * partSize, h / 3, 9 * partSize, 2 * h / 3);
	}

}
