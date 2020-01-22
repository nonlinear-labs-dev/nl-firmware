package com.google.gwt.user.client.ui;

public class NamedFrame_IFrameTemplateImpl implements com.google.gwt.user.client.ui.NamedFrame.IFrameTemplate {
  
  public com.google.gwt.safehtml.shared.SafeHtml get(java.lang.String arg0) {
    StringBuilder sb = new java.lang.StringBuilder();
    sb.append("<iframe src=\"about:blank\" name='");
    sb.append(com.google.gwt.safehtml.shared.SafeHtmlUtils.htmlEscape(arg0));
    sb.append("'>");
return new com.google.gwt.safehtml.shared.OnlyToBeUsedInGeneratedCodeStringBlessedAsSafeHtml(sb.toString());
}
}
