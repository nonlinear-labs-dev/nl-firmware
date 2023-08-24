package com.nonlinearlabs.client.world.overlay.html.setup;
import java.util.Date;

import com.google.gwt.core.client.GWT;
import com.google.gwt.core.client.UnsafeNativeLong;
import com.google.gwt.uibinder.client.UiBinder;
import com.google.gwt.uibinder.client.UiField;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.Label;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.presenters.DeviceInformationProvider;
import com.nonlinearlabs.client.presenters.DeviceSettingsProvider;
import com.nonlinearlabs.client.useCases.SystemSettings;
import com.nonlinearlabs.client.world.overlay.GWTDialog;

public class DateTimeAdjustmentDialog extends GWTDialog {

    static int popupLeft = 0;
    static int popupTop = 0;
    static DateTimeAdjustmentDialog theDialog;

    public class AdjustmentContent extends Composite {
        @UiField
        Label dateTime;

        @UiField
        Button incYear, decYear, incMonth, decMonth, incDay, decDay, incHour, decHour, incMinute, decMinute;
        
        public AdjustmentContent() {
            initWidget(ourUiBinder.createAndBindUi(this));

            DeviceInformationProvider.get().register(t -> {
                dateTime.setText(t.dateTime);
                return true;
            });


            incYear.addClickHandler(e -> {
                sendAdjustment(1, 0, 0, 0, 0);
            });

            decYear.addClickHandler(e -> {
                sendAdjustment(-1, 0, 0, 0, 0);
            });

            incMonth.addClickHandler(e -> {
                sendAdjustment(0, 1, 0, 0, 0);
            });

            decMonth.addClickHandler(e -> {
                sendAdjustment(0, -1, 0, 0, 0);
            });

            incDay.addClickHandler(e -> {
                sendAdjustment(0, 0, 1, 0, 0);
            });

            decDay.addClickHandler(e -> {
                sendAdjustment(0, 0, -1, 0, 0);
            });

            incHour.addClickHandler(e -> {
                sendAdjustment(0, 0, 0, 1, 0);
            });

            decHour.addClickHandler(e -> {
                sendAdjustment(0, 0, 0, -1, 0);
            });

            incMinute.addClickHandler(e -> {
                sendAdjustment(0, 0, 0, 0, 1);
            });
        
            decMinute.addClickHandler(e -> {
                sendAdjustment(0, 0, 0, 0, -1);
            });
        }

        private void sendAdjustment(long years, long months, long days, long hours, long minutes) {
            long rawOldOffsetInSeconds = DeviceSettingsProvider.get().getPresenter().dateTimeAdjustment;
            var adjustedOffset = adjustTimeStamp(rawOldOffsetInSeconds, years, months, days, hours, minutes);
            SystemSettings.get().setDateTimeAdjustment(adjustedOffset);
        }

        @UnsafeNativeLong
        private native long adjustTimeStamp(long seconds, long years, long months, long days, long hours, long minutes) /*-{
            var date = new Date(seconds * 1000);
            date.setFullYear(date.getFullYear() + years);
            date.setMonth(date.getMonth() + months);
            date.setDate(date.getDate() + days);
            date.setHours(date.getHours() + hours);
            date.setMinutes(date.getMinutes() + minutes);
            var milliseconds = date.getTime();
            return milliseconds / 1000;
        }-*/;
    }

    public interface AdjustContentUiBinder extends UiBinder<HTMLPanel, AdjustmentContent> {
	}

	private static AdjustContentUiBinder ourUiBinder = GWT.create(AdjustContentUiBinder.class);

    public DateTimeAdjustmentDialog() {
        super();
        add(new AdjustmentContent());
        setModal(false);
        addHeader("Adjust Date/Time");
        initalShow();
        pushDialogToFront();
    }

	public static void showDialog() {
		if (theDialog == null) {
			theDialog = new DateTimeAdjustmentDialog();
		}
	}

    @Override
    protected void setLastPopupPos(int popupLeft, int popupTop) {
        DateTimeAdjustmentDialog.popupLeft = popupLeft;
        DateTimeAdjustmentDialog.popupTop = popupTop;
    }

    @Override
    protected int getLastPopupPosTop() {
        return DateTimeAdjustmentDialog.popupTop;
    }

    @Override
    protected int getLastPopupPosLeft() {
        return DateTimeAdjustmentDialog.popupLeft;
    }

	@Override
	protected void commit() {
		hide();
		theDialog = null;
		NonMaps.theMaps.captureFocus();
		NonMaps.theMaps.getNonLinearWorld().requestLayout();
	}
}