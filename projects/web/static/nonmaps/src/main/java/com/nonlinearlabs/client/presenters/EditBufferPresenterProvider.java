package com.nonlinearlabs.client.presenters;

import com.google.gwt.core.client.Scheduler;
import com.google.gwt.core.client.Scheduler.ScheduledCommand;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.Notifier;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.MacroControlParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
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

        calculateFXUnused();
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
            presenter.fxIUnused = !fx_used && model.soundType.getValue() == SoundType.Single;
        }

        {
            var out_mix = out_mix_lvl > 0 && out_mix_to_fx > 0;
			var fx_input = out_mix || master_serial_fx > 0;
			var fx_output = master_fx_mix > 0 || master_serial_fx < 0;
			var fx_feedback = fb_mix_lvl > 0 && fb_mix_fx != 0 && fb_mix_fx_src > 0 && (osc_a_pm_fb != 0 || shp_a_fb_mix != 0 || osc_b_pm_fb != 0 || shp_b_fb_mix != 0);
			var fx_used = fx_input && (fx_output || fx_feedback);
			presenter.fxIIUnused = !fx_used && model.soundType.getValue() == SoundType.Single;
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