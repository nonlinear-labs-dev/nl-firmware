package com.nonlinearlabs.client.presenters;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.RGBA;

public class EditBufferPresenter {


	public enum SerialFX {
        FX_I_IN_II,
        FX_II_IN_I,
        None
    }

    public enum SingleSoundFBToPoly {
        FXI_To_Poly,
        FXII_To_Poly,
        FXI_FXII_To_Poly
    }

    public enum SingleSoundPolyToFX {
        Poly_To_FXI,
        Poly_To_FXII,
        Poly_To_FXI_FXII
    }

    public enum SingleSoundFXToOut {
        FXI,
        FXII,
        FXI_FXII
    }

    public enum LayerToFX {
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, NONE
    }

    public enum LayerFXToOut {
        PartI,
        PartII,
        PartI_II,
        None
    }

    public enum SplitToFX_From_VG_I {
        PartI,
        PartII,
        PartI_PartII,
        None
    }

    public enum SplitToFX_From_VG_II {
        PartI,
        PartII,
        PartI_PartII,
        None
    }

    public enum Split_I_To_Out {
        PartI,
        PartII,
        PartI_PartII,
        None
    }

    public enum Split_II_To_Out {
        PartI,
        PartII,
        PartI_PartII,
        None
    }

    public class FBToPolyParameterPrerequisites {
        boolean fb_mix_fx = false;
        boolean fb_mix_lvl = false;
        boolean osc_a_pm_fb = false;
        boolean osc_b_pm_fb = false;
        boolean shp_a_fb_mix = false;
        boolean shp_b_fb_mix = false;

        public boolean calculateState() {
            return fb_mix_fx && fb_mix_lvl && (
                osc_a_pm_fb || osc_b_pm_fb || shp_a_fb_mix || shp_b_fb_mix
            );
        }
    }

    public class LayerToFXParameterStates {
        double outmix_I_lvl;
        double outmix_II_lvl;
        double to_fx_I;
        double to_fx_II;

