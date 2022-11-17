package com.nonlinearlabs.client.presenters;
import com.nonlinearlabs.client.dataModel.RoutingAspectsSetting;

public class MidiSettings {

	static public class VelocityCurve {
		static public String[] options = { "Very Soft", "Soft", "Normal", "Hard", "Very Hard" };
		public int selected = 2;
	}

	static public class AftertouchCurve {
		static public String[] options = { "Soft", "Normal", "Hard" };
		public int selected = 1;
	}

    static public class SendChannel {
        static public String[] options = {"None", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};
        public int selected = 0;
    }

    static public class SendChannelSplit {
        static public String[] options = {"None", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "Common"};
        public int selected = 0;
    }
    
    static public class ReceiveChannel {
        static public String[] options = {"None", "Omni", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};
        public int selected = 0;
    }


    static public class ReceiveChannelSplit {
        static public String[] options = {"None", "Omni", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "Common"};
        public int selected = 0;
    }


	static public class OnOffOption {
		static public String[] options = { "On", "Off" };
		public boolean value = false;
    }

    static public class PedalMapping {
        static public String[] options = { 
            "None", "CC 01/33", "CC 02/34", "CC 03/35", "CC 04/36", "CC 05/37",
            "CC 06/38", "CC 07/39", "CC 08/40", "CC 09/41", "CC 10/42",
            "CC 11/43", "CC 12/44", "CC 13/45", "CC 14/46", "CC 15/47",
            "CC 16/48", "CC 17/49", "CC 18/50", "CC 19/51", "CC 20/52",
            "CC 21/53", "CC 22/54", "CC 23/55", "CC 24/56", "CC 25/57",
            "CC 26/58", "CC 27/59", "CC 28/60", "CC 29/61", "CC 30/62",
            "CC 31/63", "CC 64 (sw.)",  "CC 65 (sw.)",  "CC 66 (sw.)",  "CC 67 (sw.)",
            "CC 68 (sw.)",  "CC 69 (sw.)", "CC 64 (cont.)",  "CC 65 (cont.)",  "CC 66 (cont.)",  "CC 67 (cont.)",
            "CC 68 (cont.)",  "CC 69 (cont.)"
        };

        static public String[] optionsWithoutLSB = { 
            "None", "CC 01", "CC 02", "CC 03", "CC 04", "CC 05",
            "CC 06", "CC 07", "CC 08", "CC 09", "CC 10",
            "CC 11", "CC 12", "CC 13", "CC 14", "CC 15",
            "CC 16", "CC 17", "CC 18", "CC 19", "CC 20",
            "CC 21", "CC 22", "CC 23", "CC 24", "CC 25",
            "CC 26", "CC 27", "CC 28", "CC 29", "CC 30",
            "CC 31", "CC 64 (sw.)",  "CC 65 (sw.)",  "CC 66 (sw.)",  "CC 67 (sw.)",
            "CC 68 (sw.)",  "CC 69 (sw.)", "CC 64 (cont.)",  "CC 65 (cont.)",  "CC 66 (cont.)",  "CC 67 (cont.)",
            "CC 68 (cont.)",  "CC 69 (cont.)"
        };

        public int selected = 0;
    }

    static public class RibbonMapping {
        static public String[] options = {
            "None", "CC 01/33", "CC 02/34", "CC 03/35", "CC 04/36", "CC 05/37",
            "CC 06/38", "CC 07/39", "CC 08/40", "CC 09/41", "CC 10/42",
            "CC 11/43", "CC 12/44", "CC 13/45", "CC 14/46", "CC 15/47",
            "CC 16/48", "CC 17/49", "CC 18/50", "CC 19/51", "CC 20/52",
            "CC 21/53", "CC 22/54", "CC 23/55", "CC 24/56", "CC 25/57",
            "CC 26/58", "CC 27/59", "CC 28/60", "CC 29/61", "CC 30/62",
            "CC 31/63" 
        };

        static public String[] optionsWithoutLSB = {
            "None", "CC 01", "CC 02", "CC 03", "CC 04", "CC 05",
            "CC 06", "CC 07", "CC 08", "CC 09", "CC 10",
            "CC 11", "CC 12", "CC 13", "CC 14", "CC 15",
            "CC 16", "CC 17", "CC 18", "CC 19", "CC 20",
            "CC 21", "CC 22", "CC 23", "CC 24", "CC 25",
            "CC 26", "CC 27", "CC 28", "CC 29", "CC 30",
            "CC 31" 
        };

        public int selected = 0;
    }

    static public class BenderMapping {
        static public String[] options = {
            "None", "Pitchbend", "CC 01/33", "CC 02/34", "CC 03/35", "CC 04/36",
            "CC 05/37", "CC 06/38", "CC 07/39", "CC 08/40", "CC 09/41",
            "CC 10/42", "CC 11/43", "CC 12/44", "CC 13/45", "CC 14/46",
            "CC 15/47", "CC 16/48", "CC 17/49", "CC 18/50", "CC 19/51",
            "CC 20/52", "CC 21/53", "CC 22/54", "CC 23/55", "CC 24/56",
            "CC 25/57", "CC 26/58", "CC 27/59", "CC 28/60", "CC 29/61",
            "CC 30/62", "CC 31/63"
        };

