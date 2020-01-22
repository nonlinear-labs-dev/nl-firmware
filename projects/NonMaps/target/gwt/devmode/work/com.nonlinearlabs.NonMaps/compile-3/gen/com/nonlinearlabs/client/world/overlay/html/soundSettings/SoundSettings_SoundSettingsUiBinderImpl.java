// .ui.xml template last modified: 1576744061059
package com.nonlinearlabs.client.world.overlay.html.soundSettings;

import com.google.gwt.core.client.GWT;
import com.google.gwt.dom.client.Element;
import com.google.gwt.safehtml.client.SafeHtmlTemplates;
import com.google.gwt.safehtml.shared.SafeHtml;
import com.google.gwt.safehtml.shared.SafeHtmlUtils;
import com.google.gwt.safehtml.shared.SafeHtmlBuilder;
import com.google.gwt.safehtml.shared.SafeUri;
import com.google.gwt.safehtml.shared.UriUtils;
import com.google.gwt.uibinder.client.UiBinderUtil;
import com.google.gwt.uibinder.client.UiBinder;
import com.google.gwt.uibinder.client.UiBinderUtil;
import com.google.gwt.user.client.ui.HTMLPanel;

public class SoundSettings_SoundSettingsUiBinderImpl implements UiBinder<com.google.gwt.user.client.ui.HTMLPanel, com.nonlinearlabs.client.world.overlay.html.soundSettings.SoundSettings>, com.nonlinearlabs.client.world.overlay.html.soundSettings.SoundSettings.SoundSettingsUiBinder {

  interface Template extends SafeHtmlTemplates {
    @Template("<div class='forms-container dialog-container sound-settings'> <div class='mt1'></div> <div class='eins-he fl-j-center'> <span id='{0}'></span> </div> <div class='mt1'></div> <div class='linie'></div> <div class='eins-he'> <div class='w40p fl-j-center section-headline'> <p>Randomize Sound</p> </div> </div> <div class='slider-container'> <div class='w20p fl-j-center'> <span id='{1}'></span> </div> <div class='w60p fl-j-center'> <input class='slider-a' id='{2}' max='1' min='0' step='0.001' type='range' value='0'> </div> <div class='w20p fl-j-center'> <span id='{3}'></span> </div> </div> <div class='eins-he'> <div class='w35p'> <p>Transition Time</p> </div> <div class='w20p'> <span id='{4}'></span> </div> <div class='w40p'> <input class='slider-a' id='{5}' max='1' min='0' step='0.001' type='range' value='0'> </div> </div> <div class='eins-he'> <div class='w35p'> <p>Tune Reference</p> </div> <div class='w20p'> <span id='{6}'></span> </div> <div class='w40p'> <input class='slider-a' id='{7}' max='1' min='0' step='0.0001' type='range' value='0'> </div> </div> </div>")
    SafeHtml html1(String arg0, String arg1, String arg2, String arg3, String arg4, String arg5, String arg6, String arg7);
     
  }

  Template template = GWT.create(Template.class);


  public com.google.gwt.user.client.ui.HTMLPanel createAndBindUi(final com.nonlinearlabs.client.world.overlay.html.soundSettings.SoundSettings owner) {


    return new Widgets(owner).get_f_HTMLPanel1();
  }

  /**
   * Encapsulates the access to all inner widgets
   */
  class Widgets {
    private final com.nonlinearlabs.client.world.overlay.html.soundSettings.SoundSettings owner;


    public Widgets(final com.nonlinearlabs.client.world.overlay.html.soundSettings.SoundSettings owner) {
      this.owner = owner;
      build_domId0();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId1();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId2();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId3();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId4();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId5();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId6();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId7();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId0Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId1Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId3Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId4Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId6Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
    }

    SafeHtml template_html1() {
      return template.html1(get_domId0(), get_domId1(), get_domId2(), get_domId3(), get_domId4(), get_domId5(), get_domId6(), get_domId7());
    }

    /**
     * Getter for clientBundleFieldNameUnlikelyToCollideWithUserSpecifiedFieldOkay called 0 times. Type: GENERATED_BUNDLE. Build precedence: 1.
     */
    private com.nonlinearlabs.client.world.overlay.html.soundSettings.SoundSettings_SoundSettingsUiBinderImpl_GenBundle get_clientBundleFieldNameUnlikelyToCollideWithUserSpecifiedFieldOkay() {
      return build_clientBundleFieldNameUnlikelyToCollideWithUserSpecifiedFieldOkay();
    }
    private com.nonlinearlabs.client.world.overlay.html.soundSettings.SoundSettings_SoundSettingsUiBinderImpl_GenBundle build_clientBundleFieldNameUnlikelyToCollideWithUserSpecifiedFieldOkay() {
      // Creation section.
      final com.nonlinearlabs.client.world.overlay.html.soundSettings.SoundSettings_SoundSettingsUiBinderImpl_GenBundle clientBundleFieldNameUnlikelyToCollideWithUserSpecifiedFieldOkay = (com.nonlinearlabs.client.world.overlay.html.soundSettings.SoundSettings_SoundSettingsUiBinderImpl_GenBundle) GWT.create(com.nonlinearlabs.client.world.overlay.html.soundSettings.SoundSettings_SoundSettingsUiBinderImpl_GenBundle.class);
      // Setup section.

      return clientBundleFieldNameUnlikelyToCollideWithUserSpecifiedFieldOkay;
    }

