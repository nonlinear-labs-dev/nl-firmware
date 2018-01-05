package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.event.dom.client.KeyDownEvent;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.world.AppendOverwriteInsertPresetDialog.Action;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.RenameDialog;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterEditor;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImagePhase;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.EditBufferDraggingButton;

class StorePreset extends SVGImage {

	Action action = Action.APPEND;
	String presetToWaitFor = "";

	class DraggableButtonPhase extends SVGImagePhase implements EditBufferDraggingButton {

		public DraggableButtonPhase(SVGImage parent, String imageName) {
			super(parent, imageName);
		}

	}

	StorePreset(OverlayLayout parent) {
		super(parent, "Store_Enabled.svg", "Store_Drag.svg", "Store_Disabled.svg");
	}

	@Override
	public int getSelectedPhase() {
		if (isCaptureControl() || isDraggingControl()) {
			return 1;
		}

		return 0;
	}

	@Override
	protected SVGImagePhase createPhase(String s) {
		return new DraggableButtonPhase(this, s);
	}

	@Override
	public Control startDragging(Position eventPoint) {
		return getNonMaps().getNonLinearWorld().getViewport().getOverlay().createDragProxy(getPhase(1), getPixRect().getPosition());
	}

	@Override
	public Control click(Position eventPoint) {
		boolean hasSelectedBank = getPresetManager().hasSelectedBank();

		if (!hasSelectedBank)
			createNewBank();
		else
			storeToBank();

		return this;
	}

	protected PresetManager getPresetManager() {
		return getNonMaps().getNonLinearWorld().getPresetManager();
	}

	public void storeToBank() {
		String uuid = "";
		ParameterEditor ed = NonMaps.theMaps.getNonLinearWorld().getParameterEditor();
		boolean isModified = ed.isModified();

		switch (action) {
		case APPEND:
			uuid = getNonMaps().getServerProxy().appendPreset();
			break;

		case INSERT:
			uuid = getNonMaps().getServerProxy().insertPreset();
			break;

		case OVERWRITE:
			getNonMaps().getServerProxy().overwritePreset();
			break;

		default:
			break;
		}

		if (isModified && !uuid.isEmpty())
			showRename(uuid);
	}

	private void showRename(String uuid) {
		presetToWaitFor = uuid;
	}

	protected void createNewBank() {
		getNonMaps().getServerProxy().newBankFromEditBuffer(getNonMaps().getNonLinearWorld().getNonPosition().getCenterPoint());
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		if (isDraggingControl() && !isVisibilityForced()) {
			drawDetachedState(ctx);
		} else {
			super.draw(ctx, invalidationMask);
		}
	}

	protected void drawDetachedState(Context2d ctx) {
		Rect r = getPixRect().copy();
		double reduced = 0;
		r.reduceHeightBy(reduced);
		r.reduceWidthBy(reduced);
		r.drawRoundedArea(ctx, 2, 1, new Gray(26), new Gray(0));
	}

	@Override
	public Control onKey(KeyDownEvent event) {
		if (event.getNativeKeyCode() == com.google.gwt.event.dom.client.KeyCodes.KEY_S && event.isControlKeyDown()) {
			storeToBank();
			return this;
		}
		return super.onKey(event);
	}

	public void update(Node settingsNode, Node presetManagerNode) {
		if (ServerProxy.didChange(settingsNode)) {
			String str = ServerProxy.getChildText(settingsNode, "PresetStoreModeSetting", "value");
			if (str != null && !str.isEmpty()) {
				switch (str.toLowerCase()) {
				case "append":
					action = Action.APPEND;
					break;

				case "insert":
					action = Action.INSERT;
					break;

				case "overwrite":
					action = Action.OVERWRITE;
					break;
				}

				invalidate(INVALIDATION_FLAG_UI_CHANGED);
			}
		}

		if (!presetToWaitFor.isEmpty() && ServerProxy.didChange(presetManagerNode)) {
			Preset p = NonMaps.theMaps.getNonLinearWorld().getPresetManager().findPreset(presetToWaitFor);
			if (p != null) {
				presetToWaitFor = "";
				RenameDialog.open(getNonMaps(), p);
			}
		}
	}

}
