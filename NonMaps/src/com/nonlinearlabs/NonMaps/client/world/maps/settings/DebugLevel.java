package com.nonlinearlabs.NonMaps.client.world.maps.settings;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.NonLinearWorld;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.maps.ContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.NonPosition;

public class DebugLevel extends Setting {

	public enum Items {
		DEBUG, SILENT, ERROR, WARNING, INFO, GASSY;

		public String toDisplayString() {
			switch (ordinal()) {
			case 0:
				return "Debug";

			case 1:
				return "Silent";

			case 2:
				return "Error";

			case 3:
				return "Warning";

			case 4:
				return "Info";

			case 5:
				return "Gassy";

			default:
				return "";
			}
		}

		public String toString() {
			switch (ordinal()) {
			case 0:
				return "debug";

			case 1:
				return "silent";

			case 2:
				return "error";

			case 3:
				return "warning";

			case 4:
				return "info";

			case 5:
				return "gassy";

			default:
				return "";
			}
		}

		static public Items fromString(String str) {
			if (str != null) {
				if (str.equals("debug"))
					return DEBUG;
				else if (str.equals("silent"))
					return SILENT;
				else if (str.equals("error"))
					return ERROR;
				else if (str.equals("warning"))
					return WARNING;
				else if (str.equals("info"))
					return INFO;
				if (str.equals("gassy"))
					return GASSY;
			}

			return SILENT;
		}
	}

	private class DebugLevelContextMenu extends ContextMenu {

		private DebugLevelContextMenu(MapsLayout parent) {
			super(parent);

			addChild(new ContextMenuItem(this, Items.SILENT.toDisplayString()) {
				@Override
				public Control click(Position eventPoint) {
					setValue(Items.SILENT);
					return super.click(eventPoint);
				}
			});

			addChild(new ContextMenuItem(this, Items.ERROR.toDisplayString()) {
				@Override
				public Control click(Position eventPoint) {
					setValue(Items.ERROR);
					return super.click(eventPoint);
				}
			});

			addChild(new ContextMenuItem(this, Items.WARNING.toDisplayString()) {
				@Override
				public Control click(Position eventPoint) {
					setValue(Items.WARNING);
					return super.click(eventPoint);
				}
			});

			addChild(new ContextMenuItem(this, Items.INFO.toDisplayString()) {
				@Override
				public Control click(Position eventPoint) {
					setValue(Items.INFO);
					return super.click(eventPoint);
				}
			});

			addChild(new ContextMenuItem(this, Items.GASSY.toDisplayString()) {
				@Override
				public Control click(Position eventPoint) {
					setValue(Items.GASSY);
					return super.click(eventPoint);
				}
			});
		}
	}

	DebugLevel(DeveloperSettings parent) {
		super(parent, "Debug Level", Items.WARNING.toDisplayString());
	}

	public void setValue(Items value) {
		NonMaps.theMaps.getServerProxy().setSetting("DebugLevel", value.toString());
		setCurrentValue(value.toDisplayString());
	}

	@Override
	public ContextMenu createMenu(NonPosition pos) {
		NonLinearWorld world = getWorld();
		return world.setContextMenu(new DebugLevelContextMenu(world), pos);
	}

	public void update(Node settingsNode) {
		String str = ServerProxy.getChildText(settingsNode, "DebugLevel", "value");

		if (str != null && !str.isEmpty())
			setCurrentValue(Items.fromString(str).toDisplayString());
	}

	@Override
	public void setDefault() {
		setValue(Items.WARNING);
	}
}
