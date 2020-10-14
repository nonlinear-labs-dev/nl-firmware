package com.nonlinearlabs.client.presenters;

import com.nonlinearlabs.client.dataModel.Notifier;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;

public class FadeEditorPresenterProvider extends Notifier<FadeEditorPresenter> {
    private static FadeEditorPresenterProvider theProvider = new FadeEditorPresenterProvider();
    private FadeEditorPresenter presenter = new FadeEditorPresenter();

    public static FadeEditorPresenterProvider get() {
        return theProvider;
    }

    @Override
	public FadeEditorPresenter getValue() {
		return presenter;
    }
    
    public FadeEditorPresenterProvider() {
        for(VoiceGroup vg: new VoiceGroup[]{VoiceGroup.I, VoiceGroup.II}) {
            EditBufferModel.get().getParameter(new ParameterId(396, vg)).onChange(p -> {
                update();
                return true;
            });

            EditBufferModel.get().getParameter(new ParameterId(397, vg)).onChange(p -> {
                update();
                return true;
            });

            EditBufferModel.get().getParameter(new ParameterId(356, vg)).onChange(p -> {
                update();
                return true;
            });
        }
    }

    private void update() {
        EditBufferModel model = EditBufferModel.get();

        BasicParameterModel splitI = model.getParameter(new ParameterId(356, VoiceGroup.I));
        BasicParameterModel splitII = model.getParameter(new ParameterId(356, VoiceGroup.II));

        presenter.splitRangeI.from = 0;
        presenter.splitRangeI.to = Math.max(1, Math.min((int)(61 * splitI.value.value.getValue()) + 1, 61));
        presenter.splitRangeI.indicator = presenter.splitRangeI.to;

        presenter.splitRangeII.from = Math.max(0, Math.min((int)(61 * splitII.value.value.getValue()), 60));
        presenter.splitRangeII.to = 61;
        presenter.splitRangeII.indicator = presenter.splitRangeII.from;


        BasicParameterModel fadeI = model.getParameter(new ParameterId(396, VoiceGroup.I));
        BasicParameterModel fadeII = model.getParameter(new ParameterId(396, VoiceGroup.II));

        presenter.fadePointRangeI.from = 0;
        presenter.fadePointRangeI.to = (int)(61 * fadeI.value.value.getValue());
        presenter.fadePointRangeI.indicator = presenter.fadePointRangeI.to;

        presenter.fadePointRangeII.from = (int)(61 * fadeII.value.value.getValue());
        presenter.fadePointRangeII.to = 61;
        presenter.fadePointRangeII.indicator = presenter.fadePointRangeII.from;

        BasicParameterModel rangeI = model.getParameter(new ParameterId(397, VoiceGroup.I));
        BasicParameterModel rangeII = model.getParameter(new ParameterId(397, VoiceGroup.II));

        double rI = Math.min(presenter.fadePointRangeI.to + (rangeI.value.value.getValue() * 61), 61);
        double rII = Math.max(presenter.fadePointRangeII.from - (rangeII.value.value.getValue() * 61), 0);

        presenter.fadeRangeI = rI;
        presenter.fadeRangeII = rII;

        notifyChanges();
    }
}
