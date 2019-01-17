package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Name;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.CachingMapsControl;
import com.nonlinearlabs.NonMaps.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsControl;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.NonRect;

public abstract class ParameterGroupVertical extends LayoutResizingVertical implements ParameterGroupIface {

	private Name name = new Name();
	private NonRect m_viewportPosAtClick;

	public ParameterGroupVertical(MapsLayout parent) {
		super(parent);
	}

	@Override
	public Name getName() {
		return name;
	}

	@Override
	public boolean isLocked() {
		Control c = recurseChildren(new ControlFinder() {
			@Override
			public boolean onWayDownFound(Control ctrl) {
				return (ctrl instanceof Parameter);
			}
		});

		Parameter p = (Parameter) c;
		return p.isLocked();
	}

	@Override
	public <T extends MapsControl> T addChild(T child) {
		if (child instanceof ParameterGroupControls) {
			super.addChild(new CachingMapsControl(this, child));
			return child;
		}

		return super.addChild(child);
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		getPixRect().drawRoundedRect(ctx, Rect.ROUNDING_ALL, toXPixels(6), toXPixels(2), new Gray(26), null);
		super.draw(ctx, invalidationMask);
		getPixRect().drawRoundedRect(ctx, Rect.ROUNDING_ALL, toXPixels(6), toXPixels(2), null,
				getColorModuleHeaderBackground());

	}

	@Override
	public void setName(Name name) {
		this.name.copy(name);
	}

	@Override
	public Control doubleClick() {
		NonRect viewportPosAtClick = NonMaps.theMaps.getNonLinearWorld().getViewport().getNonPosition().copy();

		if (zoomTo(this)) {
			m_viewportPosAtClick = viewportPosAtClick;
		} else if (m_viewportPosAtClick != null) {
			NonMaps.theMaps.getNonLinearWorld().animateViewport(m_viewportPosAtClick, true);
			m_viewportPosAtClick = null;
		}
		getNonMaps().getNonLinearWorld().getPresetManager().resetStoredViewportPosition();
		return this;
	}

	@Override
	public Control onContextMenu(Position pos) {
		return this;
	}
}
