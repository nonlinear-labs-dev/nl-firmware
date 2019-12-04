package com.nonlinearlabs.client.integrationTests;

import com.google.gwt.core.client.Scheduler;

public class IntegrationTests {

    public static void doAllTests() {
        new SoundTypeTests();
    }

    protected static void in(int milliSeconds, Runnable r) {
        Scheduler.get().scheduleFixedPeriod(() -> {
            r.run();
            return false;
        }, milliSeconds);
    }

}
