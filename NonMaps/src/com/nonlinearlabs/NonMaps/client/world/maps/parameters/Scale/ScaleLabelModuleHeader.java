package com.nonlinearlabs.NonMaps.client.world.maps.parameters.Scale;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Name;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupVertical;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.setup.ContextMenusSetting;

public class ScaleLabelModuleHeader extends LabelModuleHeader {

	public ScaleLabelModuleHeader(ParameterGroupVertical parent, Name name) {
		super(parent, name);
	}

	public class ScaleGroupContextMenu extends ParameterGroupContextMenu {

		public ScaleGroupContextMenu(OverlayLayout parent, Scale scaleGroup) {
			super(parent);
			if (scaleGroup.anyValueNotDefault()) {
				addChild(new ContextMenuItem(this, "Reset") {
					@Override
					public Control click(Position eventPoint) {
						resetScaling();
						return super.click(eventPoint);
					}
				});
			}
		}
	}

	@Override
	public Control onContextMenu(Position pos) {
		ContextMenusSetting contextMenuSettings = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay().getSetup()
				.getContextMenuSettings();
		if (contextMenuSettings.isEnabled()) {
			Overlay o = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay();
			return o.setContextMenu(pos, new ScaleGroupContextMenu(o, (Scale) getParent()));
		}
		return super.onContextMenu(pos);
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		super.draw(ctx, invalidationMask);

		Rect pixRect = getPixRect();

		if (hasChangedOffsets()) {
			double fontHeightInUnits = getFontHeight();
			double fontHeightInPixels = toYPixels(fontHeightInUnits) * 0.595;
			ctx.setFont(fontHeightInPixels + "px " + getFontName());
			ctx.fillText("\ue170", pixRect.getLeft() + toXPixels(15), pixRect.getCenterPoint().getY() + toYPixels(moveFontVerticallyBy()));
		}
	}

	private boolean hasChangedOffsets() {
		return ((Scale) getParent()).anyValueNotDefault();
	}

}
