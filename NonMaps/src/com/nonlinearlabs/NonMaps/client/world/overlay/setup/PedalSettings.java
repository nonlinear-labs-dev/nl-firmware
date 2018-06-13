package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import java.util.function.Function;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.EnumDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.PedalType;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.PlayControls;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class PedalSettings extends OverlayLayout {

	private int choice = 0;

	private class Menu extends SettingsMenu {
		private Menu(PedalSettings parent) {
			super(parent);

			getPedalSetting().onChange(new Function<Setup.PedalType, Boolean>() {

				@Override
				public Boolean apply(PedalType t) {
					choice = t.ordinal();
					return true;
				}
			});
		}

		@Override
		public PedalSettings getParent() {
			return (PedalSettings) super.getParent();
		}

		@Override
		protected String[] getChoices() {
			String ret[] = { "Pot Tip Active", "Pot Ring Active", "Switch Closing", "Switch Opening" };
			return ret;
		}

		@Override
		protected int getChoice() {
			return choice;
		}

		@Override
		protected void chose(int c, boolean sendToServer) {
			choice = c;
			invalidate(INVALIDATION_FLAG_UI_CHANGED);

			if (sendToServer)
				NonMaps.theMaps.getServerProxy().setSetting(getPedalSettingName(), getSettingsValueString(c));
		}

		protected String getSettingsValueString(int c) {
			return getChoices()[c].replace(" ", "-").toLowerCase();
		}

		private String getPedalSettingName() {
			PlayControls playControls = NonMaps.theMaps.getNonLinearWorld().getParameterEditor().getPlayControls();
			if (playControls.getPedal(0) == getPedal())
				return "Pedal1Type";
			else if (playControls.getPedal(1) == getPedal())
				return "Pedal2Type";
			else if (playControls.getPedal(2) == getPedal())
				return "Pedal3Type";

			return "Pedal4Type";
		}

		private EnumDataModelEntity<PedalType> getPedalSetting() {
			PlayControls playControls = NonMaps.theMaps.getNonLinearWorld().getParameterEditor().getPlayControls();
			if (playControls.getPedal(0) == getPedal())
				return Setup.get().systemSettings.pedal1Type;
			else if (playControls.getPedal(1) == getPedal())
				return Setup.get().systemSettings.pedal2Type;
			else if (playControls.getPedal(2) == getPedal())
				return Setup.get().systemSettings.pedal3Type;

			return Setup.get().systemSettings.pedal4Type;
		}

	}

	private PedalSliderWithValue slider;
	private Menu menu;

	protected PedalSettings(PedalSetting parent) {
		super(parent);
		addChild(slider = new PedalSliderWithValue(this));
		addChild(menu = new Menu(this));
	}

	@Override
	public PedalSetting getParent() {
		return (PedalSetting) super.getParent();
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double margin = getButtonDimension() / 2;
		double elementsWidth = (w - margin);
		slider.doLayout(0, 0, 2 * elementsWidth / 5, h);
		menu.doLayout(slider.getRelativePosition().getWidth() + margin, 0, 3 * elementsWidth / 5, h);
	}

	public Parameter getPedal() {
		return getParent().getPedal();
	}

}