        public LayerToFX calculate() {
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
                return LayerToFX.M;
            else if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_II_ONLY)
                return LayerToFX.I;
            else if(outmix_I_open && outmix_II_open && FX_I_TO_BOTH && FX_II_TO_I_ONLY)
                return LayerToFX.K;
            else if(outmix_I_open && outmix_II_open && FX_I_TO_II_ONLY && FX_II_TO_BOTH)
                return LayerToFX.L;
            else if(outmix_I_open && outmix_II_open && FX_I_TO_II_ONLY && FX_II_TO_I_ONLY)
                return LayerToFX.H;
            else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_BOTH)
                return LayerToFX.J;
            else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_II_ONLY)
                return LayerToFX.G;
            else if(outmix_I_open && outmix_II_infinite && FX_I_TO_BOTH)
                return LayerToFX.E;
            else if(outmix_I_open && outmix_II_infinite && FX_I_TO_II_ONLY)
                return LayerToFX.C;
            else if(outmix_I_open && outmix_II_infinite && FX_I_TO_I_ONLY)
                return LayerToFX.A;
            else if(outmix_II_open && outmix_I_infinite && FX_II_TO_BOTH)
                return LayerToFX.F;
            else if(outmix_II_open && outmix_I_infinite && FX_II_TO_I_ONLY)
                return LayerToFX.D;
            else if(outmix_II_open && outmix_I_infinite && FX_II_TO_II_ONLY)
                return LayerToFX.B;
            else if(outmix_I_open && outmix_II_open && FX_I_TO_I_ONLY && FX_II_TO_I_ONLY)
                return LayerToFX.N;
            else if(outmix_I_open && outmix_II_open && FX_II_TO_II_ONLY && FX_I_TO_II_ONLY)
                return LayerToFX.O;
            else if(outmix_I_infinite && outmix_II_infinite)
                return LayerToFX.NONE;
            
            return LayerToFX.NONE;
        }
    }

    public class LayerFXToOutParameterStates {
        double part_vol_I;
        double part_vol_II;
        double part_mute_I;
        double part_mute_II;

        public LayerFXToOut calculate() {
            if(part_vol_I > 0 && (part_vol_II == 0 || part_mute_II == 1))
            return LayerFXToOut.PartI;
            else if(part_vol_II > 0 && (part_vol_I == 0 || part_mute_I == 1))
            return LayerFXToOut.PartII;
            else if(part_vol_I > 0 && part_vol_II > 0)
            return LayerFXToOut.PartI_II;
            else if(part_vol_I == 0 && part_vol_II == 0)
                return LayerFXToOut.None;
            return LayerFXToOut.None;
        }
    } 

    public class SplitToFXFrom_VG_I_ParameterStates {
        double out_mix_to_fx;
        double out_mix_lvl;

        public SplitToFX_From_VG_I calculate() {
            if(out_mix_to_fx == 0 && out_mix_lvl > 0)
                return SplitToFX_From_VG_I.PartI;
            else if(out_mix_to_fx == 1 && out_mix_lvl > 0)
                return SplitToFX_From_VG_I.PartII;
            else if(out_mix_to_fx > 0 && out_mix_to_fx < 1 && out_mix_lvl > 0)
                return SplitToFX_From_VG_I.PartI_PartII;

            return SplitToFX_From_VG_I.None;
        }
    }

    public class SplitToFXFrom_VG_II_ParameterStates {
        double out_mix_to_fx;
        double out_mix_lvl;

        public SplitToFX_From_VG_II calculate() {
            if(out_mix_to_fx == 0 && out_mix_lvl > 0)
                return SplitToFX_From_VG_II.PartI;
            else if(out_mix_to_fx == 1 && out_mix_lvl > 0)
                return SplitToFX_From_VG_II.PartII;
            else if(out_mix_to_fx > 0 && out_mix_to_fx < 1 && out_mix_lvl > 0)
                return SplitToFX_From_VG_II.PartI_PartII;

            return SplitToFX_From_VG_II.None;
        }
    }

    public class SplitIToOutParameterStates {
        double part_vol_I, part_vol_II;
        
        public Split_I_To_Out calculate() {
            if(part_vol_I > 0 && part_vol_II == 0)
                return Split_I_To_Out.PartI;
            else if(part_vol_I == 0 && part_vol_II > 0)
                return Split_I_To_Out.PartII;
            else if(part_vol_I > 0 && part_vol_II > 0)
                return Split_I_To_Out.PartI_PartII;
            return Split_I_To_Out.None;
        }
    }

    public class SplitIIToOutParameterStates {
        double part_vol_I, part_vol_II;

        public Split_II_To_Out calculate() {
            if(part_vol_I > 0 && part_vol_II == 0)
                return Split_II_To_Out.PartI;
            else if(part_vol_I == 0 && part_vol_II > 0)
                return Split_II_To_Out.PartII;
            else if(part_vol_I > 0 && part_vol_II > 0)
                return Split_II_To_Out.PartI_PartII;
            return Split_II_To_Out.None;
        }
    }

    public abstract class SplitFB_Into_X_ParameterStates {
        double fb_mix_fx_src, fb_mix_fx, fb_mix_lvl, osc_a_pm_fb, osc_b_pm_fb, shp_a_fb_mix, shp_b_fb_mix;
        
        public boolean isFeedbackUsed() {
            boolean used = (osc_a_pm_fb != 0 || osc_b_pm_fb != 0 || shp_a_fb_mix > 0 || shp_b_fb_mix > 0);
            return used;
        }

        public boolean calculate() { return false; }
    }

    public class SplitFB_I_Into_I_ParameterStates extends SplitFB_Into_X_ParameterStates {
        @Override
        public boolean calculate() {
            return fb_mix_fx_src < 1 && fb_mix_fx != 0 && fb_mix_lvl > 0 && isFeedbackUsed();
        }
    }

    public class SplitFB_I_Into_II_ParameterStates extends SplitFB_Into_X_ParameterStates {
        @Override
        public boolean calculate() {
            return fb_mix_fx_src > 0 && fb_mix_fx != 0 && fb_mix_lvl > 0 && isFeedbackUsed();
        }
    }

    public class SplitFB_II_Into_I_ParameterStates extends SplitFB_Into_X_ParameterStates {
        @Override
        public boolean calculate() {
            return fb_mix_fx_src > 0 && fb_mix_fx != 0 && fb_mix_lvl > 0 && isFeedbackUsed();
        }
    }

    public class SplitFB_II_Into_II_ParameterStates extends SplitFB_Into_X_ParameterStates {
        @Override
        public boolean calculate() {
            return fb_mix_fx_src < 1 && fb_mix_fx != 0 && fb_mix_lvl > 0 && isFeedbackUsed();
        }
    }

    public boolean allParametersLocked = false;
    public boolean isAnyParameterLocked = false;
    public boolean isAnyParameterChanged = false;
    public boolean isAnyScaleOffsetParameterNotDefault = false;

    public boolean layerFBI = false;
    public boolean layerFBII = false;

    public boolean splitFXToI = false;
    public boolean splitFXToII = false;

    public boolean layerFXToI = false;
    public boolean layerFXToII = false;

    public String loadedPresetUUID = "";
    public ParameterPresenter selectedParameter;
    public String voiceGroup = "";
    public RGB voiceGroupIndicationColor = RGBA.transparent();
    public VoiceGroup voiceGroupEnum = VoiceGroup.I;
    public boolean splitOverlap = false;

    public RGB voiceGroupI_ForegroundColor = RGBA.transparent();
    public RGB voiceGroupII_ForegroundColor = RGBA.transparent();
    public RGB voiceGroupI_BackgroundColor = RGBA.transparent();
    public RGB voiceGroupII_BackgroundColor = RGBA.transparent();

    public String currentPartName;
    public SoundType soundType;

    //arrow states
    //general
    public SerialFX serialFX = SerialFX.None;

    //split
    public SplitToFXFrom_VG_I_ParameterStates splitToFXArrow_I = new SplitToFXFrom_VG_I_ParameterStates();
    public SplitToFXFrom_VG_II_ParameterStates splitToFXArrow_II = new SplitToFXFrom_VG_II_ParameterStates();
    public SplitIToOutParameterStates splitIToOut = new SplitIToOutParameterStates();
    public SplitIIToOutParameterStates splitIIToOut = new SplitIIToOutParameterStates();
    
    public SplitFB_I_Into_I_ParameterStates split_fb_I_into_I = new SplitFB_I_Into_I_ParameterStates();
    public SplitFB_I_Into_II_ParameterStates split_fb_I_into_II = new SplitFB_I_Into_II_ParameterStates();
    public SplitFB_II_Into_I_ParameterStates split_fb_II_into_I = new SplitFB_II_Into_I_ParameterStates();
    public SplitFB_II_Into_II_ParameterStates split_fb_II_into_II = new SplitFB_II_Into_II_ParameterStates();
    //layer
    public LayerFXToOutParameterStates layerFXToOut = new LayerFXToOutParameterStates();
    public LayerToFXParameterStates layerToFX = new LayerToFXParameterStates();
    
    //single
    public FBToPolyParameterPrerequisites isSingleSoundFeedbackToPolyActive = new FBToPolyParameterPrerequisites();
    public SingleSoundFBToPoly fbToPolyArrowState = SingleSoundFBToPoly.FXI_To_Poly;
    public SingleSoundPolyToFX polyToFX = SingleSoundPolyToFX.Poly_To_FXI;
    public SingleSoundFXToOut fxToOut = SingleSoundFXToOut.FXI;

}