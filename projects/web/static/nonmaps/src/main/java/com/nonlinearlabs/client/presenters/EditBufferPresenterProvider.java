package com.nonlinearlabs.client.presenters;

import com.google.gwt.core.client.Scheduler;
import com.google.gwt.core.client.Scheduler.ScheduledCommand;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.Notifier;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.MacroControlParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.presenters.EditBufferPresenter.GenericArrowEnum;
import com.nonlinearlabs.client.presenters.EditBufferPresenter.SerialFX;
import com.nonlinearlabs.client.presenters.EditBufferPresenter.SingleSoundFBToPoly;
import com.nonlinearlabs.client.presenters.EditBufferPresenter.SingleSoundFXToOut;
import com.nonlinearlabs.client.presenters.EditBufferPresenter.SingleSoundPolyToFX;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterFactory;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.dataModel.editBuffer.PhysicalControlParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ScaleOffsetParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.SendParameterModel;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.RGBA;
import com.nonlinearlabs.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;

public class EditBufferPresenterProvider extends Notifier<EditBufferPresenter> {
    private static EditBufferPresenterProvider theProvider = new EditBufferPresenterProvider();
    private EditBufferPresenter presenter = new EditBufferPresenter();
    private boolean bruteForceScheduled = false;
    private int allParameterSubscription = 0;
    private int selectedParameterSubscription = 0;

    private EditBufferModel model;

    private EditBufferPresenterProvider() {
        model = EditBufferModel.get();

        model.voiceGroup.onChange(v -> {
            monitorSelectedParameter();

            presenter.voiceGroupEnum = v;

            switch (v) {
                case Global:
                    presenter.voiceGroup = "";
                    presenter.voiceGroupIndicationColor = RGBA.transparent();
                    presenter.voiceGroupI_ForegroundColor = RGBA.transparent();
                    presenter.voiceGroupII_ForegroundColor = RGBA.transparent();
                    presenter.voiceGroupI_BackgroundColor = RGBA.transparent();
                    presenter.voiceGroupII_BackgroundColor = RGBA.transparent();
                    break;

                case I:
                    presenter.voiceGroup = "\uE071";
                    presenter.voiceGroupIndicationColor = new RGB(0x26, 0xb0, 0xff);
                    presenter.voiceGroupI_ForegroundColor = new RGB(0x26, 0xb0, 0xff);
                    presenter.voiceGroupII_ForegroundColor = new Gray(0x08);
                    presenter.voiceGroupI_BackgroundColor = new Gray(0x66);
                    presenter.voiceGroupII_BackgroundColor = new Gray(0x4c);
                    break;

                case II:
                    presenter.voiceGroup = "\uE072";
                    presenter.voiceGroupIndicationColor = new RGB(0xff, 0x99, 0x33);
                    presenter.voiceGroupI_ForegroundColor = new Gray(0x08);
                    presenter.voiceGroupII_ForegroundColor = new RGB(0xff, 0x99, 0x33);
                    presenter.voiceGroupI_BackgroundColor = new Gray(0x4c);
                    presenter.voiceGroupII_BackgroundColor = new Gray(0x66);
                    break;
            }

            presenter.currentPartName = model.getPresetNameOfVoiceGroup(v);
            notifyChanges();
            return true;
        });

        model.onChange(v -> {
            monitorAllParameters();
            return true;
        });

        model.loadedPresetInVG1.onChange(I -> {
            scheduleBruteForce();
            return true;
        });

        model.loadedPresetInVG2.onChange(II -> {
            scheduleBruteForce();
            return true;
        });

        model.loadedPreset.onChange(v -> {
            presenter.loadedPresetUUID = v;
            presenter.currentPartName = model.getPresetNameOfVoiceGroup(model.voiceGroup.getValue());
            notifyChanges();
            return true;
        });

        model.soundType.onChange(v -> {
            presenter.soundType = v;
            notifyChanges();
            return true;
        });
    }

