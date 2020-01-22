package com.nonlinearlabs.client.world.overlay.html.soundSettings;

public class SoundSettings_SoundSettingsUiBinderImpl_TemplateImpl implements com.nonlinearlabs.client.world.overlay.html.soundSettings.SoundSettings_SoundSettingsUiBinderImpl.Template {
  
  public com.google.gwt.safehtml.shared.SafeHtml html1(java.lang.String arg0,java.lang.String arg1,java.lang.String arg2,java.lang.String arg3,java.lang.String arg4,java.lang.String arg5,java.lang.String arg6,java.lang.String arg7) {
    StringBuilder sb = new java.lang.StringBuilder();
    sb.append("<div class='forms-container dialog-container sound-settings'> <div class='mt1'></div> <div class='eins-he fl-j-center'> <span id='");
    sb.append(com.google.gwt.safehtml.shared.SafeHtmlUtils.htmlEscape(arg0));
    sb.append("'></span> </div> <div class='mt1'></div> <div class='linie'></div> <div class='eins-he'> <div class='w40p fl-j-center section-headline'> <p>Randomize Sound</p> </div> </div> <div class='slider-container'> <div class='w20p fl-j-center'> <span id='");
    sb.append(com.google.gwt.safehtml.shared.SafeHtmlUtils.htmlEscape(arg1));
    sb.append("'></span> </div> <div class='w60p fl-j-center'> <input class='slider-a' id='");
    sb.append(com.google.gwt.safehtml.shared.SafeHtmlUtils.htmlEscape(arg2));
    sb.append("' max='1' min='0' step='0.001' type='range' value='0'> </div> <div class='w20p fl-j-center'> <span id='");
    sb.append(com.google.gwt.safehtml.shared.SafeHtmlUtils.htmlEscape(arg3));
    sb.append("'></span> </div> </div> <div class='eins-he'> <div class='w35p'> <p>Transition Time</p> </div> <div class='w20p'> <span id='");
    sb.append(com.google.gwt.safehtml.shared.SafeHtmlUtils.htmlEscape(arg4));
    sb.append("'></span> </div> <div class='w40p'> <input class='slider-a' id='");
    sb.append(com.google.gwt.safehtml.shared.SafeHtmlUtils.htmlEscape(arg5));
    sb.append("' max='1' min='0' step='0.001' type='range' value='0'> </div> </div> <div class='eins-he'> <div class='w35p'> <p>Tune Reference</p> </div> <div class='w20p'> <span id='");
    sb.append(com.google.gwt.safehtml.shared.SafeHtmlUtils.htmlEscape(arg6));
    sb.append("'></span> </div> <div class='w40p'> <input class='slider-a' id='");
    sb.append(com.google.gwt.safehtml.shared.SafeHtmlUtils.htmlEscape(arg7));
    sb.append("' max='1' min='0' step='0.0001' type='range' value='0'> </div> </div> </div>");
return new com.google.gwt.safehtml.shared.OnlyToBeUsedInGeneratedCodeStringBlessedAsSafeHtml(sb.toString());
}
}
