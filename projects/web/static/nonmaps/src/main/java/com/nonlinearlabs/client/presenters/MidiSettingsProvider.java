package com.nonlinearlabs.client.presenters;

import java.util.LinkedList;
import java.util.function.Function;

import com.google.gwt.core.client.GWT;
import com.nonlinearlabs.client.dataModel.Notifier;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.SystemSettings;
import com.nonlinearlabs.client.presenters.MidiSettings.AftertouchMapping;
import com.nonlinearlabs.client.presenters.MidiSettings.BenderMapping;
import com.nonlinearlabs.client.presenters.MidiSettings.PedalMapping;
import com.nonlinearlabs.client.presenters.MidiSettings.RibbonMapping;
import com.nonlinearlabs.client.presenters.MidiSettings.RoutingSetting;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BooleanValues;

public class MidiSettingsProvider extends Notifier<MidiSettingsPresenter> {
	public static MidiSettingsProvider theInstance = new MidiSettingsProvider();

	public static MidiSettingsProvider get() {
		return theInstance;
	}

	private LinkedList<Function<MidiSettings, Boolean>> clients = new LinkedList<Function<MidiSettings, Boolean>>();
	private MidiSettings settings = new MidiSettings();
    private MidiSettingsPresenter presenter = new MidiSettingsPresenter();
    private int selectedParameterId = -1;

