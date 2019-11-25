package com.nonlinearlabs.client.world.overlay.belt;

import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class ParameterCompareButton extends SVGImage {

	public ParameterCompareButton(Control parent) {
		super(parent, "PreRecall_B_Enabled.svg", "PreRecall_B_Active.svg", "PreRecall_B_Disabled.svg");
	}

	public static boolean inCompare = false;

	@Override
	public int getSelectedPhase() {
		if (EditBufferPresenterProvider.getPresenter().isAnyParameterChanged) {
			if (inCompare) {
				return 1;
			} else {
				return 0;
			}
		} else {
			if (inCompare) {
				return 1;
			}
		}

		return 2;
	}

	@Override
	public Control click(Position eventPos) {
		if (inCompare || EditBufferPresenterProvider.getPresenter().isAnyParameterChanged) {
			inCompare = !inCompare;
		}
		return this;
	}

}
