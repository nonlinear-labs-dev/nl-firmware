package com.google.gwt.user.client.ui;

import com.google.gwt.core.client.GWT;
import com.google.gwt.resources.client.ResourcePrototype;

public class DisclosurePanel_DefaultImages_default_InlineClientBundleGenerator implements com.google.gwt.user.client.ui.DisclosurePanel.DefaultImages {
  private static DisclosurePanel_DefaultImages_default_InlineClientBundleGenerator _instance0 = new DisclosurePanel_DefaultImages_default_InlineClientBundleGenerator();
  private void disclosurePanelClosedInitializer() {
    disclosurePanelClosed = new com.google.gwt.resources.client.impl.ImageResourcePrototype(
      "disclosurePanelClosed",
      com.google.gwt.safehtml.shared.UriUtils.fromTrustedString(com.google.gwt.i18n.client.LocaleInfo.getCurrentLocale().isRTL() ?externalImage_rtl : externalImage),
      0, 0, 16, 16, false, false
    );
  }
  private static class disclosurePanelClosedInitializer {
    static {
      _instance0.disclosurePanelClosedInitializer();
    }
    static com.google.gwt.resources.client.ImageResource get() {
      return disclosurePanelClosed;
    }
  }
  public com.google.gwt.resources.client.ImageResource disclosurePanelClosed() {
    return disclosurePanelClosedInitializer.get();
  }
  private void disclosurePanelOpenInitializer() {
    disclosurePanelOpen = new com.google.gwt.resources.client.impl.ImageResourcePrototype(
      "disclosurePanelOpen",
      com.google.gwt.safehtml.shared.UriUtils.fromTrustedString(externalImage0),
      0, 0, 16, 16, false, false
    );
  }
  private static class disclosurePanelOpenInitializer {
    static {
      _instance0.disclosurePanelOpenInitializer();
    }
    static com.google.gwt.resources.client.ImageResource get() {
      return disclosurePanelOpen;
    }
  }
  public com.google.gwt.resources.client.ImageResource disclosurePanelOpen() {
    return disclosurePanelOpenInitializer.get();
  }
  private static java.util.HashMap<java.lang.String, com.google.gwt.resources.client.ResourcePrototype> resourceMap;
  private static final java.lang.String externalImage = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAnklEQVR4XmNgoDZITk4WQhcjCaSlpZ1JTU3tBdL86HJEAaDGS0D8H4hfpaenJ6PLEwRIBsDweSC2QVeHE2AxAIaXArEcunoMgMcAEP4GDJ+6wsJCTnR9cEDAABjeGBoayoyuFwwIGPCcYMDiMOAnKGqzsrJ40NVjACwGbAQmLhV0dTgBkgFXgc71QJcnCIAaDwOdm48zkAgBvFE0KAEAjDyRV4CcerkAAAAASUVORK5CYII=";
  private static final java.lang.String externalImage_rtl = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAo0lEQVR4XmNgoCdITk4WQhcjCqSlpfGnpqb2Aukz6HIEQXp6ejJQ4ysg/g/El9DlcQKgYhsgPg/VCMOEDQAqkgPipWgaCRtQWFjICfRnHVDRNywa8RsQGhrKDJTciEUDOsZuAAxAA+w5Fo3EGQACWVlZPNAo+0mWATAATDQqaZjeIt4AGAB6ywOo8SrZBoAAKJCB3soHGnAYXY4kAIpqdLGBBwDshpFXSB62tAAAAABJRU5ErkJggg==";
  private static final java.lang.String externalImage0 = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAoElEQVR4XmNgGD6gsLCQMy0t7TAQXyICn0lOThZCN4MhNTU1Hyj5nxAGqutF1wsGoaGhzEAFV9E1oOFXQMyPrhcO0tPTPbBogmOgfDK6HgwAVLgRXSMUn0dXixUAA0gFqPgnFgNs0NXiBKCAQtO8FF0NXpCVlcUD1PQcqvkbEMuhqyEIQAEGMgDomjp0OaIB0ICNoESGLk40AKUNdLHBBQAIRpFXGETgdQAAAABJRU5ErkJggg==";
  private static com.google.gwt.resources.client.ImageResource disclosurePanelClosed;
  private static com.google.gwt.resources.client.ImageResource disclosurePanelOpen;
  
  public ResourcePrototype[] getResources() {
    return new ResourcePrototype[] {
      disclosurePanelClosed(), 
      disclosurePanelOpen(), 
    };
  }
  public ResourcePrototype getResource(String name) {
    if (GWT.isScript()) {
      return getResourceNative(name);
    } else {
      if (resourceMap == null) {
        resourceMap = new java.util.HashMap<java.lang.String, com.google.gwt.resources.client.ResourcePrototype>();
        resourceMap.put("disclosurePanelClosed", disclosurePanelClosed());
        resourceMap.put("disclosurePanelOpen", disclosurePanelOpen());
      }
      return resourceMap.get(name);
    }
  }
  private native ResourcePrototype getResourceNative(String name) /*-{
    switch (name) {
      case 'disclosurePanelClosed': return this.@com.google.gwt.user.client.ui.DisclosurePanel.DefaultImages::disclosurePanelClosed()();
      case 'disclosurePanelOpen': return this.@com.google.gwt.user.client.ui.DisclosurePanel.DefaultImages::disclosurePanelOpen()();
    }
    return null;
  }-*/;
}
