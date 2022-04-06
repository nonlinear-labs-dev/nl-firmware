package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.NonDimension;
import com.nonlinearlabs.client.world.maps.NonRect;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.PhysicalControlParameter;

public class PhysicalControlSendParameter extends PhysicalControlParameter {

    private boolean wasLastReceivedVisible = false;
    public PhysicalControlSendParameter(MapsLayout parent, int parameterID) {
        super(parent, parameterID);
    }

    @Override
    protected boolean onPresenterUpdated(ParameterPresenter p)
    {
        super.onPresenterUpdated(p);
        if(wasLastReceivedVisible != p.hidden) {
            getParent().invalidate(INVALIDATION_FLAG_SCROLLED);
            getParent().requestLayout();
        }
        wasLastReceivedVisible = p.hidden;
        return true;
    }

    private int getSibling()
    {
        switch(presenter.id.getNumber())
        {
            case 398:
                return 254;
            case 399:
                return 259;
            case 400:
                return 264;
            case 401:
                return 269;
            case 402:
                return 274;
            case 403:
                return 279;
            case 404:
                return 284;
            case 405:
                return 289;
        }
        return -1;
    }

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
		if (presenter.hidden) {
            Parameter sibling = NonMaps.theMaps.getNonLinearWorld().getParameterEditor().findParameter(getSibling());
            sibling.doFirstLayoutPass(levelOfDetail);
            NonRect dim = sibling.getNonPosition();
            setNonSize(new NonDimension(0, dim.getHeight()));
		} else {
			super.doFirstLayoutPass(levelOfDetail);
		}
	}
}
