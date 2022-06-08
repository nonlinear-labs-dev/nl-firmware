package com.nonlinearlabs.client.integrationTests;

import java.util.ArrayList;
import java.util.List;
import java.util.function.BooleanSupplier;

import com.google.gwt.core.client.GWT;
import com.google.gwt.core.client.Scheduler;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.ServerProxy;

public class TestWithSteps {
    class Step {
        BooleanSupplier done;
        Runnable task;
        int timeout;
        boolean stillWaiting;
        String name;

        Step(int timeout, BooleanSupplier done, Runnable task) {
            this.done = done;
            this.task = task;
            this.timeout = timeout;
        }

        boolean isDone() {
            stillWaiting = !done.getAsBoolean();
            return !stillWaiting;
        }

        void run() {
            stillWaiting = true;
            Scheduler.get().scheduleFixedPeriod(() -> {
                assert (!stillWaiting);
                return false;
            }, timeout);
            task.run();
        }
    }

    List<Step> steps = new ArrayList<Step>();
    Runnable after = null; 

    TestWithSteps(Runnable a) {
        after(a);
    }

    TestWithSteps() {
    }

    public void after(Runnable a) {
        this.after = a;
    }

    private void tryNextStep() {
        Step s = steps.get(0);
        if (s.isDone()) {
            nextStep();
        }
    }

    private boolean nextStep() {
        steps.remove(0);

        if (!steps.isEmpty()) {
            steps.get(0).run();
            return true;
        } else if(after != null) {
            after.run();
        }
        return false;
    }

    public void setStepName(String n) {
        steps.get(steps.size() - 1).name = n;
    }

    protected void addStep(int timeout, Runnable task, BooleanSupplier done) {
        steps.add(new Step(timeout, done, task));
    }

    protected void addStep(Runnable task, BooleanSupplier done) {
        steps.add(new Step(5000, done, task));
    }

    public void start() {
        ServerProxy p = NonMaps.get().getServerProxy();
        p.documentFromPlayground.onChange(v -> {
            tryNextStep();
            return !steps.isEmpty();
        });

        if (!steps.isEmpty())
            steps.get(0).run();
    }
}
