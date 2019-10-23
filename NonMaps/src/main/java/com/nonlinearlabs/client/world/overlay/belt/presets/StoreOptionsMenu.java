package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.ServerProxy;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.ContextMenu;
import com.nonlinearlabs.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.client.world.overlay.Overlay;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class StoreOptionsMenu extends SVGImage {

	public class StoreOptionsContextMenu extends ContextMenu {

		public StoreOptionsContextMenu(OverlayLayout parent) {
			super(parent);

			if (!currentSetting.equals("Append")) {
				addChild(new ContextMenuItem(this, "Append") {
					@Override
					public Control click(Position eventPoint) {
						NonMaps.get().getServerProxy().setSetting("PresetStoreModeSetting", "append");
						return super.click(eventPoint);
					}
				});
			}

			if (!currentSetting.equals("Insert")) {
				addChild(new ContextMenuItem(this, "Insert") {
					@Override
					public Control click(Position eventPoint) {
						NonMaps.get().getServerProxy().setSetting("PresetStoreModeSetting", "insert");
						return super.click(eventPoint);
					}
				});
			}
			if (!currentSetting.equals("Overwrite")) {
				addChild(new ContextMenuItem(this, "Overwrite") {
					@Override
					public Control click(Position eventPoint) {
						NonMaps.get().getServerProxy().setSetting("PresetStoreModeSetting", "overwrite");
						return super.click(eventPoint);
					}
				});
			}
		}
	}

	String currentSetting = "Append";

	public StoreOptionsMenu(OverlayLayout parent) {
		super(parent, "Append_Enabled.svg", "Insert_Enabled.svg", "Overwrite_Enabled.svg");
	}

	@Override
	public int getSelectedPhase() {
		switch (currentSetting) {
		case "Append":
			return 0;
		case "Overwrite":
			return 2;
		case "Insert":
			return 1;
		default:
			return 0;
		}
	};

	@Override
	public Control mouseDown(Position pos) {
		return openContextMenu(pos);
	}

	@Override
	public Control onContextMenu(Position pos) {
		return openContextMenu(pos);
	}

	private Control openContextMenu(Position pos) {
		Overlay o = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay();

		if (o.getContextMenu() instanceof StoreOptionsContextMenu) {
			o.removeExistingContextMenus();
			return this;
		}

		pos = getPixRect().getLeftTop();
		StoreOptionsContextMenu socm = new StoreOptionsContextMenu(o);
		pos.moveBy(0, -socm.getDesiredHeight() - 2);
		return o.setContextMenu(pos, socm);
	}

	public void update(Node settingsNode) {
		if (ServerProxy.didChange(settingsNode)) {
			String str = ServerProxy.getChildText(settingsNode, "PresetStoreModeSetting", "value");
			if (str != null && !str.isEmpty()) {
				switch (str.toLowerCase()) {
				case "append":
					currentSetting = "Append";
					break;

				case "insert":
					currentSetting = "Insert";
					break;

				case "overwrite":
					currentSetting = "Overwrite";
					break;
				}

				requestLayout();
				invalidate(INVALIDATION_FLAG_UI_CHANGED);
			}
		}
	}

	public double getPictureHeight() {
		return getPhase(getSelectedPhase()).getImgHeight();
	}

	public double getPictureWidth() {
		return getPhase(getSelectedPhase()).getImgWidth();
	}
}
