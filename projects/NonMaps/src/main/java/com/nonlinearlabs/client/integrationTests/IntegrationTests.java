package com.nonlinearlabs.client.integrationTests;

import com.google.gwt.core.client.Scheduler;

public class IntegrationTests {

    public static void doAllTests() {
        //new ConvertSoundTypeTests();
        new OracleTests();
    }

    protected static void in(int milliSeconds, Runnable r) {
        Scheduler.get().scheduleFixedPeriod(() -> {
            r.run();
            return false;
        }, milliSeconds);
    }

}
