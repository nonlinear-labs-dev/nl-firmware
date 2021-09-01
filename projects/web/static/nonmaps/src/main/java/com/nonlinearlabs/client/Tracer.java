package com.nonlinearlabs.client;

import java.util.ArrayList;

public class Tracer {

	static private int num = 0;
	static ArrayList<String> history = new ArrayList<String>();

	public native static void registerCrashHandler() /*-{
														var nativeLog = console.log.bind(console) //store native function
														
														console.log = function(text) { //override
														nativeLog(text);
														@com.nonlinearlabs.client.Tracer::addToLog(Ljava/lang/String;)(text);
														}
														}-*/;

	static public void log(String txt) {
		txt = num++ + ": " + txt;
		consoleLog(txt);
	}

	private native static void consoleLog(String message) /*-{
															console.log(message);
															}-*/;

	public static void sendHistoryToServer() {
		for (String s : history)
			NonMaps.theMaps.getServerProxy().log(s);
	}

	public static void addToLog(String text) {
		history.add(text);

		while (history.size() > 100) {
			history.remove(0);
		}
	}

}
