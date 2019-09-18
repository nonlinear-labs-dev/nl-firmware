package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.BooleanValues;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupVertical;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PhysicalControlParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources.Pedal;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class SourcesAndAmounts extends ParameterGroupVertical {

	private SourcesAndAmountsControls controls;

	public class MacroControlMappingsGroupContextMenu extends ContextMenu {
		public MacroControlMappingsGroupContextMenu(OverlayLayout parent) {
			super(parent);

			addChild(new ContextMenuItem(this, (isHWLocked() ? "Unlock" : "Lock") + " Hardware Sources") {
				@Override
				public Control click(Position eventPoint) {
					NonMaps.get().getServerProxy().toggleGroupLock("CS");
					return super.click(eventPoint);
				}
			});

			addChild(new ContextMenuItem(this, (isAmountsLocked() ? "Unlock" : "Lock") + " Amounts") {
				@Override
				public Control click(Position eventPoint) {
					NonMaps.get().getServerProxy().toggleGroupLock("MCM");
					return super.click(eventPoint);
				}
			});

			addChild(new ContextMenuItem(this, "Lock all groups") {
				@Override
				public Control click(Position eventPoint) {
					NonMaps.get().getServerProxy().lockAllGroups();
					return super.click(eventPoint);
				}
			});

			if (NonMaps.get().getNonLinearWorld().getParameterEditor().isAnyParameterLocked()) {
				addChild(new ContextMenuItem(this, "Unlock all groups") {
					@Override
					public Control click(Position eventPoint) {
						NonMaps.get().getServerProxy().unlockAllGroups();
						return super.click(eventPoint);
					}
				});
			}
		}
	}

	public SourcesAndAmounts(MapsLayout parent) {
		super(parent);
		addChild(new LabelModuleHeader(this, "Hardware Sources and Amounts") {

			@Override
			public Control onContextMenu(Position pos) {
				boolean showContextMenus = Setup.get().localSettings.contextMenus.getValue() == BooleanValues.on;

				if (showContextMenus) {
					Overlay o = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay();
					ContextMenu c = new MacroControlMappingsGroupContextMenu(o);
					return o.setContextMenu(pos, c);
				}
				return super.onContextMenu(pos);
			}

			@Override
			protected boolean isLocked() {
				return super.isLocked();
			}

			@Override
			public void draw(Context2d ctx, int invalidationMask) {
				super.draw(ctx, invalidationMask);

				Rect pixRect = getPixRect();

				if (isHWLocked())
					ctx.fillText(getSourcesLockSymbol(),
							pixRect.getLeft() + ctx.measureText(getSourcesLockSymbol()).getWidth() * 0.75,
							pixRect.getCenterPoint().getY() + toYPixels(moveFontVerticallyBy()));
			}

			private String getSourcesLockSymbol() {
				return "\ue20c";
			}

			private String getAmountLockSymbol() {
				return "\ue20b";
			}

			@Override
			protected String getLockSymbol() {
				return getAmountLockSymbol();
			}

			@Override
			protected float getLockSymbolOffsetFactor() {
				return 0.75f;
			}

			@Override
			public RGB getColorFont() {
				return RGB.lighterGray();
			}
		});

		addChild(controls = new SourcesAndAmountsControls(this));
	}

	@Override
	public String getID() {
		return "MCM";
	}

	public void onReturningModeChanged(PhysicalControlParameter src) {
		controls.onReturningModeChanged(src);
	}

	public Pedal getPedal(int i) {
		return controls.getPedal(i);
	}

	private boolean isAmountsLocked() {
		return controls.isAmountsLocked();
	}

	private boolean isHWLocked() {
		return controls.isHWLocked();
	}
}
