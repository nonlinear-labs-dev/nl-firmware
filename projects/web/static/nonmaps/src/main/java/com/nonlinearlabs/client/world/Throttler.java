package com.nonlinearlabs.client.world;

import com.google.gwt.core.client.Scheduler;
import com.google.gwt.core.client.Scheduler.RepeatingCommand;

class Throttler {
    private int maxDelay = 0;
    private Runnable scheduled = null;
    private RepeatingCommand timer = null;

    Throttler(int delay) {
        maxDelay = delay;
    }

    public void schedule(Runnable s) {
        scheduled = s;

        if (timer == null) {
            scheduled.run();
            scheduled = null;

            timer = new RepeatingCommand() {
                @Override
                public boolean execute() {
                    if (scheduled != null) {
                        scheduled.run();
                        scheduled = null;
                        return true;
                    }

                    timer = null;
                    return false;
                }
            };

            Scheduler.get().scheduleFixedPeriod(timer, maxDelay);
        } else {
            scheduled = s;
        }
    }
}