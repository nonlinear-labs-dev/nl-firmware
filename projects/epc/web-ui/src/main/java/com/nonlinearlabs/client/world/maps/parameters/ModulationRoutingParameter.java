package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulationRouterParameterModel;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.presenters.ParameterPresenterProviders;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public class ModulationRoutingParameter extends Parameter {

	private ParameterPresenter physicalControlParameterPresenter;

	public ModulationRoutingParameter(MapsLayout parent, int paramID) {
		super(parent, paramID);

		ModulationRouterParameterModel p = (ModulationRouterParameterModel) EditBufferModel.get()
				.getAnyParameter(paramID);

		ParameterPresenterProviders.get().registerForCurrentVoiceGroup(p.getAssociatedPhysicalControlID().getNumber(),
				v -> {
					physicalControlParameterPresenter = v;
					onReturningModeChanged();
					return true;
				});
	}

	@Override
	protected int getBackgroundRoundings() {
		return Rect.ROUNDING_NONE;
	}

	protected void setupChildren(boolean returning) {
		removeAll();

		if (returning) {
			addChild(new Spacer(this) {
				@Override
				protected double getBasicHeight() {
					return 20;
				}

				@Override
				protected double getMinHeight() {
					return 10;
				}

				@Override
				public double getLevelOfDetailForFullVisibility() {
					return getParent().getLevelOfDetailForFullVisibility() + 1;
				}
			});

			addChild(new SliderHorizontal(this, getParameterNumber()) {
				@Override
				protected double getBasicWidth() {
					return 120;
				}

				@Override
				protected double getBasicHeight() {
					return 9;
				}
			});

			addChild(new ValueDisplaySmall(this, getParameterNumber()));
		} else {
			addChild(createRoutingButton());
		}

		requestLayout();
	}

	protected ModulationRoutingButton createRoutingButton() {
		return new ModulationRoutingButton(this, getParameterNumber());
	}

	public void onReturningModeChanged() {
		setupChildren(physicalControlParameterPresenter.isReturning);
	}

}