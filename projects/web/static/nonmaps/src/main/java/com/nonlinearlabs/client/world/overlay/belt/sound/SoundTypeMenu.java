package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.ContextMenu;
import com.nonlinearlabs.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.client.world.overlay.Overlay;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class SoundTypeMenu extends SVGImage {

	public class SoundTypeMenuContextMenu extends ContextMenu {

		private void addSingleChild(String suffix) {
			addChild(new ContextMenuItem(this, "Convert to Single" + suffix) {
				@Override
				public Control click(Position eventPoint) {
					EditBufferUseCases.get().convertToSingleSound();
					return super.click(eventPoint);
				}
			});
		} 

		private void addLayerChild(String suffix) {
			addChild(new ContextMenuItem(this, "Convert to Layer" + suffix) {
				@Override
				public Control click(Position eventPoint) {
					EditBufferUseCases.get().convertToLayerSound();
					return super.click(eventPoint);
				}
			});
		}

		private void addSplitChild(String suffix) {
			addChild(new ContextMenuItem(this, "Convert to Split" + suffix) {
				@Override
				public Control click(Position eventPoint) {
					EditBufferUseCases.get().convertToSplitSound();
					return super.click(eventPoint);
				}
			});
		}

		private void addSplitChildWithFX() {
			addChild(new ContextMenuItem(this, "Convert to Split (FX I only)") {
				@Override
				public Control click(Position eventPoint) {
					EditBufferUseCases.get().convertToSplitWithFXIOnly();
					return super.click(eventPoint);
				}
			});
		}

		private void addLayerChildWithFX() {
			addChild(new ContextMenuItem(this, "Convert to Layer (FX I only)") {
				@Override
				public Control click(Position eventPoint) {
					EditBufferUseCases.get().convertToLayerWithFXIOnly();
					return super.click(eventPoint);
				}
			});
		}

		public SoundTypeMenuContextMenu(OverlayLayout parent) {
			super(parent);

			EditBufferModel.SoundType currentSetting = EditBufferModel.get().soundType.getValue();


			if (currentSetting == EditBufferModel.SoundType.Single) {
				addLayerChildWithFX();
				addLayerChild(" (both FX)");
				addSplitChildWithFX();
				addSplitChild(" (both FX)");
			}

			if (currentSetting == EditBufferModel.SoundType.Layer) {
				addSingleChild(" (both FX)");
				addSplitChild("");
			}

			if (currentSetting == EditBufferModel.SoundType.Split) {
				addSingleChild(" (both FX)");
				addLayerChild("");
			}
		}
	}

	public SoundTypeMenu(OverlayLayout parent) {
		super(parent, "Menu_Single_Enabled_L.svg", "Menu_Split_Enabled_L.svg", "Menu_Layer_Enabled_L.svg");
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