	private MidiSettingsProvider() {
		SystemSettings s = SetupModel.get().systemSettings;

        //Send
        s.sendChannel.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.sendChannel.selected != newValue) {
                settings.sendChannel.selected = t.ordinal();
                notifyClients();
            }
            return true;
        });

        s.sendChannelSplit.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.sendChannelSplit.selected != newValue) {
                settings.sendChannelSplit.selected = newValue;
                notifyClients();
            }
            return true;
        });    

        //Receive
        s.receiveChannel.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.receiveChannel.selected != newValue) {
                settings.receiveChannel.selected = newValue;
                notifyClients();
            }
            return true;
        });

        s.receiveChannelSplit.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.receiveChannelSplit.selected != newValue) {
                settings.receiveChannelSplit.selected = newValue;
                notifyClients();
            }
            return true;
        });

        s.receiveAftertouchCurve.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.receiveAftertouchCurve.selected != newValue) {
                settings.receiveAftertouchCurve.selected = newValue;   
                notifyClients();
            }
            return true;
        });

        s.receiveVelocityCurve.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.receiveVelocityCurve.selected != newValue) {
                settings.receiveVelocityCurve.selected = newValue;
                notifyClients();
            }
            return true;
        });

        s.pedal1Mapping.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.pedalMapping1.selected != newValue) {
                settings.pedalMapping1.selected = newValue;
                notifyClients();
            }
            return true;
        });

        s.pedal2Mapping.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.pedalMapping2.selected != newValue) {
                settings.pedalMapping2.selected = newValue;
                notifyClients();
            }
            return true;
        });

        s.pedal3Mapping.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.pedalMapping3.selected != newValue) {
                settings.pedalMapping3.selected = newValue;
                notifyClients();
            }
            return true;
        });

        s.pedal4Mapping.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.pedalMapping4.selected != newValue) {
                settings.pedalMapping4.selected = newValue;
                notifyClients();
            }
            return true;
        });

        s.ribbon1Mapping.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.ribbonMapping1.selected != newValue) {
                settings.ribbonMapping1.selected = newValue;
                notifyClients();
            }
            return true;
        });

        s.ribbon2Mapping.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.ribbonMapping2.selected != newValue) {
                settings.ribbonMapping2.selected = newValue;
                notifyClients();
            }
            return true;
        });

        s.ribbon3Mapping.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.ribbonMapping3.selected != newValue) {
                settings.ribbonMapping3.selected = newValue;
                notifyClients();
            }
            return true;
        });

        s.ribbon4Mapping.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.ribbonMapping4.selected != newValue) {
                settings.ribbonMapping4.selected = newValue;
                notifyClients();
            }
            return true;
        });

        s.benderMapping.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.benderMapping.selected != newValue) {
                settings.benderMapping.selected = newValue;
                notifyClients();
            }
            return true;
        });

        s.aftertouchMapping.onChange(t -> {
            int newValue = t.ordinal();
            if(settings.aftertouchMapping.selected != newValue) {
                settings.aftertouchMapping.selected = newValue;
                notifyClients();
            }
            return true;
        });

        s.highVelocityCC.onChange(t -> {
            boolean val = t.equals(BooleanValues.on);
            if(settings.highVelocityCC.value != val) {
                settings.highVelocityCC.value = val;
                notifyClients();
            }
            return true;
        });
	
        s.enable14BitSupport.onChange(t -> {
            boolean val = t.equals(BooleanValues.on);
            if(settings.enable14BitCC.value != val) {
                settings.enable14BitCC.value = val;
                notifyClients();
            }
            return true;
        });

        s.autoStartRecorder.onChange(t -> {
            boolean val = t.equals(BooleanValues.on);
            if(settings.autoStartRecorder.value != val) {
                settings.autoStartRecorder.value = val;
                notifyClients();
            }
            return true;
        });

        s.routingAspects.onChange(t -> {
            RoutingSetting d = MidiSettings.convert(t);
            if(d != settings.routingSetting) {
                settings.routingSetting = d;
                notifyClients();
            }
            return true;
        });

        s.localEnabled.onChange(t -> {
            boolean val = t.equals(BooleanValues.on);
            if(val != settings.localEnabled.value) {
                settings.localEnabled.value = val;
                notifyClients();
            }
            return true;
        });

        EditBufferModel.get().selectedParameter.onChange(i -> {
            selectedParameterId = i;
            notifyClients();
            return true;
        });
    }

	protected void notifyClients() {
        updatePresenter();
        notifyChanges();
		clients.removeIf(listener -> !listener.apply(settings));
	}

    private String cleanUpNoneStrings(String n)
    {
        if(n.equals("None"))
            return "";
        return n;
    }

    private void updatePresenter() {
        if(settings.enable14BitCC.value) 
        {
            presenter.pedal1CCString = cleanUpNoneStrings(PedalMapping.options[settings.pedalMapping1.selected]);
            presenter.pedal2CCString = cleanUpNoneStrings(PedalMapping.options[settings.pedalMapping2.selected]);
            presenter.pedal3CCString = cleanUpNoneStrings(PedalMapping.options[settings.pedalMapping3.selected]);
            presenter.pedal4CCString = cleanUpNoneStrings(PedalMapping.options[settings.pedalMapping4.selected]);
            presenter.ribbon1CCString = cleanUpNoneStrings(RibbonMapping.options[settings.ribbonMapping1.selected]);
            presenter.ribbon2CCString = cleanUpNoneStrings(RibbonMapping.options[settings.ribbonMapping2.selected]);
            presenter.benderCCString = cleanUpNoneStrings(BenderMapping.options[settings.benderMapping.selected]);
            presenter.aftertouchCCString = cleanUpNoneStrings(AftertouchMapping.options[settings.aftertouchMapping.selected]);
        }
        else {
            presenter.pedal1CCString = cleanUpNoneStrings(PedalMapping.optionsWithoutLSB[settings.pedalMapping1.selected]);
            presenter.pedal2CCString = cleanUpNoneStrings(PedalMapping.optionsWithoutLSB[settings.pedalMapping2.selected]);
            presenter.pedal3CCString = cleanUpNoneStrings(PedalMapping.optionsWithoutLSB[settings.pedalMapping3.selected]);
            presenter.pedal4CCString = cleanUpNoneStrings(PedalMapping.optionsWithoutLSB[settings.pedalMapping4.selected]);
            presenter.ribbon1CCString = cleanUpNoneStrings(RibbonMapping.optionsWithoutLSB[settings.ribbonMapping1.selected]);
            presenter.ribbon2CCString = cleanUpNoneStrings(RibbonMapping.optionsWithoutLSB[settings.ribbonMapping2.selected]);
            presenter.benderCCString = cleanUpNoneStrings(BenderMapping.optionsWithoutLSB[settings.benderMapping.selected]);
            presenter.aftertouchCCString = cleanUpNoneStrings(AftertouchMapping.optionsWithoutLSB[settings.aftertouchMapping.selected]);
        }

        presenter.selectedParameterCCString = presenter.getCCStringFor(selectedParameterId);
    }

    public void register(Function<MidiSettings, Boolean> cb) {
		clients.add(cb);
		cb.apply(settings);
	}

	public MidiSettings getSettings() {
		return settings;
	}

    @Override
    public MidiSettingsPresenter getValue() {
        return presenter;
    }
}
