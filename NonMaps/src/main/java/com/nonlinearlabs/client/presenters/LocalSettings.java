package com.nonlinearlabs.client.presenters;

import com.nonlinearlabs.client.dataModel.setup.SetupModel;

public class LocalSettings {

	static public class SelectionAutoScroll {
		static public String[] options = { "Off", "Parameter", "Preset", "Parameter and Preset" };
		public int selected = 3;
		public SetupModel.SelectionAutoScroll selectedOption = SetupModel.SelectionAutoScroll.off;

		public boolean isOneOf(SetupModel.SelectionAutoScroll... choices) {
			for (SetupModel.SelectionAutoScroll c : choices)
				if (c == selectedOption)
					return true;

			return false;
		}
	}

	static public class EditParameter {
		static public String[] options = { "Always", "If Selected", "Never" };
		public int selected = 1;
	}

	static public class ContextMenus {
		static public String[] options = { "On", "Off" };
		public boolean value = true;
	}

	static public class PresetDragDrop {
		static public String[] options = { "On", "Off" };
		public boolean value = true;
	}

	static public class DisplayScalingFactor {
		static public String[] options = { "50%", "75%", "100%", "125%", "150%" };
		public int selected = 2;
	}

	static public class StripeBrightness {
		static public String[] options = { "Off", "10%", "25%", "50%" };
		public int selected = 2;
	}

	static public class BitmapCache {
		static public String[] options = { "On", "Off" };
		public boolean value = true;
	}

	static public class ShowDeveloperOptions {
		static public String[] options = { "On", "Off" };
		public boolean value = true;
	}

	public SelectionAutoScroll selectionAutoScroll = new SelectionAutoScroll();
	public EditParameter editParameter = new EditParameter();
	public ContextMenus contextMenus = new ContextMenus();
	public PresetDragDrop presetDragDrop = new PresetDragDrop();
	public DisplayScalingFactor displayScalingFactor = new DisplayScalingFactor();
	public StripeBrightness stripeBrightness = new StripeBrightness();
	public BitmapCache bitmapCache = new BitmapCache();
	public ShowDeveloperOptions showDeveloperOptions = new ShowDeveloperOptions();
}
