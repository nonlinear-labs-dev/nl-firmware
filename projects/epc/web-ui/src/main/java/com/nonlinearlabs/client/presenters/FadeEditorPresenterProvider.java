package com.nonlinearlabs.client.presenters;

import com.nonlinearlabs.client.dataModel.Notifier;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.presenters.FadeEditorPresenter.SelectedFadeControl;

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

        EditBufferModel.get().selectedParameter.onChange(paramNum -> {
            switch(paramNum) {
                case 356:
                    presenter.selectedControl = SelectedFadeControl.Split;
                break;
                case 396:
                    presenter.selectedControl = SelectedFadeControl.FadePoint;
                break;
                case 397:
                    presenter.selectedControl = SelectedFadeControl.FadeRange;
                break;
            }
            notifyChanges();
            return true;
        });
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
        presenter.fadePointI = Math.max(Math.min(fadeI.value.value.getValue(), 1), 0);
        presenter.fadePointII = Math.max(Math.min(fadeII.value.value.getValue(), 1), 0);

        notifyChanges();
    }
}