    /**
     * Getter for f_HTMLPanel1 called 1 times. Type: DEFAULT. Build precedence: 1.
     */
    private com.google.gwt.user.client.ui.HTMLPanel get_f_HTMLPanel1() {
      return build_f_HTMLPanel1();
    }
    private com.google.gwt.user.client.ui.HTMLPanel build_f_HTMLPanel1() {
      // Creation section.
      final com.google.gwt.user.client.ui.HTMLPanel f_HTMLPanel1 = new com.google.gwt.user.client.ui.HTMLPanel(template_html1().asString());
      // Setup section.

      {
        // Attach section.
        UiBinderUtil.TempAttachment __attachRecord__ = UiBinderUtil.attachToDom(f_HTMLPanel1.getElement());

        get_domId0Element().get();
        get_domId1Element().get();
        get_randomValueSlider();
        get_domId3Element().get();
        get_domId4Element().get();
        get_transitionTimeSlider();
        get_domId6Element().get();
        get_tuneReferenceSlider();

        // Detach section.
        __attachRecord__.detach();
      }
      f_HTMLPanel1.addAndReplaceElement(get_initialize(), get_domId0Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_randomValueDisplayString(), get_domId1Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_applyRandomization(), get_domId3Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_transitionTimeDisplayString(), get_domId4Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_tuneReferenceDisplayString(), get_domId6Element().get());

      return f_HTMLPanel1;
    }