        static public String[] optionsWithoutLSB = {
            "None", "Pitchbend", "CC 01", "CC 02", "CC 03", "CC 04",
            "CC 05", "CC 06", "CC 07", "CC 08", "CC 09",
            "CC 10", "CC 11", "CC 12", "CC 13", "CC 14",
            "CC 15", "CC 16", "CC 17", "CC 18", "CC 19",
            "CC 20", "CC 21", "CC 22", "CC 23", "CC 24",
            "CC 25", "CC 26", "CC 27", "CC 28", "CC 29",
            "CC 30", "CC 31"
        };

        public int selected = 0;
    }

    static public class AftertouchMapping {
        static public String[] options = {
            "None", "Ch. Pressure",   "CC 01/33", "CC 02/34", "CC 03/35", "CC 04/36",
            "CC 05/37", "CC 06/38", "CC 07/39", "CC 08/40", "CC 09/41",
            "CC 10/42", "CC 11/43", "CC 12/44", "CC 13/45", "CC 14/46",
            "CC 15/47", "CC 16/48", "CC 17/49", "CC 18/50", "CC 19/51",
            "CC 20/52", "CC 21/53", "CC 22/54", "CC 23/55", "CC 24/56",
            "CC 25/57", "CC 26/58", "CC 27/59", "CC 28/60", "CC 29/61",
            "CC 30/62", "CC 31/63", "Pitchbend up", "Pitchbend down"
        };

        static public String[] optionsWithoutLSB = {
            "None", "Ch. Pressure",   "CC 01", "CC 02", "CC 03", "CC 04",
            "CC 05", "CC 06", "CC 07", "CC 08", "CC 09",
            "CC 10", "CC 11", "CC 12", "CC 13", "CC 14",
            "CC 15", "CC 16", "CC 17", "CC 18", "CC 19",
            "CC 20", "CC 21", "CC 22", "CC 23", "CC 24",
            "CC 25", "CC 26", "CC 27", "CC 28", "CC 29",
            "CC 30", "CC 31", "Pitchbend up", "Pitchbend down"
        };

        public int selected = 0;
    }

    static public class RoutingEntry {
        public static final int numAspects = 5;
        public RoutingEntry() {
            for(int i = 0; i < numAspects; i++) {
                states[i] = new OnOffOption();
            }
        }

        public OnOffOption states[] = new OnOffOption[numAspects];
    }

    static public class RoutingSetting {
        public static final int numRoutings = 12;
        RoutingSetting() {
            for(int i = 0; i < numRoutings; i++) {
                routings[i] = new RoutingEntry();
            }
        }
        public RoutingEntry routings[] = new RoutingEntry[numRoutings]; 
    }


    //Global
    public OnOffOption localEnabled = new OnOffOption();
    
    //Channels
    public SendChannel sendChannel = new SendChannel();
    public SendChannelSplit sendChannelSplit = new SendChannelSplit();
    public ReceiveChannel receiveChannel = new ReceiveChannel();
    public ReceiveChannelSplit receiveChannelSplit = new ReceiveChannelSplit();

    //Unused
    public VelocityCurve receiveVelocityCurve = new VelocityCurve();
    public AftertouchCurve receiveAftertouchCurve = new AftertouchCurve();

    //Mapping
    public PedalMapping pedalMapping1 = new PedalMapping();
    public PedalMapping pedalMapping2 = new PedalMapping();
    public PedalMapping pedalMapping3 = new PedalMapping();
    public PedalMapping pedalMapping4 = new PedalMapping();
    public RibbonMapping ribbonMapping1 = new RibbonMapping();
    public RibbonMapping ribbonMapping2 = new RibbonMapping();
    public RibbonMapping ribbonMapping3 = new RibbonMapping();
    public RibbonMapping ribbonMapping4 = new RibbonMapping();
    public BenderMapping benderMapping = new BenderMapping();
    public AftertouchMapping aftertouchMapping = new AftertouchMapping();
    public OnOffOption highVelocityCC = new OnOffOption();
    public OnOffOption enable14BitCC = new OnOffOption();

    //Routing
    public RoutingSetting routingSetting = new RoutingSetting();

    //FLAC
    public OnOffOption autoStartRecorder = new OnOffOption();

    public static RoutingSetting convert(RoutingAspectsSetting s) {
        RoutingSetting ret = new RoutingSetting();
        for(int routing = 0; routing < RoutingSetting.numRoutings; routing++) {
            for(int aspect = 0; aspect < RoutingEntry.numAspects; aspect++) {
                boolean state = s.m_data[routing][aspect];
                ret.routings[routing].states[aspect].value = state;
            }
        }
        return ret;
    }
}
