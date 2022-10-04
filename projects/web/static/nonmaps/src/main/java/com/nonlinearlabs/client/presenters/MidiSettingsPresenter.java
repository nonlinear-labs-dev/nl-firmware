package com.nonlinearlabs.client.presenters;

public class MidiSettingsPresenter {
    public String pedal1CCString = "";
    public String pedal2CCString = "";
    public String pedal3CCString = "";
    public String pedal4CCString = "";
    public String ribbon1CCString = "";
    public String ribbon2CCString = "";
    public String ribbon3CCString = "";
    public String ribbon4CCString = "";
    public String benderCCString = "";
    public String aftertouchCCString = "";
    public String selectedParameterCCString = "";

    public String getCCStringFor(int parameter)
    {
        switch (parameter) {
            case 398:
            case 254:
                return pedal1CCString;
            case 399:
            case 259:
                return pedal2CCString;
            case 400:
            case 264:
                return pedal3CCString;
            case 401:
            case 269:
                return pedal4CCString;
            case 402:
            case 274:
                return benderCCString;
            case 403:
            case 279:
                return aftertouchCCString;
            case 404:
            case 284:
                return ribbon1CCString;
            case 405:
            case 289:
                return ribbon2CCString;
            case 409:
            case 410:
                return ribbon3CCString;
            case 411:
            case 412:
                return ribbon4CCString;
            default:
                return "";
        }
    }
}