package com.nonlinearlabs.client.world.overlay.belt;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.IPreset;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.overlay.DragProxy;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.belt.Belt.BeltTab;

public class SoundBeltButton extends BeltButton {

	private Belt m_belt;

	public SoundBeltButton(OverlayLayout parent, Belt belt) {
		super(parent, belt, "Sound_Tab_Enabled_A.svg", "Sound_Tab_Disabled_A.svg");
		m_belt = belt;
	}

	@Override
	public Control click(Position eventPoint) {
		if (!isActive()) {
			belt.setSoundView(true);
		} else {
			belt.toggle();
		}
		return this;
	}

	@Override
	public boolean isActive() {
		return belt.isSoundView();
	}


	@Override
	public Control drag(Position p, DragProxy dragProxy) {
		if(getPixRect().contains(p) && dragProxy.getOrigin() instanceof IPreset)
		{
			if(m_belt.isHidden())
				m_belt.toggle();
				
			m_belt.openTab(BeltTab.Sound);
			return this;
		}
		return super.drag(p, dragProxy);
	}
}