    private void monitorAllParameters() {
        int localSubscribtion = ++allParameterSubscription;

        for (VoiceGroup g : VoiceGroup.values()) {
            for (BasicParameterModel param : EditBufferModel.get().byVoiceGroup[g.ordinal()].parameters.values()) {
                ParameterPresenterProviders.get().register(param.id, v -> {
                    if (localSubscribtion == allParameterSubscription) {
                        scheduleBruteForce();
                        return true;
                    }
                    return false;
                });
            }
        }
    }

    private void scheduleBruteForce() {
        if (!bruteForceScheduled) {
            bruteForceScheduled = true;

            Scheduler.get().scheduleDeferred(new ScheduledCommand() {

                @Override
                public void execute() {
                    bruteForceScheduled = false;
                    bruteForce();
                }
            });
        }
    }

    private boolean isPartLabelChanged(VoiceGroup vg) {
        if (model.soundType.getValue() != SoundType.Single) {
            Preset origin = NonMaps.get().getNonLinearWorld().getPresetManager()
                    .findPreset(model.loadedPreset.getValue());
            if (origin != null) {
                String ogName = origin.getPartName(vg);
                return !model.getPresetNameOfVoiceGroup(vg).equals(ogName);
            }
        }
        return false;
    }

    private void bruteForce() {
        boolean anyScaleNotDef = isAnyScaleOffsetParameterNotDefault();
        if (presenter.isAnyScaleOffsetParameterNotDefault != anyScaleNotDef) {
            presenter.isAnyScaleOffsetParameterNotDefault = anyScaleNotDef;
            notifyChanges();
        }

        boolean anyChanged = isAnyParameterChanged();
        if (model.soundType.getValue() != SoundType.Single) {
            anyChanged |= isPartLabelChanged(VoiceGroup.I) || isPartLabelChanged(VoiceGroup.II);
        }

        if (presenter.isAnyParameterChanged != anyChanged) {
            presenter.isAnyParameterChanged = anyChanged;
            notifyChanges();
        }

        boolean anyLocked = isAnyParameterLocked();
        if (anyLocked != presenter.isAnyParameterLocked) {
            presenter.isAnyParameterLocked = anyLocked;
            notifyChanges();
        }

        boolean allLocked = areAllParametersLocked();
        if (allLocked != presenter.allParametersLocked) {
            presenter.allParametersLocked = allLocked;
            notifyChanges();
        }

        if (model.soundType.getValue() == SoundType.Split) {
            BasicParameterModel sI = model.getParameter(new ParameterId(356, VoiceGroup.I));
            BasicParameterModel sII = model.getParameter(new ParameterId(356, VoiceGroup.II));
            
            Double dI = sI.value.value.getValue() * 100;
            Double dII = sII.value.value.getValue() * 100;
            double iI = dI.intValue();
            double iII = dII.intValue();
            boolean splitOverlap = iI >= iII;
            presenter.splitOverlap = splitOverlap;
            notifyChanges();
        } else if (presenter.splitOverlap != false) {
            presenter.splitOverlap = false;
            notifyChanges();
        }

        if (model.soundType.getValue() == SoundType.Split) {
            boolean lfxI = isCrossFX(VoiceGroup.II);
            boolean lfxII = isCrossFX(VoiceGroup.I);

            if (lfxI != presenter.splitFXToI || lfxII != presenter.splitFXToII) {
                presenter.splitFXToI = lfxI;
                presenter.splitFXToII = lfxII;
                notifyChanges();
            }
        } else if (presenter.splitFXToI != false || presenter.splitFXToII != false) {
            presenter.splitFXToI = false;
            presenter.splitFXToII = false;
            notifyChanges();
        }

        if (model.soundType.getValue() == SoundType.Layer) {
            boolean fbI = isLayerFB(VoiceGroup.I);
            boolean fbII = isLayerFB(VoiceGroup.II);
            boolean fxI = isCrossFX(VoiceGroup.II);
            boolean fxII = isCrossFX(VoiceGroup.I);

            if (fbI != presenter.layerFBI || fbII != presenter.layerFBII || fxI != presenter.layerFXToI
                    || fxII != presenter.layerFXToII) {
                presenter.layerFBI = fbI;
                presenter.layerFBII = fbII;
                presenter.layerFXToI = fxI;
                presenter.layerFXToII = fxII;
                notifyChanges();
            }
        } else if (presenter.layerFBI != false || presenter.layerFBII != false || presenter.layerFXToI != false
                || presenter.layerFXToII != false) {
            presenter.layerFBI = false;
            presenter.layerFBII = false;
            presenter.layerFXToI = false;
            presenter.layerFXToII = false;
            notifyChanges();
        }

        bruteForceSoundBeltArrowStates();
        calculateFXUnused();
    }

