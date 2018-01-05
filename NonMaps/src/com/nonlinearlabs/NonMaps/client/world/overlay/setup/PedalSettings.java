package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.PlayControls;

public class PedalSettings extends SettingsControl {

	private int choice = 0;

	private class Menu extends SettingsMenu {
		private Menu(PedalSettings parent) {
			super(parent);
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
				return "Pedal1Mode";
			else if (playControls.getPedal(1) == getPedal())
				return "Pedal2Mode";
			else if (playControls.getPedal(2) == getPedal())
				return "Pedal3Mode";

			return "Pedal4Mode";
		}

		@Override
		public void update(Node settingsNode, Node deviceInfo) {
			String str = ServerProxy.getChildText(settingsNode, getPedalSettingName(), "value");
			fromSettingsString(str);
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
	public void update(Node settingsNode, Node deviceInfo) {

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