    /**
     * Getter for domId0 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId0;
    private java.lang.String get_domId0() {
      return domId0;
    }
    private java.lang.String build_domId0() {
      // Creation section.
      domId0 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId0;
    }

    /**
     * Getter for initialize called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Button get_initialize() {
      return build_initialize();
    }
    private com.google.gwt.user.client.ui.Button build_initialize() {
      // Creation section.
      final com.google.gwt.user.client.ui.Button initialize = (com.google.gwt.user.client.ui.Button) GWT.create(com.google.gwt.user.client.ui.Button.class);
      // Setup section.
      initialize.setText("Initialize");

      this.owner.initialize = initialize;

      return initialize;
    }

    /**
     * Getter for domId0Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId0Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId0Element() {
      return domId0Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId0Element() {
      // Creation section.
      domId0Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId0());
      // Setup section.

      return domId0Element;
    }

    /**
     * Getter for domId1 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId1;
    private java.lang.String get_domId1() {
      return domId1;
    }
    private java.lang.String build_domId1() {
      // Creation section.
      domId1 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId1;
    }

    /**
     * Getter for randomValueDisplayString called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Label get_randomValueDisplayString() {
      return build_randomValueDisplayString();
    }
    private com.google.gwt.user.client.ui.Label build_randomValueDisplayString() {
      // Creation section.
      final com.google.gwt.user.client.ui.Label randomValueDisplayString = (com.google.gwt.user.client.ui.Label) GWT.create(com.google.gwt.user.client.ui.Label.class);
      // Setup section.
      randomValueDisplayString.setStyleName("gui-label-1");

      this.owner.randomValueDisplayString = randomValueDisplayString;

      return randomValueDisplayString;
    }

    /**
     * Getter for domId1Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId1Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId1Element() {
      return domId1Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId1Element() {
      // Creation section.
      domId1Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId1());
      // Setup section.

      return domId1Element;
    }

    /**
     * Getter for randomValueSlider called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.dom.client.InputElement get_randomValueSlider() {
      return build_randomValueSlider();
    }
    private com.google.gwt.dom.client.InputElement build_randomValueSlider() {
      // Creation section.
      final com.google.gwt.dom.client.InputElement randomValueSlider = new com.google.gwt.uibinder.client.LazyDomElement(get_domId2()).get().cast();
      // Setup section.

      this.owner.randomValueSlider = randomValueSlider;

      return randomValueSlider;
    }

    /**
     * Getter for domId2 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId2;
    private java.lang.String get_domId2() {
      return domId2;
    }
    private java.lang.String build_domId2() {
      // Creation section.
      domId2 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId2;
    }

    /**
     * Getter for domId3 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId3;
    private java.lang.String get_domId3() {
      return domId3;
    }
    private java.lang.String build_domId3() {
      // Creation section.
      domId3 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId3;
    }

    /**
     * Getter for applyRandomization called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Button get_applyRandomization() {
      return build_applyRandomization();
    }
    private com.google.gwt.user.client.ui.Button build_applyRandomization() {
      // Creation section.
      final com.google.gwt.user.client.ui.Button applyRandomization = (com.google.gwt.user.client.ui.Button) GWT.create(com.google.gwt.user.client.ui.Button.class);
      // Setup section.
      applyRandomization.setText("Apply");

      this.owner.applyRandomization = applyRandomization;

      return applyRandomization;
    }

    /**
     * Getter for domId3Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId3Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId3Element() {
      return domId3Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId3Element() {
      // Creation section.
      domId3Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId3());
      // Setup section.

      return domId3Element;
    }

    /**
     * Getter for domId4 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId4;
    private java.lang.String get_domId4() {
      return domId4;
    }
    private java.lang.String build_domId4() {
      // Creation section.
      domId4 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId4;
    }

    /**
     * Getter for transitionTimeDisplayString called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Label get_transitionTimeDisplayString() {
      return build_transitionTimeDisplayString();
    }
    private com.google.gwt.user.client.ui.Label build_transitionTimeDisplayString() {
      // Creation section.
      final com.google.gwt.user.client.ui.Label transitionTimeDisplayString = (com.google.gwt.user.client.ui.Label) GWT.create(com.google.gwt.user.client.ui.Label.class);
      // Setup section.
      transitionTimeDisplayString.setStyleName("gui-label-1");

      this.owner.transitionTimeDisplayString = transitionTimeDisplayString;

      return transitionTimeDisplayString;
    }

    /**
     * Getter for domId4Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId4Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId4Element() {
      return domId4Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId4Element() {
      // Creation section.
      domId4Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId4());
      // Setup section.

      return domId4Element;
    }

    /**
     * Getter for transitionTimeSlider called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.dom.client.InputElement get_transitionTimeSlider() {
      return build_transitionTimeSlider();
    }
    private com.google.gwt.dom.client.InputElement build_transitionTimeSlider() {
      // Creation section.
      final com.google.gwt.dom.client.InputElement transitionTimeSlider = new com.google.gwt.uibinder.client.LazyDomElement(get_domId5()).get().cast();
      // Setup section.

      this.owner.transitionTimeSlider = transitionTimeSlider;

      return transitionTimeSlider;
    }

    /**
     * Getter for domId5 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId5;
    private java.lang.String get_domId5() {
      return domId5;
    }
    private java.lang.String build_domId5() {
      // Creation section.
      domId5 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId5;
    }

    /**
     * Getter for domId6 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId6;
    private java.lang.String get_domId6() {
      return domId6;
    }
    private java.lang.String build_domId6() {
      // Creation section.
      domId6 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId6;
    }

    /**
     * Getter for tuneReferenceDisplayString called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Label get_tuneReferenceDisplayString() {
      return build_tuneReferenceDisplayString();
    }
    private com.google.gwt.user.client.ui.Label build_tuneReferenceDisplayString() {
      // Creation section.
      final com.google.gwt.user.client.ui.Label tuneReferenceDisplayString = (com.google.gwt.user.client.ui.Label) GWT.create(com.google.gwt.user.client.ui.Label.class);
      // Setup section.
      tuneReferenceDisplayString.setStyleName("gui-label-1");

      this.owner.tuneReferenceDisplayString = tuneReferenceDisplayString;

      return tuneReferenceDisplayString;
    }

    /**
     * Getter for domId6Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId6Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId6Element() {
      return domId6Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId6Element() {
      // Creation section.
      domId6Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId6());
      // Setup section.

      return domId6Element;
    }

    /**
     * Getter for tuneReferenceSlider called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.dom.client.InputElement get_tuneReferenceSlider() {
      return build_tuneReferenceSlider();
    }
    private com.google.gwt.dom.client.InputElement build_tuneReferenceSlider() {
      // Creation section.
      final com.google.gwt.dom.client.InputElement tuneReferenceSlider = new com.google.gwt.uibinder.client.LazyDomElement(get_domId7()).get().cast();
      // Setup section.

      this.owner.tuneReferenceSlider = tuneReferenceSlider;

      return tuneReferenceSlider;
    }

    /**
     * Getter for domId7 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId7;
    private java.lang.String get_domId7() {
      return domId7;
    }
    private java.lang.String build_domId7() {
      // Creation section.
      domId7 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId7;
    }
  }
}