    private void bruteForceSoundBeltArrowStates() {
        presenter.splitToFXArrow_I = calculateSplitToFXArrow(VoiceGroup.I);
        presenter.splitToFXArrow_II = calculateSplitToFXArrow(VoiceGroup.II);

        presenter.splitToOut = calculateSplitToOutArrow();

        presenter.split_fb_I_into_I = calculateSplitFeedbackFromI_into_I();
        presenter.split_fb_I_into_II = calculateSplitFeedbackFromI_into_I();
        presenter.split_fb_II_into_I = calculateSplitFeedbackFromII_into_I();
        presenter.split_fb_II_into_II = calculateSplitFeedbackFromII_into_II();


        presenter.layerFXToOut = calculateLayerFXToOut();
        presenter.layerIToFXI = calculateLayerIToFXI();
        presenter.layerIToFXII = calculateLayerIToFXII();
        presenter.layerIIToFXI = calculateLayerIIToFXI();
        presenter.layerIIToFXII = calculateLayerIIToFXII();

        presenter.isSingleSoundFeedbackToPolyActive = calculateSingleSoundFeedbackToPolyActive();
        presenter.fbToPolyArrowState = calculateSingleSoundPolyArrowState();
        presenter.polyToFX = calculateSingleSoundPolyToFXState();
        presenter.fxToOut = calculateSingleSoundFXToOutState();
    }

    public SingleSoundFXToOut calculateSingleSoundFXToOutState() {
        var cp = model.getParameter(new ParameterId(428, VoiceGroup.Global)).value.getQuantizedAndClipped(true);
        if(cp == 0) {
            return SingleSoundFXToOut.FXI;
        } else if(cp == 1) {
            return SingleSoundFXToOut.FXII;
        } else {
            return SingleSoundFXToOut.FXI_FXII;
        }
    }

