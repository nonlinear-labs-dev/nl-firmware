package com.nonlinearlabs.client.world.overlay.html.setup;

import com.google.gwt.core.client.GWT;
import com.google.gwt.uibinder.client.UiBinder;
import com.google.gwt.uibinder.client.UiField;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.ListBox;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.TestToneTypeSetting;
import com.nonlinearlabs.client.presenters.DeviceInformationProvider;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.TestToneType;
import com.nonlinearlabs.client.presenters.DeviceSettings.TestToneTypePresenter;
import com.nonlinearlabs.client.useCases.SystemSettings;
import com.nonlinearlabs.client.presenters.DeviceSettingsProvider;
import com.nonlinearlabs.client.world.overlay.GWTDialog;

public class ToggleTestToneDialog extends GWTDialog {

    static int lastPopupPosTop = 0;
    static int lastPopupPosLeft = 0;
    static ToggleTestToneDialog theDialog;

    public ToggleTestToneDialog() {
        super();
        add(new ToggleTestToneContent());
        setModal(false);
        addHeader("Test Tone");
        initalShow();
        pushDialogToFront();
    }

    @Override
    protected void setLastPopupPos(int popupLeft, int popupTop) {
        lastPopupPosLeft = popupLeft;
        lastPopupPosTop = popupTop;
    }

    @Override
    protected int getLastPopupPosTop() {
        return lastPopupPosTop;
    }

    @Override
    protected int getLastPopupPosLeft() {
        return lastPopupPosLeft;
    }

    public static void showDialog() {
		if (theDialog == null) {
			theDialog = new ToggleTestToneDialog();
		}
	}

    @Override
    protected void commit() {
        hide();

        SystemSettings.get().setTestTone(TestToneType.Off);

		theDialog = null;
		NonMaps.theMaps.captureFocus();
		NonMaps.theMaps.getNonLinearWorld().requestLayout();
    }

    public class ToggleTestToneContent extends Composite {
        @UiField
        ListBox list;

        public ToggleTestToneContent() {
            initWidget(uiBinder.createAndBindUi(this));

            DeviceSettingsProvider.get().register(t -> {
                list.clear();
                for (String s : TestToneTypePresenter.options)
                    list.addItem(s);
                list.setSelectedIndex(t.testToneType.selected);
                return true;
            });

            list.addChangeHandler(t -> {
                SystemSettings.get().setTestTone(TestToneType.values()[list.getSelectedIndex()]);
            });
        }

    }

    public interface ToggleToneContentUiBinder extends UiBinder<HTMLPanel, ToggleTestToneContent> {
	}

    private static ToggleToneContentUiBinder uiBinder = GWT.create(ToggleToneContentUiBinder.class);
}
