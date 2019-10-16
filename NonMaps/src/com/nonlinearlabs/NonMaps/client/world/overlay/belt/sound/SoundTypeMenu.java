package com.nonlinearlabs.NonMaps.client.world.overlay.belt.sound;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.NonMaps.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public class SoundTypeMenu extends SVGImage {

	public class SoundTypeMenuContextMenu extends ContextMenu {

		public SoundTypeMenuContextMenu(OverlayLayout parent) {
			super(parent);

			EditBufferModel.SoundType currentSetting = EditBufferModel.get().soundType.getValue();

			if (currentSetting != EditBufferModel.SoundType.Single) {
				addChild(new ContextMenuItem(this, "Single") {
					@Override
					public Control click(Position eventPoint) {
						EditBufferUseCases.get().convertToSingleSound();
						return super.click(eventPoint);
					}
				});
			}

			if (currentSetting != EditBufferModel.SoundType.Split) {
				addChild(new ContextMenuItem(this, "Split") {
					@Override
					public Control click(Position eventPoint) {
						EditBufferUseCases.get().convertToSplitSound();
						return super.click(eventPoint);
					}
				});
			}

			if (currentSetting != EditBufferModel.SoundType.Layer) {
				addChild(new ContextMenuItem(this, "Layer") {
					@Override
					public Control click(Position eventPoint) {
						EditBufferUseCases.get().convertToLayerSound();
						return super.click(eventPoint);
					}
				});
			}
		}
	}

	public SoundTypeMenu(OverlayLayout parent) {
		super(parent, "Append_Enabled.svg", "Insert_Enabled.svg", "Overwrite_Enabled.svg");
	}

	@Override
	public int getSelectedPhase() {
		switch (EditBufferModel.get().soundType.getValue()) {
		case Single:
			return 0;
		case Split:
			return 1;
		case Layer:
			return 2;
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

		if (o.getContextMenu() instanceof SoundTypeMenuContextMenu) {
			o.removeExistingContextMenus();
			return this;
		}

		pos = getPixRect().getLeftTop();
		SoundTypeMenuContextMenu socm = new SoundTypeMenuContextMenu(o);
		pos.moveBy(0, -socm.getDesiredHeight() - 2);
		return o.setContextMenu(pos, socm);
	}

	public double getPictureHeight() {
		return getPhase(getSelectedPhase()).getImgHeight();
	}

	public double getPictureWidth() {
		return getPhase(getSelectedPhase()).getImgWidth();
	}
}