    public SingleSoundPolyToFX calculateSingleSoundPolyToFXState() {
        var cp = model.getParameter(new ParameterId(362, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        if(cp > 0 && cp < 1){
            return SingleSoundPolyToFX.Poly_To_FXI_FXII;
        } else if(cp == 0) {
            return SingleSoundPolyToFX.Poly_To_FXI;
        } else {
            return SingleSoundPolyToFX.Poly_To_FXII;
        }
    }

    public SingleSoundFBToPoly calculateSingleSoundPolyArrowState() {
        var cp = model.getParameter(new ParameterId(354, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        if(cp > 0 && cp < 1)
            return SingleSoundFBToPoly.FXI_FXII_To_Poly;
        else if(cp == 0)
            return SingleSoundFBToPoly.FXI_To_Poly;
        else
            return SingleSoundFBToPoly.FXII_To_Poly;
    }

    public boolean calculateSingleSoundFeedbackToPolyActive() {
        var fb_mix_fx = model.getParameter(new ParameterId(160, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var fb_mix_lvl = model.getParameter(new ParameterId(299, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        return fb_mix_fx != 0 && fb_mix_lvl > 0 && isFeedbackUsed(VoiceGroup.I);
    }

    public boolean calculateLayerIToFXI() {
        var outmix_I_lvl = model.getParameter(new ParameterId(185, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var outmix_II_lvl = model.getParameter(new ParameterId(185, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        var to_fx_I = model.getParameter(new ParameterId(362, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var to_fx_II = model.getParameter(new ParameterId(362, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        
        var outmix_I_infinite = outmix_I_lvl == 0;
        var outmix_II_infinite = outmix_II_lvl == 0;
        var outmix_I_open = outmix_I_lvl > 0;
        var outmix_II_open = outmix_II_lvl > 0;
        var FX_I_TO_I_ONLY = to_fx_I == 0;
        var FX_II_TO_II_ONLY = to_fx_II == 0;
        var FX_I_TO_II_ONLY = to_fx_I == 1;
        var FX_II_TO_I_ONLY = to_fx_II == 1;
        var FX_I_TO_BOTH = to_fx_I > 0 && to_fx_I < 1;
        var FX_II_TO_BOTH = to_fx_II > 0 && to_fx_II < 1;

        if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_BOTH)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_II_ONLY)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_I_ONLY)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_II_ONLY && FX_II_TO_BOTH)
            return false;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_II_ONLY && FX_II_TO_I_ONLY)
            return false;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_BOTH)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_II_ONLY)
            return true;
        else if(outmix_I_open && outmix_II_infinite && FX_I_TO_BOTH)
            return true;
        else if(outmix_I_open && outmix_II_infinite && FX_I_TO_II_ONLY)
            return false;
        else if(outmix_I_open && outmix_II_infinite && FX_I_TO_I_ONLY)
            return true;
        else if(outmix_II_open && outmix_I_infinite && FX_II_TO_BOTH)
            return false;
        else if(outmix_II_open && outmix_I_infinite && FX_II_TO_I_ONLY)
            return false;
        else if(outmix_II_open && outmix_I_infinite && FX_II_TO_II_ONLY)
            return false;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_I_ONLY)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_II_TO_II_ONLY && FX_I_TO_II_ONLY)
            return false;
        else if(outmix_I_infinite && outmix_II_infinite)
            return false;
        
        return false;
    }

    public boolean calculateLayerIToFXII() {
        var outmix_I_lvl = model.getParameter(new ParameterId(185, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var outmix_II_lvl = model.getParameter(new ParameterId(185, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        var to_fx_I = model.getParameter(new ParameterId(362, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var to_fx_II = model.getParameter(new ParameterId(362, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        
        var outmix_I_infinite = outmix_I_lvl == 0;
        var outmix_II_infinite = outmix_II_lvl == 0;
        var outmix_I_open = outmix_I_lvl > 0;
        var outmix_II_open = outmix_II_lvl > 0;
        var FX_I_TO_I_ONLY = to_fx_I == 0;
        var FX_II_TO_II_ONLY = to_fx_II == 0;
        var FX_I_TO_II_ONLY = to_fx_I == 1;
        var FX_II_TO_I_ONLY = to_fx_II == 1;
        var FX_I_TO_BOTH = to_fx_I > 0 && to_fx_I < 1;
        var FX_II_TO_BOTH = to_fx_II > 0 && to_fx_II < 1;

        if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_BOTH)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_II_ONLY)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_I_ONLY)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_II_ONLY && FX_II_TO_BOTH)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_II_ONLY && FX_II_TO_I_ONLY)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_BOTH)
            return false;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_II_ONLY)
            return false;
        else if(outmix_I_open && outmix_II_infinite && FX_I_TO_BOTH)
            return true;
        else if(outmix_I_open && outmix_II_infinite && FX_I_TO_II_ONLY)
            return true;
        else if(outmix_I_open && outmix_II_infinite && FX_I_TO_I_ONLY)
            return false;
        else if(outmix_II_open && outmix_I_infinite && FX_II_TO_BOTH)
            return false;
        else if(outmix_II_open && outmix_I_infinite && FX_II_TO_I_ONLY)
            return false;
        else if(outmix_II_open && outmix_I_infinite && FX_II_TO_II_ONLY)
            return false;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_I_ONLY)
            return false;
        else if(outmix_I_open && outmix_II_open && FX_II_TO_II_ONLY && FX_I_TO_II_ONLY)
            return true;
        else if(outmix_I_infinite && outmix_II_infinite)
            return false;
            
        return false;    
    }

    public boolean calculateLayerIIToFXI() {
        var outmix_I_lvl = model.getParameter(new ParameterId(185, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var outmix_II_lvl = model.getParameter(new ParameterId(185, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        var to_fx_I = model.getParameter(new ParameterId(362, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var to_fx_II = model.getParameter(new ParameterId(362, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        
        var outmix_I_infinite = outmix_I_lvl == 0;
        var outmix_II_infinite = outmix_II_lvl == 0;
        var outmix_I_open = outmix_I_lvl > 0;
        var outmix_II_open = outmix_II_lvl > 0;
        var FX_I_TO_I_ONLY = to_fx_I == 0;
        var FX_II_TO_II_ONLY = to_fx_II == 0;
        var FX_I_TO_II_ONLY = to_fx_I == 1;
        var FX_II_TO_I_ONLY = to_fx_II == 1;
        var FX_I_TO_BOTH = to_fx_I > 0 && to_fx_I < 1;
        var FX_II_TO_BOTH = to_fx_II > 0 && to_fx_II < 1;

        if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_BOTH)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_II_ONLY)
            return false;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_I_ONLY)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_II_ONLY && FX_II_TO_BOTH)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_II_ONLY && FX_II_TO_I_ONLY)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_BOTH)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_II_ONLY)
            return false;
        else if(outmix_I_open && outmix_II_infinite && FX_I_TO_BOTH)
            return false;
        else if(outmix_I_open && outmix_II_infinite && FX_I_TO_II_ONLY)
            return false;
        else if(outmix_I_open && outmix_II_infinite && FX_I_TO_I_ONLY)
            return false;
        else if(outmix_II_open && outmix_I_infinite && FX_II_TO_BOTH)
            return true;
        else if(outmix_II_open && outmix_I_infinite && FX_II_TO_I_ONLY)
            return true;
        else if(outmix_II_open && outmix_I_infinite && FX_II_TO_II_ONLY)
            return false;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_I_ONLY)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_II_TO_II_ONLY && FX_I_TO_II_ONLY)
            return false;
        else if(outmix_I_infinite && outmix_II_infinite)
            return false;
                
        return false;    
    }

    public boolean calculateLayerIIToFXII() {
        var outmix_I_lvl = model.getParameter(new ParameterId(185, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var outmix_II_lvl = model.getParameter(new ParameterId(185, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        var to_fx_I = model.getParameter(new ParameterId(362, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var to_fx_II = model.getParameter(new ParameterId(362, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        
        var outmix_I_infinite = outmix_I_lvl == 0;
        var outmix_II_infinite = outmix_II_lvl == 0;
        var outmix_I_open = outmix_I_lvl > 0;
        var outmix_II_open = outmix_II_lvl > 0;
        var FX_I_TO_I_ONLY = to_fx_I == 0;
        var FX_II_TO_II_ONLY = to_fx_II == 0;
        var FX_I_TO_II_ONLY = to_fx_I == 1;
        var FX_II_TO_I_ONLY = to_fx_II == 1;
        var FX_I_TO_BOTH = to_fx_I > 0 && to_fx_I < 1;
        var FX_II_TO_BOTH = to_fx_II > 0 && to_fx_II < 1;

        if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_BOTH)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_II_ONLY)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_I_ONLY)
            return false;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_II_ONLY && FX_II_TO_BOTH)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_II_ONLY && FX_II_TO_I_ONLY)
            return false;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_BOTH)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_II_ONLY)
            return true;
        else if(outmix_I_open && outmix_II_infinite && FX_I_TO_BOTH)
            return false;
        else if(outmix_I_open && outmix_II_infinite && FX_I_TO_II_ONLY)
            return false;
        else if(outmix_I_open && outmix_II_infinite && FX_I_TO_I_ONLY)
            return false;
        else if(outmix_II_open && outmix_I_infinite && FX_II_TO_BOTH)
            return true;
        else if(outmix_II_open && outmix_I_infinite && FX_II_TO_I_ONLY)
            return false;
        else if(outmix_II_open && outmix_I_infinite && FX_II_TO_II_ONLY)
            return true;
        else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_I_ONLY)
            return false;
        else if(outmix_I_open && outmix_II_open && FX_II_TO_II_ONLY && FX_I_TO_II_ONLY)
            return true;
        else if(outmix_I_infinite && outmix_II_infinite)
            return false;
        
        return false;    
    }

    public GenericArrowEnum calculateLayerFXToOut() {
        var part_vol_I = model.getParameter(new ParameterId(358, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var part_vol_II = model.getParameter(new ParameterId(358, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        var part_mute_I = model.getParameter(new ParameterId(395, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var part_mute_II = model.getParameter(new ParameterId(395, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        
        if(part_vol_I > 0 && (part_vol_II == 0 || part_mute_II == 1))
            return GenericArrowEnum.PartI;
        else if(part_vol_II > 0 && (part_vol_I == 0 || part_mute_I == 1))
            return GenericArrowEnum.PartII;
        else if(part_vol_I > 0 && part_vol_II > 0)
            return GenericArrowEnum.PartI_PartII;
        
        return GenericArrowEnum.None;
    }

    public boolean calculateSplitFeedbackFromI_into_I() {
        var fb_mix_fx_src = model.getParameter(new ParameterId(354, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var fb_mix_fx = model.getParameter(new ParameterId(160, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var fb_mix_lvl = model.getParameter(new ParameterId(299, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        return fb_mix_fx_src < 1 && fb_mix_fx != 0 && fb_mix_lvl > 0 && isFeedbackUsed(VoiceGroup.I);
    }

    public boolean calculateSplitFeedbackFromI_into_II() {
        var fb_mix_fx_src = model.getParameter(new ParameterId(354, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var fb_mix_fx = model.getParameter(new ParameterId(160, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var fb_mix_lvl = model.getParameter(new ParameterId(299, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        return fb_mix_fx_src > 0 && fb_mix_fx != 0 && fb_mix_lvl > 0 && isFeedbackUsed(VoiceGroup.I);
    }


    public boolean calculateSplitFeedbackFromII_into_I() {
        var fb_mix_fx_src = model.getParameter(new ParameterId(354, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        var fb_mix_fx = model.getParameter(new ParameterId(160, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        var fb_mix_lvl = model.getParameter(new ParameterId(299, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        return fb_mix_fx_src > 0 && fb_mix_fx != 0 && fb_mix_lvl > 0 && isFeedbackUsed(VoiceGroup.II);
    }

    public boolean calculateSplitFeedbackFromII_into_II() {
        var fb_mix_fx_src = model.getParameter(new ParameterId(354, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        var fb_mix_fx = model.getParameter(new ParameterId(160, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        var fb_mix_lvl = model.getParameter(new ParameterId(299, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        return fb_mix_fx_src < 1 && fb_mix_fx != 0 && fb_mix_lvl > 0 && isFeedbackUsed(VoiceGroup.II);
    }

    public boolean isFeedbackUsed(VoiceGroup vg) {
        var osc_a_pm_fb = model.getParameter(new ParameterId(68, vg)).value.getQuantizedAndClipped(true);
        var osc_b_pm_fb = model.getParameter(new ParameterId(98, vg)).value.getQuantizedAndClipped(true);
        var shp_a_fb_mix = model.getParameter(new ParameterId(78, vg)).value.getQuantizedAndClipped(true);
        var shp_b_fb_mix = model.getParameter(new ParameterId(108, vg)).value.getQuantizedAndClipped(true);
        boolean used = (osc_a_pm_fb != 0 || osc_b_pm_fb != 0 || shp_a_fb_mix > 0 || shp_b_fb_mix > 0);
        return used;
    }

    private GenericArrowEnum calculateSplitToFXArrow(VoiceGroup vg) {
        var out_mix_to_fx = model.getParameter(new ParameterId(362, vg)).value.getClippedValue();
        var out_mix_lvl = model.getParameter(new ParameterId(185, vg)).value.getClippedValue();
        if(out_mix_to_fx == 0 && out_mix_lvl > 0)
            return GenericArrowEnum.PartI;
        else if(out_mix_to_fx == 1 && out_mix_lvl > 0)
            return GenericArrowEnum.PartII;
        else if(out_mix_to_fx > 0 && out_mix_to_fx < 1 && out_mix_lvl > 0)
            return GenericArrowEnum.PartI_PartII;
        return GenericArrowEnum.None;
    }

    private GenericArrowEnum calculateSplitToOutArrow() {
        var part_vol_I = model.getParameter(new ParameterId(358, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var part_vol_II = model.getParameter(new ParameterId(358, VoiceGroup.II)).value.getQuantizedAndClipped(true);
        
        if(part_vol_I > 0 && part_vol_II == 0)
            return GenericArrowEnum.PartI;
        else if(part_vol_I == 0 && part_vol_II > 0)
            return GenericArrowEnum.PartII;
        else if(part_vol_I > 0 && part_vol_II > 0)
            return GenericArrowEnum.PartI_PartII;
        return GenericArrowEnum.None;
    }

    private void calculateFXUnused() {
        var out_mix_lvl = EditBufferModel.get().getParameter(new ParameterId(185, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var out_mix_to_fx = EditBufferModel.get().getParameter(new ParameterId(362, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var master_serial_fx = EditBufferModel.get().getParameter(new ParameterId(408, VoiceGroup.Global)).value.getQuantizedAndClipped(true);
        var master_fx_mix = EditBufferModel.get().getParameter(new ParameterId(428, VoiceGroup.Global)).value.getQuantizedAndClipped(true);
        var fb_mix_lvl = EditBufferModel.get().getParameter(new ParameterId(299, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var fb_mix_fx = EditBufferModel.get().getParameter(new ParameterId(160, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var fb_mix_fx_src = EditBufferModel.get().getParameter(new ParameterId(354, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var osc_a_pm_fb = EditBufferModel.get().getParameter(new ParameterId(68, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var shp_a_fb_mix = EditBufferModel.get().getParameter(new ParameterId(78, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var osc_b_pm_fb = EditBufferModel.get().getParameter(new ParameterId(98, VoiceGroup.I)).value.getQuantizedAndClipped(true);
        var shp_b_fb_mix = EditBufferModel.get().getParameter(new ParameterId(108, VoiceGroup.I)).value.getQuantizedAndClipped(true);

        {
            var out_mix = out_mix_lvl > 0 && out_mix_to_fx < 1;
            var fx_input = out_mix || master_serial_fx < 0;
            var fx_output = master_fx_mix < 1 || master_serial_fx > 0;
            var fx_feedback = fb_mix_lvl > 0 && fb_mix_fx != 0 && fb_mix_fx_src < 1 && (osc_a_pm_fb != 0 || shp_a_fb_mix != 0 || osc_b_pm_fb != 0 || shp_b_fb_mix != 0);
            var fx_used = fx_input && (fx_output || fx_feedback);
            presenter.fxIUnused = !fx_used;
        }

        {
            var out_mix = out_mix_lvl > 0 && out_mix_to_fx > 0;
			var fx_input = out_mix || master_serial_fx > 0;
			var fx_output = master_fx_mix > 0 || master_serial_fx < 0;
			var fx_feedback = fb_mix_lvl > 0 && fb_mix_fx != 0 && fb_mix_fx_src > 0 && (osc_a_pm_fb != 0 || shp_a_fb_mix != 0 || osc_b_pm_fb != 0 || shp_b_fb_mix != 0);
			var fx_used = fx_input && (fx_output || fx_feedback);
			presenter.fxIIUnused = !fx_used;
        }
    }

    private boolean cpNotZero(int num, VoiceGroup vg) {
        BasicParameterModel param = model.getParameter(new ParameterId(num, vg));
        return param.value.value.getValue() != 0;
    }

    private boolean cpGreaterThanZero(int num, VoiceGroup vg) {
        BasicParameterModel param = model.getParameter(new ParameterId(num, vg));
        return param.value.value.getValue() > 0;
    }

    private boolean isLayerFB(VoiceGroup vg) {
        boolean oscFB = cpNotZero(346, vg);

        boolean combMix = cpNotZero(156, vg);
        boolean combSrc = cpNotZero(350, vg);
        boolean comb = combMix && combSrc;

        boolean svfMix = cpNotZero(158, vg);
        boolean svfSrc = cpNotZero(352, vg);
        boolean svf = svfMix && svfSrc;

        boolean fxMix = cpNotZero(160, vg);
        boolean fxSrc = cpNotZero(354, vg);
        boolean fx = fxMix && fxSrc;

        boolean pfbl = cpNotZero(299, vg);
        boolean shaperFB = cpNotZero(68, vg) || cpNotZero(98, vg);
        boolean oscPM = cpGreaterThanZero(78, vg) || cpGreaterThanZero(108, vg);

        boolean other = pfbl && (shaperFB || oscPM);

        return (oscFB || comb || svf || fx) && other;
    }

    private boolean isCrossFX(VoiceGroup vg) {
        return cpGreaterThanZero(362, vg);
    }

    private boolean isAnyParameterLocked() {
        for (VoiceGroup g : VoiceGroup.values()) {
            for (BasicParameterModel param : EditBufferModel.get().byVoiceGroup[g.ordinal()].parameters.values()) {
                if (param.isLocked())
                    return true;
            }
        }

        return false;
    }

    public boolean areAllParametersLocked() {
        for (VoiceGroup g : VoiceGroup.values()) {
            for (BasicParameterModel param : EditBufferModel.get().byVoiceGroup[g.ordinal()].parameters.values()) {
                if (!param.isLocked())
                    return false;
            }
        }

        return true;
    }

    private boolean isAnyParameterChanged() {
        for (VoiceGroup g : VoiceGroup.values()) {
            for (BasicParameterModel param : EditBufferModel.get().byVoiceGroup[g.ordinal()].parameters.values()) {
                if (param instanceof MacroControlParameterModel) {
                    MacroControlParameterModel mc = (MacroControlParameterModel) param;
                    if (ParameterPresenterProvider.isMCMetaChanged(mc))
                        return true;
                }

                if (param instanceof SendParameterModel || param instanceof PhysicalControlParameterModel)
                    continue;

                if (ParameterPresenterProvider.isValueChanged(param))
                    return true;
            }
        }

        return false;
    }

    private void monitorSelectedParameter() {
        EditBufferModel.get().selectedParameter.onChange(selectedParameterID -> {
            int localSubscription = ++selectedParameterSubscription;

            ParameterPresenterProviders.get().registerForCurrentVoiceGroup(selectedParameterID, v -> {
                if (localSubscription != selectedParameterSubscription)
                    return false;

                presenter.selectedParameter = v;
                notifyChanges();
                return true;
            });
            return true;
        });
    }

    private boolean isAnyScaleOffsetParameterNotDefault() {
        for (int scaleParamID : ParameterFactory.getScaleOffsetParameters()) {
            ScaleOffsetParameterModel p = (ScaleOffsetParameterModel) EditBufferModel.get()
                    .getParameter(new ParameterId(scaleParamID, VoiceGroup.Global));

            double currentValue = p.value.getQuantizedAndClipped(true);
            double defaultValue = p.value.metaData.defaultValue.getValue();

            if (currentValue != defaultValue)
                return true;
        }
        return false;
    }

    @Override
    public EditBufferPresenter getValue() {
        return presenter;
    }

    public static EditBufferPresenterProvider get() {
        return theProvider;
    }

    public static EditBufferPresenter getPresenter() {
        return get().presenter;
    }

}