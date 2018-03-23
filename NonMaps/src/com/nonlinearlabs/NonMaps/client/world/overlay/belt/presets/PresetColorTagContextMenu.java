package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.ColorTag;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class PresetColorTagContextMenu extends ContextMenu {

	private final Preset preset;

	private void setColor(ColorTag.Color color) {
		final PresetManager pm = preset.getParent().getParent();
		final boolean hasMultipleSelection = pm.hasMultiplePresetSelection();

		if (hasMultipleSelection) {
			for (String uuid : pm.getMultiSelection().getSelectedPresets()) {
				Preset p = pm.findPreset(uuid);
				NonMaps.get().getServerProxy().setPresetAttribute(p, "color", color.toString());
			}
		} else {
			NonMaps.get().getServerProxy().setPresetAttribute(preset, "color", color.toString());
		}
	}

	public PresetColorTagContextMenu(OverlayLayout parent, final Preset p) {
		super(parent);
		this.preset = p;

		for (ColorTag.Color c : ColorTag.Color.values()) {
			String name = c.toString();
			name = name.substring(0, 1).toUpperCase() + name.substring(1);
			addChild(new ContextMenuItem((ContextMenu) this, name) {
				@Override
				public Control click(Position eventPoint) {
					setColor(c);
					NonMaps.get().getNonLinearWorld().getViewport().getOverlay().removeExistingContextMenus();
					endMultiSelection();
					return super.click(eventPoint);
				}

				@Override
				public void draw(Context2d ctx, int invalidationMask) {
					super.draw(ctx, invalidationMask);
					Rect r = this.getPixRect().copy();
					r.setWidth(getColorTagWidth());
					r.reduceHeightBy(r.getHeight() / 4);
					r.fill(ctx, c.toRGB());
				}

				public double getColorTagWidth() {
					return Millimeter.toPixels(1.5);
				}

				@Override
				public double getLeftPadding() {
					return super.getLeftPadding() + getColorTagWidth();
				}
			});
		}
	}
}
