// .ui.xml template last modified: 1573733823226
package com.nonlinearlabs.client.world.overlay.html.setup;

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

public class Setup_SetupUiBinderImpl implements UiBinder<com.google.gwt.user.client.ui.HTMLPanel, com.nonlinearlabs.client.world.overlay.html.setup.Setup>, com.nonlinearlabs.client.world.overlay.html.setup.Setup.SetupUiBinder {

  interface Template extends SafeHtmlTemplates {
    @Template("Device Settings")
    SafeHtml html1();
     
    @Template("GUI Settings")
    SafeHtml html2();
     
    @Template("System Info")
    SafeHtml html3();
     
    @Template("About")
    SafeHtml html4();
     
    @Template("<div class='tabcontainer w33 html-dialog'> <div class='tab'> <span id='{0}'></span> <span id='{1}'></span> <span id='{2}'></span> <span id='{3}'></span> </div> <div class='tabcontent' id='{4}'> <div class='forms-container'> <div class='mt1'></div> <div class='eins-he'> <div class='w70p'> <p>Velocity Curve</p> </div> <div class='w30p'> <span id='{5}'></span> </div> </div> <div class='linie'></div> <div class='eins-he'> <div class='w70p'> <p>Aftertouch Curve</p> </div> <div class='w30p'> <span id='{6}'></span> </div> </div> <div class='linie'></div> <div class='eins-he'> <div class='w70p'> <p>Bender Curve</p> </div> <div class='w30p'> <span id='{7}'></span> </div> </div> <div class='linie'></div> <div class='eins-he fl-j-center'> <h3 class='uneditable'>Pedal Settings</h3> </div> <div class='eins-he'> <div class='w30p'> <p>Pedal 1</p> </div> <div class='w10p'> <span id='{8}'></span> </div> <div class='w25p'> <input class='slider-b' disabled='disabled' id='{9}' max='1' min='0' step='0.001' type='range'> </div> <div class='w5p'></div> <div class='w30p'> <span id='{10}'></span> </div> </div> <div class='eins-he'> <div class='w30p'> <p>Pedal 2</p> </div> <div class='w10p'> <span id='{11}'></span> </div> <div class='w25p'> <input class='slider-b' disabled='disabled' id='{12}' max='1' min='0' step='0.001' type='range'> </div> <div class='w5p'></div> <div class='w30p'> <span id='{13}'></span> </div> </div> <div class='eins-he'> <div class='w30p'> <p>Pedal 3</p> </div> <div class='w10p'> <span id='{14}'></span> </div> <div class='w25p'> <input class='slider-b' disabled='disabled' id='{15}' max='1' min='0' step='0.001' type='range'> </div> <div class='w5p'></div> <div class='w30p'> <span id='{16}'></span> </div> </div> <div class='eins-he'> <div class='w30p'> <p>Pedal 4</p> </div> <div class='w10p'> <span id='{17}'></span> </div> <div class='w25p'> <input class='slider-b' disabled='disabled' id='{18}' max='1' min='0' step='0.001' type='range'> </div> <div class='w5p'></div> <div class='w30p'> <span id='{19}'></span> </div> </div> <div class='linie'></div> <div class='eins-he'> <div class='w70p'> <p>Preset Glitch Surpression</p> </div> <div class='w30p mw-fit-content'> <span id='{20}'></span> <span id='{21}'></span> </div> </div> <div class='linie'></div> <div class='eins-he'> <div class='w35p'> <p>Edit Smoothing Time</p> </div> <div class='w20p'> <span id='{22}'></span> </div> <div class='w40p'> <input class='slider-a' id='{23}' max='1' min='0' step='0.001' type='range' value='0'> </div> </div> </div> </div> <div class='tabcontent' id='{24}'> <div class='forms-container'> <div class='mt1'></div> <div class='eins-he'> <div class='w60p'> <p>Selection Auto Scroll</p> </div> <div class='w40p'> <span id='{25}'></span> </div> </div> <div class='linie'></div> <div class='eins-he'> <div class='w60p'> <p>Edit Parameter</p> </div> <div class='w40p'> <span id='{26}'></span> </div> </div> <div class='linie'></div> <div class='eins-he'> <div class='w60p'> <p>Highlight Changed Parameters</p> </div> <div class='w40p mw-fit-content'> <span id='{27}'></span> <span id='{28}'></span> </div> </div> <div class='linie'></div> <div class='eins-he'> <div class='w60p'> <p>Show Context Menus</p> </div> <div class='w40p mw-fit-content'> <span id='{29}'></span> <span id='{30}'></span> </div> </div> <div class='linie'></div> <div class='eins-he'> <div class='w60p'> <p>Preset Drag and Drop</p> </div> <div class='w40p mw-fit-content'> <span id='{31}'></span> <span id='{32}'></span> </div> </div> <div class='linie'></div> <div class='eins-he'> <div class='w60p'> <p>Display Scaling Factor</p> </div> <div class='w40p'> <span id='{33}'></span> </div> </div> <div class='linie'></div> <div class='eins-he'> <div class='w60p'> <p>Stripe Brightness</p> </div> <div class='w40p'> <span id='{34}'></span> </div> </div> <div class='linie'></div> <div class='eins-he'> <div class='w60p'> <p>Bitmap Cache</p> </div> <div class='w40p mw-fit-content'> <span id='{35}'></span> <span id='{36}'></span> </div> </div> <div class='linie'></div> <div class='eins-he'> <div class='w60p'> <p>Show Developer Options</p> </div> <div class='w40p mw-fit-content'> <span id='{37}'></span> <span id='{38}'></span> </div> </div> </div> </div> <div class='tabcontent' id='{39}'> <div class='forms-container'> <div class='mt1'></div> <div class='eins-he'> <div class='w40p'> <p>Device Name</p> </div> <div class='w40p'> <span id='{40}'></span> </div> <div class='w5p'></div> <div class='w15p fl-j-center'> <span id='{41}'></span> </div> </div> <div class='eins-he'> <div class='w40p'> <p>Free Internal Memory</p> </div> <div class='w60p'> <span id='{42}'></span> </div> </div> <div class='linie'></div> <div class='mt05'></div> <div class='halb-he'> <div class='w40p'> <p>UI Software Version</p> </div> <div class='w60p'> <span id='{43}'></span> </div> </div> <div class='halb-he'> <div class='w40p'> <p>RT Software Version</p> </div> <div class='w60p'> <span id='{44}'></span> </div> </div> <div class='halb-he'> <div class='w40p'> <p>OS Version</p> </div> <div class='w60p'> <span id='{45}'></span> </div> </div>  <div class='halb-he'> <div class='w40p'> <p>Date/Time</p> </div> <div class='w60p'> <span id='{46}'></span> </div> </div> <div class='mb05'></div> </div> </div> <div class='tabcontent' id='{47}'> <div class='forms-container'> <div class='mt1'></div> <h2 class='h-blau'>C15 Development &amp; Design Team</h2> <div class='txt-area'> <p>Stephan Schmitt, Daniel Tzschentke, Simone Fabbri, Henry Högelow, Matthias Seeber, Tino Meinert, Justus Scheil, Klaus Strohhäcker, Anton Schmied, Pascal Hürst, Nemanja Nikodijevic, Gabriel Mulzer, Mike Daliot, Paul Schulz, Ulrich Wappler </p> </div> <div class='t-center mt2 mb1 h-blau'> <small> <p> NONLINEAR LABS GmbH <br> Helmholtzstraße 2-9 E <br> 10587 Berlin Germany </p> <p> www.nonlinear-labs.de <br> E-Mail: info@nonlinear-labs.de </p> </small> </div> </div> </div> </div>")
    SafeHtml html5(String arg0, String arg1, String arg2, String arg3, String arg4, String arg5, String arg6, String arg7, String arg8, String arg9, String arg10, String arg11, String arg12, String arg13, String arg14, String arg15, String arg16, String arg17, String arg18, String arg19, String arg20, String arg21, String arg22, String arg23, String arg24, String arg25, String arg26, String arg27, String arg28, String arg29, String arg30, String arg31, String arg32, String arg33, String arg34, String arg35, String arg36, String arg37, String arg38, String arg39, String arg40, String arg41, String arg42, String arg43, String arg44, String arg45, String arg46, String arg47);
     
  }

  Template template = GWT.create(Template.class);


  public com.google.gwt.user.client.ui.HTMLPanel createAndBindUi(final com.nonlinearlabs.client.world.overlay.html.setup.Setup owner) {


    return new Widgets(owner).get_f_HTMLPanel1();
  }

  /**
   * Encapsulates the access to all inner widgets
   */
  class Widgets {
    private final com.nonlinearlabs.client.world.overlay.html.setup.Setup owner;


    public Widgets(final com.nonlinearlabs.client.world.overlay.html.setup.Setup owner) {
      this.owner = owner;
      build_domId0();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId1();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId2();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId3();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId4();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId5();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId6();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId7();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId8();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId9();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId10();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId11();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId12();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId13();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId14();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId15();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId16();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId17();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId18();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId19();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId20();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId21();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId22();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId23();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId24();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId25();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId26();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId27();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId28();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId29();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId30();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId31();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId32();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId33();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId34();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId35();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId36();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId37();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId38();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId39();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId40();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId41();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId42();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId43();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId44();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId45();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId46();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId47();  // more than one getter call detected. Type: DOM_ID_HOLDER, precedence: 2
      build_domId0Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId1Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId2Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId3Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId5Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId6Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId7Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId8Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId10Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId11Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId13Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId14Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId16Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId17Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId19Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId20Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId21Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId22Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId25Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId26Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId27Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId28Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId29Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId30Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId31Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId32Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId33Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId34Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId35Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId36Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId37Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId38Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId40Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId41Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId42Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId43Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId44Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId45Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
      build_domId46Element();  // more than one getter call detected. Type: DEFAULT, precedence: 2
    }

    SafeHtml template_html1() {
      return template.html1();
    }
    SafeHtml template_html2() {
      return template.html2();
    }
    SafeHtml template_html3() {
      return template.html3();
    }
    SafeHtml template_html4() {
      return template.html4();
    }
    SafeHtml template_html5() {
      return template.html5(get_domId0(), get_domId1(), get_domId2(), get_domId3(), get_domId4(), get_domId5(), get_domId6(), get_domId7(), get_domId8(), get_domId9(), get_domId10(), get_domId11(), get_domId12(), get_domId13(), get_domId14(), get_domId15(), get_domId16(), get_domId17(), get_domId18(), get_domId19(), get_domId20(), get_domId21(), get_domId22(), get_domId23(), get_domId24(), get_domId25(), get_domId26(), get_domId27(), get_domId28(), get_domId29(), get_domId30(), get_domId31(), get_domId32(), get_domId33(), get_domId34(), get_domId35(), get_domId36(), get_domId37(), get_domId38(), get_domId39(), get_domId40(), get_domId41(), get_domId42(), get_domId43(), get_domId44(), get_domId45(), get_domId46(), get_domId47());
    }

    /**
     * Getter for clientBundleFieldNameUnlikelyToCollideWithUserSpecifiedFieldOkay called 0 times. Type: GENERATED_BUNDLE. Build precedence: 1.
     */
    private com.nonlinearlabs.client.world.overlay.html.setup.Setup_SetupUiBinderImpl_GenBundle get_clientBundleFieldNameUnlikelyToCollideWithUserSpecifiedFieldOkay() {
      return build_clientBundleFieldNameUnlikelyToCollideWithUserSpecifiedFieldOkay();
    }
    private com.nonlinearlabs.client.world.overlay.html.setup.Setup_SetupUiBinderImpl_GenBundle build_clientBundleFieldNameUnlikelyToCollideWithUserSpecifiedFieldOkay() {
      // Creation section.
      final com.nonlinearlabs.client.world.overlay.html.setup.Setup_SetupUiBinderImpl_GenBundle clientBundleFieldNameUnlikelyToCollideWithUserSpecifiedFieldOkay = (com.nonlinearlabs.client.world.overlay.html.setup.Setup_SetupUiBinderImpl_GenBundle) GWT.create(com.nonlinearlabs.client.world.overlay.html.setup.Setup_SetupUiBinderImpl_GenBundle.class);
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
      final com.google.gwt.user.client.ui.HTMLPanel f_HTMLPanel1 = new com.google.gwt.user.client.ui.HTMLPanel(template_html5().asString());
      // Setup section.

      {
        // Attach section.
        UiBinderUtil.TempAttachment __attachRecord__ = UiBinderUtil.attachToDom(f_HTMLPanel1.getElement());

        get_domId0Element().get();
        get_domId1Element().get();
        get_domId2Element().get();
        get_domId3Element().get();
        get_deviceSettings();
        get_domId5Element().get();
        get_domId6Element().get();
        get_domId7Element().get();
        get_domId8Element().get();
        get_pedal1Slider();
        get_domId10Element().get();
        get_domId11Element().get();
        get_pedal2Slider();
        get_domId13Element().get();
        get_domId14Element().get();
        get_pedal3Slider();
        get_domId16Element().get();
        get_domId17Element().get();
        get_pedal4Slider();
        get_domId19Element().get();
        get_domId20Element().get();
        get_domId21Element().get();
        get_domId22Element().get();
        get_editSmoothingTimeSlider();
        get_uiSettings();
        get_domId25Element().get();
        get_domId26Element().get();
        get_domId27Element().get();
        get_domId28Element().get();
        get_domId29Element().get();
        get_domId30Element().get();
        get_domId31Element().get();
        get_domId32Element().get();
        get_domId33Element().get();
        get_domId34Element().get();
        get_domId35Element().get();
        get_domId36Element().get();
        get_domId37Element().get();
        get_domId38Element().get();
        get_systemInfo();
        get_domId40Element().get();
        get_domId41Element().get();
        get_domId42Element().get();
        get_domId43Element().get();
        get_domId44Element().get();
        get_domId45Element().get();
        get_domId46Element().get();
        get_about();

        // Detach section.
        __attachRecord__.detach();
      }
      f_HTMLPanel1.addAndReplaceElement(get_deviceSettingsButton(), get_domId0Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_uiSettingsButton(), get_domId1Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_systemInfoButton(), get_domId2Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_aboutButton(), get_domId3Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_velocityCurve(), get_domId5Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_aftertouchCurve(), get_domId6Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_benderCurve(), get_domId7Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_pedal1DisplayString(), get_domId8Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_pedal1Type(), get_domId10Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_pedal2DisplayString(), get_domId11Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_pedal2Type(), get_domId13Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_pedal3DisplayString(), get_domId14Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_pedal3Type(), get_domId16Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_pedal4DisplayString(), get_domId17Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_pedal4Type(), get_domId19Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_presetGlitchSurpressionOn(), get_domId20Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_presetGlitchSurpressionOff(), get_domId21Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_editSmoothingTimeDisplayString(), get_domId22Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_selectionAutoScroll(), get_domId25Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_editParameter(), get_domId26Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_highlightChangedOn(), get_domId27Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_highlightChangedOff(), get_domId28Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_showContextMenusOn(), get_domId29Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_showContextMenusOff(), get_domId30Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_presetDragDropOn(), get_domId31Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_presetDragDropOff(), get_domId32Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_scalingFactor(), get_domId33Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_stripeBrightness(), get_domId34Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_bitmapCacheOn(), get_domId35Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_bitmapCacheOff(), get_domId36Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_developerOptionsOn(), get_domId37Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_developerOptionsOff(), get_domId38Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_deviceName(), get_domId40Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_saveDeviceName(), get_domId41Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_freeMemory(), get_domId42Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_uiVersion(), get_domId43Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_rtVersion(), get_domId44Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_osVersion(), get_domId45Element().get());
      f_HTMLPanel1.addAndReplaceElement(get_dateTime(), get_domId46Element().get());

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
     * Getter for deviceSettingsButton called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Button get_deviceSettingsButton() {
      return build_deviceSettingsButton();
    }
    private com.google.gwt.user.client.ui.Button build_deviceSettingsButton() {
      // Creation section.
      final com.google.gwt.user.client.ui.Button deviceSettingsButton = (com.google.gwt.user.client.ui.Button) GWT.create(com.google.gwt.user.client.ui.Button.class);
      // Setup section.
      deviceSettingsButton.setHTML(template_html1().asString());
      deviceSettingsButton.setStyleName("tabheader");

      this.owner.deviceSettingsButton = deviceSettingsButton;

      return deviceSettingsButton;
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
     * Getter for uiSettingsButton called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Button get_uiSettingsButton() {
      return build_uiSettingsButton();
    }
    private com.google.gwt.user.client.ui.Button build_uiSettingsButton() {
      // Creation section.
      final com.google.gwt.user.client.ui.Button uiSettingsButton = (com.google.gwt.user.client.ui.Button) GWT.create(com.google.gwt.user.client.ui.Button.class);
      // Setup section.
      uiSettingsButton.setHTML(template_html2().asString());
      uiSettingsButton.setStyleName("tabheader");

      this.owner.uiSettingsButton = uiSettingsButton;

      return uiSettingsButton;
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
     * Getter for systemInfoButton called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Button get_systemInfoButton() {
      return build_systemInfoButton();
    }
    private com.google.gwt.user.client.ui.Button build_systemInfoButton() {
      // Creation section.
      final com.google.gwt.user.client.ui.Button systemInfoButton = (com.google.gwt.user.client.ui.Button) GWT.create(com.google.gwt.user.client.ui.Button.class);
      // Setup section.
      systemInfoButton.setHTML(template_html3().asString());
      systemInfoButton.setStyleName("tabheader");

      this.owner.systemInfoButton = systemInfoButton;

      return systemInfoButton;
    }

    /**
     * Getter for domId2Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId2Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId2Element() {
      return domId2Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId2Element() {
      // Creation section.
      domId2Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId2());
      // Setup section.

      return domId2Element;
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
     * Getter for aboutButton called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Button get_aboutButton() {
      return build_aboutButton();
    }
    private com.google.gwt.user.client.ui.Button build_aboutButton() {
      // Creation section.
      final com.google.gwt.user.client.ui.Button aboutButton = (com.google.gwt.user.client.ui.Button) GWT.create(com.google.gwt.user.client.ui.Button.class);
      // Setup section.
      aboutButton.setHTML(template_html4().asString());
      aboutButton.setStyleName("tabheader");

      this.owner.aboutButton = aboutButton;

      return aboutButton;
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
     * Getter for deviceSettings called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.dom.client.DivElement get_deviceSettings() {
      return build_deviceSettings();
    }
    private com.google.gwt.dom.client.DivElement build_deviceSettings() {
      // Creation section.
      final com.google.gwt.dom.client.DivElement deviceSettings = new com.google.gwt.uibinder.client.LazyDomElement(get_domId4()).get().cast();
      // Setup section.

      this.owner.deviceSettings = deviceSettings;

      return deviceSettings;
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
     * Getter for velocityCurve called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.ListBox get_velocityCurve() {
      return build_velocityCurve();
    }
    private com.google.gwt.user.client.ui.ListBox build_velocityCurve() {
      // Creation section.
      final com.google.gwt.user.client.ui.ListBox velocityCurve = (com.google.gwt.user.client.ui.ListBox) GWT.create(com.google.gwt.user.client.ui.ListBox.class);
      // Setup section.

      this.owner.velocityCurve = velocityCurve;

      return velocityCurve;
    }

    /**
     * Getter for domId5Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId5Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId5Element() {
      return domId5Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId5Element() {
      // Creation section.
      domId5Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId5());
      // Setup section.

      return domId5Element;
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
     * Getter for aftertouchCurve called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.ListBox get_aftertouchCurve() {
      return build_aftertouchCurve();
    }
    private com.google.gwt.user.client.ui.ListBox build_aftertouchCurve() {
      // Creation section.
      final com.google.gwt.user.client.ui.ListBox aftertouchCurve = (com.google.gwt.user.client.ui.ListBox) GWT.create(com.google.gwt.user.client.ui.ListBox.class);
      // Setup section.

      this.owner.aftertouchCurve = aftertouchCurve;

      return aftertouchCurve;
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

    /**
     * Getter for benderCurve called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.ListBox get_benderCurve() {
      return build_benderCurve();
    }
    private com.google.gwt.user.client.ui.ListBox build_benderCurve() {
      // Creation section.
      final com.google.gwt.user.client.ui.ListBox benderCurve = (com.google.gwt.user.client.ui.ListBox) GWT.create(com.google.gwt.user.client.ui.ListBox.class);
      // Setup section.

      this.owner.benderCurve = benderCurve;

      return benderCurve;
    }

    /**
     * Getter for domId7Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId7Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId7Element() {
      return domId7Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId7Element() {
      // Creation section.
      domId7Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId7());
      // Setup section.

      return domId7Element;
    }

    /**
     * Getter for domId8 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId8;
    private java.lang.String get_domId8() {
      return domId8;
    }
    private java.lang.String build_domId8() {
      // Creation section.
      domId8 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId8;
    }

    /**
     * Getter for pedal1DisplayString called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Label get_pedal1DisplayString() {
      return build_pedal1DisplayString();
    }
    private com.google.gwt.user.client.ui.Label build_pedal1DisplayString() {
      // Creation section.
      final com.google.gwt.user.client.ui.Label pedal1DisplayString = (com.google.gwt.user.client.ui.Label) GWT.create(com.google.gwt.user.client.ui.Label.class);
      // Setup section.
      pedal1DisplayString.setText("90 %");
      pedal1DisplayString.setStyleName("gui-label-1");

      this.owner.pedal1DisplayString = pedal1DisplayString;

      return pedal1DisplayString;
    }

    /**
     * Getter for domId8Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId8Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId8Element() {
      return domId8Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId8Element() {
      // Creation section.
      domId8Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId8());
      // Setup section.

      return domId8Element;
    }

    /**
     * Getter for pedal1Slider called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.dom.client.InputElement get_pedal1Slider() {
      return build_pedal1Slider();
    }
    private com.google.gwt.dom.client.InputElement build_pedal1Slider() {
      // Creation section.
      final com.google.gwt.dom.client.InputElement pedal1Slider = new com.google.gwt.uibinder.client.LazyDomElement(get_domId9()).get().cast();
      // Setup section.

      this.owner.pedal1Slider = pedal1Slider;

      return pedal1Slider;
    }

    /**
     * Getter for domId9 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId9;
    private java.lang.String get_domId9() {
      return domId9;
    }
    private java.lang.String build_domId9() {
      // Creation section.
      domId9 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId9;
    }

    /**
     * Getter for domId10 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId10;
    private java.lang.String get_domId10() {
      return domId10;
    }
    private java.lang.String build_domId10() {
      // Creation section.
      domId10 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId10;
    }

    /**
     * Getter for pedal1Type called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.ListBox get_pedal1Type() {
      return build_pedal1Type();
    }
    private com.google.gwt.user.client.ui.ListBox build_pedal1Type() {
      // Creation section.
      final com.google.gwt.user.client.ui.ListBox pedal1Type = (com.google.gwt.user.client.ui.ListBox) GWT.create(com.google.gwt.user.client.ui.ListBox.class);
      // Setup section.

      this.owner.pedal1Type = pedal1Type;

      return pedal1Type;
    }

    /**
     * Getter for domId10Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId10Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId10Element() {
      return domId10Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId10Element() {
      // Creation section.
      domId10Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId10());
      // Setup section.

      return domId10Element;
    }

    /**
     * Getter for domId11 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId11;
    private java.lang.String get_domId11() {
      return domId11;
    }
    private java.lang.String build_domId11() {
      // Creation section.
      domId11 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId11;
    }

    /**
     * Getter for pedal2DisplayString called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Label get_pedal2DisplayString() {
      return build_pedal2DisplayString();
    }
    private com.google.gwt.user.client.ui.Label build_pedal2DisplayString() {
      // Creation section.
      final com.google.gwt.user.client.ui.Label pedal2DisplayString = (com.google.gwt.user.client.ui.Label) GWT.create(com.google.gwt.user.client.ui.Label.class);
      // Setup section.
      pedal2DisplayString.setText("90 %");
      pedal2DisplayString.setStyleName("gui-label-1");

      this.owner.pedal2DisplayString = pedal2DisplayString;

      return pedal2DisplayString;
    }

    /**
     * Getter for domId11Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId11Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId11Element() {
      return domId11Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId11Element() {
      // Creation section.
      domId11Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId11());
      // Setup section.

      return domId11Element;
    }

    /**
     * Getter for pedal2Slider called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.dom.client.InputElement get_pedal2Slider() {
      return build_pedal2Slider();
    }
    private com.google.gwt.dom.client.InputElement build_pedal2Slider() {
      // Creation section.
      final com.google.gwt.dom.client.InputElement pedal2Slider = new com.google.gwt.uibinder.client.LazyDomElement(get_domId12()).get().cast();
      // Setup section.

      this.owner.pedal2Slider = pedal2Slider;

      return pedal2Slider;
    }

    /**
     * Getter for domId12 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId12;
    private java.lang.String get_domId12() {
      return domId12;
    }
    private java.lang.String build_domId12() {
      // Creation section.
      domId12 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId12;
    }

    /**
     * Getter for domId13 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId13;
    private java.lang.String get_domId13() {
      return domId13;
    }
    private java.lang.String build_domId13() {
      // Creation section.
      domId13 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId13;
    }

    /**
     * Getter for pedal2Type called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.ListBox get_pedal2Type() {
      return build_pedal2Type();
    }
    private com.google.gwt.user.client.ui.ListBox build_pedal2Type() {
      // Creation section.
      final com.google.gwt.user.client.ui.ListBox pedal2Type = (com.google.gwt.user.client.ui.ListBox) GWT.create(com.google.gwt.user.client.ui.ListBox.class);
      // Setup section.

      this.owner.pedal2Type = pedal2Type;

      return pedal2Type;
    }

    /**
     * Getter for domId13Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId13Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId13Element() {
      return domId13Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId13Element() {
      // Creation section.
      domId13Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId13());
      // Setup section.

      return domId13Element;
    }

    /**
     * Getter for domId14 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId14;
    private java.lang.String get_domId14() {
      return domId14;
    }
    private java.lang.String build_domId14() {
      // Creation section.
      domId14 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId14;
    }

    /**
     * Getter for pedal3DisplayString called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Label get_pedal3DisplayString() {
      return build_pedal3DisplayString();
    }
    private com.google.gwt.user.client.ui.Label build_pedal3DisplayString() {
      // Creation section.
      final com.google.gwt.user.client.ui.Label pedal3DisplayString = (com.google.gwt.user.client.ui.Label) GWT.create(com.google.gwt.user.client.ui.Label.class);
      // Setup section.
      pedal3DisplayString.setText("90 %");
      pedal3DisplayString.setStyleName("gui-label-1");

      this.owner.pedal3DisplayString = pedal3DisplayString;

      return pedal3DisplayString;
    }

    /**
     * Getter for domId14Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId14Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId14Element() {
      return domId14Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId14Element() {
      // Creation section.
      domId14Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId14());
      // Setup section.

      return domId14Element;
    }

    /**
     * Getter for pedal3Slider called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.dom.client.InputElement get_pedal3Slider() {
      return build_pedal3Slider();
    }
    private com.google.gwt.dom.client.InputElement build_pedal3Slider() {
      // Creation section.
      final com.google.gwt.dom.client.InputElement pedal3Slider = new com.google.gwt.uibinder.client.LazyDomElement(get_domId15()).get().cast();
      // Setup section.

      this.owner.pedal3Slider = pedal3Slider;

      return pedal3Slider;
    }

    /**
     * Getter for domId15 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId15;
    private java.lang.String get_domId15() {
      return domId15;
    }
    private java.lang.String build_domId15() {
      // Creation section.
      domId15 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId15;
    }

    /**
     * Getter for domId16 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId16;
    private java.lang.String get_domId16() {
      return domId16;
    }
    private java.lang.String build_domId16() {
      // Creation section.
      domId16 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId16;
    }

    /**
     * Getter for pedal3Type called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.ListBox get_pedal3Type() {
      return build_pedal3Type();
    }
    private com.google.gwt.user.client.ui.ListBox build_pedal3Type() {
      // Creation section.
      final com.google.gwt.user.client.ui.ListBox pedal3Type = (com.google.gwt.user.client.ui.ListBox) GWT.create(com.google.gwt.user.client.ui.ListBox.class);
      // Setup section.

      this.owner.pedal3Type = pedal3Type;

      return pedal3Type;
    }

    /**
     * Getter for domId16Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId16Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId16Element() {
      return domId16Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId16Element() {
      // Creation section.
      domId16Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId16());
      // Setup section.

      return domId16Element;
    }

    /**
     * Getter for domId17 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId17;
    private java.lang.String get_domId17() {
      return domId17;
    }
    private java.lang.String build_domId17() {
      // Creation section.
      domId17 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId17;
    }

    /**
     * Getter for pedal4DisplayString called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Label get_pedal4DisplayString() {
      return build_pedal4DisplayString();
    }
    private com.google.gwt.user.client.ui.Label build_pedal4DisplayString() {
      // Creation section.
      final com.google.gwt.user.client.ui.Label pedal4DisplayString = (com.google.gwt.user.client.ui.Label) GWT.create(com.google.gwt.user.client.ui.Label.class);
      // Setup section.
      pedal4DisplayString.setText("90 %");
      pedal4DisplayString.setStyleName("gui-label-1");

      this.owner.pedal4DisplayString = pedal4DisplayString;

      return pedal4DisplayString;
    }

    /**
     * Getter for domId17Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId17Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId17Element() {
      return domId17Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId17Element() {
      // Creation section.
      domId17Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId17());
      // Setup section.

      return domId17Element;
    }

    /**
     * Getter for pedal4Slider called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.dom.client.InputElement get_pedal4Slider() {
      return build_pedal4Slider();
    }
    private com.google.gwt.dom.client.InputElement build_pedal4Slider() {
      // Creation section.
      final com.google.gwt.dom.client.InputElement pedal4Slider = new com.google.gwt.uibinder.client.LazyDomElement(get_domId18()).get().cast();
      // Setup section.

      this.owner.pedal4Slider = pedal4Slider;

      return pedal4Slider;
    }

    /**
     * Getter for domId18 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId18;
    private java.lang.String get_domId18() {
      return domId18;
    }
    private java.lang.String build_domId18() {
      // Creation section.
      domId18 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId18;
    }

    /**
     * Getter for domId19 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId19;
    private java.lang.String get_domId19() {
      return domId19;
    }
    private java.lang.String build_domId19() {
      // Creation section.
      domId19 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId19;
    }

    /**
     * Getter for pedal4Type called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.ListBox get_pedal4Type() {
      return build_pedal4Type();
    }
    private com.google.gwt.user.client.ui.ListBox build_pedal4Type() {
      // Creation section.
      final com.google.gwt.user.client.ui.ListBox pedal4Type = (com.google.gwt.user.client.ui.ListBox) GWT.create(com.google.gwt.user.client.ui.ListBox.class);
      // Setup section.

      this.owner.pedal4Type = pedal4Type;

      return pedal4Type;
    }

    /**
     * Getter for domId19Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId19Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId19Element() {
      return domId19Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId19Element() {
      // Creation section.
      domId19Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId19());
      // Setup section.

      return domId19Element;
    }

    /**
     * Getter for domId20 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId20;
    private java.lang.String get_domId20() {
      return domId20;
    }
    private java.lang.String build_domId20() {
      // Creation section.
      domId20 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId20;
    }

    /**
     * Getter for presetGlitchSurpressionOn called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.RadioButton get_presetGlitchSurpressionOn() {
      return build_presetGlitchSurpressionOn();
    }
    private com.google.gwt.user.client.ui.RadioButton build_presetGlitchSurpressionOn() {
      // Creation section.
      final com.google.gwt.user.client.ui.RadioButton presetGlitchSurpressionOn = new com.google.gwt.user.client.ui.RadioButton("rad_pgs");
      // Setup section.
      presetGlitchSurpressionOn.setValue(true);

      this.owner.presetGlitchSurpressionOn = presetGlitchSurpressionOn;

      return presetGlitchSurpressionOn;
    }

    /**
     * Getter for domId20Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId20Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId20Element() {
      return domId20Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId20Element() {
      // Creation section.
      domId20Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId20());
      // Setup section.

      return domId20Element;
    }

    /**
     * Getter for domId21 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId21;
    private java.lang.String get_domId21() {
      return domId21;
    }
    private java.lang.String build_domId21() {
      // Creation section.
      domId21 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId21;
    }

    /**
     * Getter for presetGlitchSurpressionOff called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.RadioButton get_presetGlitchSurpressionOff() {
      return build_presetGlitchSurpressionOff();
    }
    private com.google.gwt.user.client.ui.RadioButton build_presetGlitchSurpressionOff() {
      // Creation section.
      final com.google.gwt.user.client.ui.RadioButton presetGlitchSurpressionOff = new com.google.gwt.user.client.ui.RadioButton("rad_pgs");
      // Setup section.
      presetGlitchSurpressionOff.setValue(false);

      this.owner.presetGlitchSurpressionOff = presetGlitchSurpressionOff;

      return presetGlitchSurpressionOff;
    }

    /**
     * Getter for domId21Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId21Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId21Element() {
      return domId21Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId21Element() {
      // Creation section.
      domId21Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId21());
      // Setup section.

      return domId21Element;
    }

    /**
     * Getter for domId22 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId22;
    private java.lang.String get_domId22() {
      return domId22;
    }
    private java.lang.String build_domId22() {
      // Creation section.
      domId22 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId22;
    }

    /**
     * Getter for editSmoothingTimeDisplayString called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Label get_editSmoothingTimeDisplayString() {
      return build_editSmoothingTimeDisplayString();
    }
    private com.google.gwt.user.client.ui.Label build_editSmoothingTimeDisplayString() {
      // Creation section.
      final com.google.gwt.user.client.ui.Label editSmoothingTimeDisplayString = (com.google.gwt.user.client.ui.Label) GWT.create(com.google.gwt.user.client.ui.Label.class);
      // Setup section.
      editSmoothingTimeDisplayString.setText("90 %");
      editSmoothingTimeDisplayString.setStyleName("gui-label-1");

      this.owner.editSmoothingTimeDisplayString = editSmoothingTimeDisplayString;

      return editSmoothingTimeDisplayString;
    }

    /**
     * Getter for domId22Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId22Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId22Element() {
      return domId22Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId22Element() {
      // Creation section.
      domId22Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId22());
      // Setup section.

      return domId22Element;
    }

    /**
     * Getter for editSmoothingTimeSlider called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.dom.client.InputElement get_editSmoothingTimeSlider() {
      return build_editSmoothingTimeSlider();
    }
    private com.google.gwt.dom.client.InputElement build_editSmoothingTimeSlider() {
      // Creation section.
      final com.google.gwt.dom.client.InputElement editSmoothingTimeSlider = new com.google.gwt.uibinder.client.LazyDomElement(get_domId23()).get().cast();
      // Setup section.

      this.owner.editSmoothingTimeSlider = editSmoothingTimeSlider;

      return editSmoothingTimeSlider;
    }

    /**
     * Getter for domId23 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId23;
    private java.lang.String get_domId23() {
      return domId23;
    }
    private java.lang.String build_domId23() {
      // Creation section.
      domId23 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId23;
    }

    /**
     * Getter for uiSettings called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.dom.client.DivElement get_uiSettings() {
      return build_uiSettings();
    }
    private com.google.gwt.dom.client.DivElement build_uiSettings() {
      // Creation section.
      final com.google.gwt.dom.client.DivElement uiSettings = new com.google.gwt.uibinder.client.LazyDomElement(get_domId24()).get().cast();
      // Setup section.

      this.owner.uiSettings = uiSettings;

      return uiSettings;
    }

    /**
     * Getter for domId24 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId24;
    private java.lang.String get_domId24() {
      return domId24;
    }
    private java.lang.String build_domId24() {
      // Creation section.
      domId24 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId24;
    }

    /**
     * Getter for domId25 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId25;
    private java.lang.String get_domId25() {
      return domId25;
    }
    private java.lang.String build_domId25() {
      // Creation section.
      domId25 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId25;
    }

    /**
     * Getter for selectionAutoScroll called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.ListBox get_selectionAutoScroll() {
      return build_selectionAutoScroll();
    }
    private com.google.gwt.user.client.ui.ListBox build_selectionAutoScroll() {
      // Creation section.
      final com.google.gwt.user.client.ui.ListBox selectionAutoScroll = (com.google.gwt.user.client.ui.ListBox) GWT.create(com.google.gwt.user.client.ui.ListBox.class);
      // Setup section.

      this.owner.selectionAutoScroll = selectionAutoScroll;

      return selectionAutoScroll;
    }

    /**
     * Getter for domId25Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId25Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId25Element() {
      return domId25Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId25Element() {
      // Creation section.
      domId25Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId25());
      // Setup section.

      return domId25Element;
    }

    /**
     * Getter for domId26 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId26;
    private java.lang.String get_domId26() {
      return domId26;
    }
    private java.lang.String build_domId26() {
      // Creation section.
      domId26 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId26;
    }

    /**
     * Getter for editParameter called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.ListBox get_editParameter() {
      return build_editParameter();
    }
    private com.google.gwt.user.client.ui.ListBox build_editParameter() {
      // Creation section.
      final com.google.gwt.user.client.ui.ListBox editParameter = (com.google.gwt.user.client.ui.ListBox) GWT.create(com.google.gwt.user.client.ui.ListBox.class);
      // Setup section.

      this.owner.editParameter = editParameter;

      return editParameter;
    }

    /**
     * Getter for domId26Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId26Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId26Element() {
      return domId26Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId26Element() {
      // Creation section.
      domId26Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId26());
      // Setup section.

      return domId26Element;
    }

    /**
     * Getter for domId27 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId27;
    private java.lang.String get_domId27() {
      return domId27;
    }
    private java.lang.String build_domId27() {
      // Creation section.
      domId27 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId27;
    }

    /**
     * Getter for highlightChangedOn called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.RadioButton get_highlightChangedOn() {
      return build_highlightChangedOn();
    }
    private com.google.gwt.user.client.ui.RadioButton build_highlightChangedOn() {
      // Creation section.
      final com.google.gwt.user.client.ui.RadioButton highlightChangedOn = new com.google.gwt.user.client.ui.RadioButton("rad_hcp");
      // Setup section.
      highlightChangedOn.setValue(true);

      this.owner.highlightChangedOn = highlightChangedOn;

      return highlightChangedOn;
    }

    /**
     * Getter for domId27Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId27Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId27Element() {
      return domId27Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId27Element() {
      // Creation section.
      domId27Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId27());
      // Setup section.

      return domId27Element;
    }

    /**
     * Getter for domId28 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId28;
    private java.lang.String get_domId28() {
      return domId28;
    }
    private java.lang.String build_domId28() {
      // Creation section.
      domId28 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId28;
    }

    /**
     * Getter for highlightChangedOff called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.RadioButton get_highlightChangedOff() {
      return build_highlightChangedOff();
    }
    private com.google.gwt.user.client.ui.RadioButton build_highlightChangedOff() {
      // Creation section.
      final com.google.gwt.user.client.ui.RadioButton highlightChangedOff = new com.google.gwt.user.client.ui.RadioButton("rad_hcp");
      // Setup section.
      highlightChangedOff.setValue(false);

      this.owner.highlightChangedOff = highlightChangedOff;

      return highlightChangedOff;
    }

    /**
     * Getter for domId28Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId28Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId28Element() {
      return domId28Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId28Element() {
      // Creation section.
      domId28Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId28());
      // Setup section.

      return domId28Element;
    }

    /**
     * Getter for domId29 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId29;
    private java.lang.String get_domId29() {
      return domId29;
    }
    private java.lang.String build_domId29() {
      // Creation section.
      domId29 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId29;
    }

    /**
     * Getter for showContextMenusOn called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.RadioButton get_showContextMenusOn() {
      return build_showContextMenusOn();
    }
    private com.google.gwt.user.client.ui.RadioButton build_showContextMenusOn() {
      // Creation section.
      final com.google.gwt.user.client.ui.RadioButton showContextMenusOn = new com.google.gwt.user.client.ui.RadioButton("rad_scm");
      // Setup section.
      showContextMenusOn.setValue(true);

      this.owner.showContextMenusOn = showContextMenusOn;

      return showContextMenusOn;
    }

    /**
     * Getter for domId29Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId29Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId29Element() {
      return domId29Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId29Element() {
      // Creation section.
      domId29Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId29());
      // Setup section.

      return domId29Element;
    }

    /**
     * Getter for domId30 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId30;
    private java.lang.String get_domId30() {
      return domId30;
    }
    private java.lang.String build_domId30() {
      // Creation section.
      domId30 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId30;
    }

    /**
     * Getter for showContextMenusOff called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.RadioButton get_showContextMenusOff() {
      return build_showContextMenusOff();
    }
    private com.google.gwt.user.client.ui.RadioButton build_showContextMenusOff() {
      // Creation section.
      final com.google.gwt.user.client.ui.RadioButton showContextMenusOff = new com.google.gwt.user.client.ui.RadioButton("rad_scm");
      // Setup section.
      showContextMenusOff.setValue(false);

      this.owner.showContextMenusOff = showContextMenusOff;

      return showContextMenusOff;
    }

    /**
     * Getter for domId30Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId30Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId30Element() {
      return domId30Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId30Element() {
      // Creation section.
      domId30Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId30());
      // Setup section.

      return domId30Element;
    }

    /**
     * Getter for domId31 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId31;
    private java.lang.String get_domId31() {
      return domId31;
    }
    private java.lang.String build_domId31() {
      // Creation section.
      domId31 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId31;
    }

    /**
     * Getter for presetDragDropOn called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.RadioButton get_presetDragDropOn() {
      return build_presetDragDropOn();
    }
    private com.google.gwt.user.client.ui.RadioButton build_presetDragDropOn() {
      // Creation section.
      final com.google.gwt.user.client.ui.RadioButton presetDragDropOn = new com.google.gwt.user.client.ui.RadioButton("rad_pdd");
      // Setup section.
      presetDragDropOn.setValue(true);

      this.owner.presetDragDropOn = presetDragDropOn;

      return presetDragDropOn;
    }

    /**
     * Getter for domId31Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId31Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId31Element() {
      return domId31Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId31Element() {
      // Creation section.
      domId31Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId31());
      // Setup section.

      return domId31Element;
    }

    /**
     * Getter for domId32 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId32;
    private java.lang.String get_domId32() {
      return domId32;
    }
    private java.lang.String build_domId32() {
      // Creation section.
      domId32 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId32;
    }

    /**
     * Getter for presetDragDropOff called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.RadioButton get_presetDragDropOff() {
      return build_presetDragDropOff();
    }
    private com.google.gwt.user.client.ui.RadioButton build_presetDragDropOff() {
      // Creation section.
      final com.google.gwt.user.client.ui.RadioButton presetDragDropOff = new com.google.gwt.user.client.ui.RadioButton("rad_pdd");
      // Setup section.
      presetDragDropOff.setValue(false);

      this.owner.presetDragDropOff = presetDragDropOff;

      return presetDragDropOff;
    }

    /**
     * Getter for domId32Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId32Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId32Element() {
      return domId32Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId32Element() {
      // Creation section.
      domId32Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId32());
      // Setup section.

      return domId32Element;
    }

    /**
     * Getter for domId33 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId33;
    private java.lang.String get_domId33() {
      return domId33;
    }
    private java.lang.String build_domId33() {
      // Creation section.
      domId33 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId33;
    }

    /**
     * Getter for scalingFactor called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.ListBox get_scalingFactor() {
      return build_scalingFactor();
    }
    private com.google.gwt.user.client.ui.ListBox build_scalingFactor() {
      // Creation section.
      final com.google.gwt.user.client.ui.ListBox scalingFactor = (com.google.gwt.user.client.ui.ListBox) GWT.create(com.google.gwt.user.client.ui.ListBox.class);
      // Setup section.

      this.owner.scalingFactor = scalingFactor;

      return scalingFactor;
    }

    /**
     * Getter for domId33Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId33Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId33Element() {
      return domId33Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId33Element() {
      // Creation section.
      domId33Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId33());
      // Setup section.

      return domId33Element;
    }

    /**
     * Getter for domId34 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId34;
    private java.lang.String get_domId34() {
      return domId34;
    }
    private java.lang.String build_domId34() {
      // Creation section.
      domId34 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId34;
    }

    /**
     * Getter for stripeBrightness called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.ListBox get_stripeBrightness() {
      return build_stripeBrightness();
    }
    private com.google.gwt.user.client.ui.ListBox build_stripeBrightness() {
      // Creation section.
      final com.google.gwt.user.client.ui.ListBox stripeBrightness = (com.google.gwt.user.client.ui.ListBox) GWT.create(com.google.gwt.user.client.ui.ListBox.class);
      // Setup section.

      this.owner.stripeBrightness = stripeBrightness;

      return stripeBrightness;
    }

    /**
     * Getter for domId34Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId34Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId34Element() {
      return domId34Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId34Element() {
      // Creation section.
      domId34Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId34());
      // Setup section.

      return domId34Element;
    }

    /**
     * Getter for domId35 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId35;
    private java.lang.String get_domId35() {
      return domId35;
    }
    private java.lang.String build_domId35() {
      // Creation section.
      domId35 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId35;
    }

    /**
     * Getter for bitmapCacheOn called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.RadioButton get_bitmapCacheOn() {
      return build_bitmapCacheOn();
    }
    private com.google.gwt.user.client.ui.RadioButton build_bitmapCacheOn() {
      // Creation section.
      final com.google.gwt.user.client.ui.RadioButton bitmapCacheOn = new com.google.gwt.user.client.ui.RadioButton("rad_bmpc");
      // Setup section.
      bitmapCacheOn.setValue(true);

      this.owner.bitmapCacheOn = bitmapCacheOn;

      return bitmapCacheOn;
    }

    /**
     * Getter for domId35Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId35Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId35Element() {
      return domId35Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId35Element() {
      // Creation section.
      domId35Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId35());
      // Setup section.

      return domId35Element;
    }

    /**
     * Getter for domId36 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId36;
    private java.lang.String get_domId36() {
      return domId36;
    }
    private java.lang.String build_domId36() {
      // Creation section.
      domId36 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId36;
    }

    /**
     * Getter for bitmapCacheOff called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.RadioButton get_bitmapCacheOff() {
      return build_bitmapCacheOff();
    }
    private com.google.gwt.user.client.ui.RadioButton build_bitmapCacheOff() {
      // Creation section.
      final com.google.gwt.user.client.ui.RadioButton bitmapCacheOff = new com.google.gwt.user.client.ui.RadioButton("rad_bmpc");
      // Setup section.
      bitmapCacheOff.setValue(false);

      this.owner.bitmapCacheOff = bitmapCacheOff;

      return bitmapCacheOff;
    }

    /**
     * Getter for domId36Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId36Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId36Element() {
      return domId36Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId36Element() {
      // Creation section.
      domId36Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId36());
      // Setup section.

      return domId36Element;
    }

    /**
     * Getter for domId37 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId37;
    private java.lang.String get_domId37() {
      return domId37;
    }
    private java.lang.String build_domId37() {
      // Creation section.
      domId37 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId37;
    }

    /**
     * Getter for developerOptionsOn called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.RadioButton get_developerOptionsOn() {
      return build_developerOptionsOn();
    }
    private com.google.gwt.user.client.ui.RadioButton build_developerOptionsOn() {
      // Creation section.
      final com.google.gwt.user.client.ui.RadioButton developerOptionsOn = new com.google.gwt.user.client.ui.RadioButton("rad_devop");
      // Setup section.
      developerOptionsOn.setValue(true);

      this.owner.developerOptionsOn = developerOptionsOn;

      return developerOptionsOn;
    }

    /**
     * Getter for domId37Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId37Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId37Element() {
      return domId37Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId37Element() {
      // Creation section.
      domId37Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId37());
      // Setup section.

      return domId37Element;
    }

    /**
     * Getter for domId38 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId38;
    private java.lang.String get_domId38() {
      return domId38;
    }
    private java.lang.String build_domId38() {
      // Creation section.
      domId38 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId38;
    }

    /**
     * Getter for developerOptionsOff called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.RadioButton get_developerOptionsOff() {
      return build_developerOptionsOff();
    }
    private com.google.gwt.user.client.ui.RadioButton build_developerOptionsOff() {
      // Creation section.
      final com.google.gwt.user.client.ui.RadioButton developerOptionsOff = new com.google.gwt.user.client.ui.RadioButton("rad_devop");
      // Setup section.
      developerOptionsOff.setValue(false);

      this.owner.developerOptionsOff = developerOptionsOff;

      return developerOptionsOff;
    }

    /**
     * Getter for domId38Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId38Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId38Element() {
      return domId38Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId38Element() {
      // Creation section.
      domId38Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId38());
      // Setup section.

      return domId38Element;
    }

    /**
     * Getter for systemInfo called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.dom.client.DivElement get_systemInfo() {
      return build_systemInfo();
    }
    private com.google.gwt.dom.client.DivElement build_systemInfo() {
      // Creation section.
      final com.google.gwt.dom.client.DivElement systemInfo = new com.google.gwt.uibinder.client.LazyDomElement(get_domId39()).get().cast();
      // Setup section.

      this.owner.systemInfo = systemInfo;

      return systemInfo;
    }

    /**
     * Getter for domId39 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId39;
    private java.lang.String get_domId39() {
      return domId39;
    }
    private java.lang.String build_domId39() {
      // Creation section.
      domId39 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId39;
    }

    /**
     * Getter for domId40 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId40;
    private java.lang.String get_domId40() {
      return domId40;
    }
    private java.lang.String build_domId40() {
      // Creation section.
      domId40 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId40;
    }

    /**
     * Getter for deviceName called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.TextArea get_deviceName() {
      return build_deviceName();
    }
    private com.google.gwt.user.client.ui.TextArea build_deviceName() {
      // Creation section.
      final com.google.gwt.user.client.ui.TextArea deviceName = (com.google.gwt.user.client.ui.TextArea) GWT.create(com.google.gwt.user.client.ui.TextArea.class);
      // Setup section.
      deviceName.setStyleName("editable w100p txt-oneline");
      deviceName.setVisibleLines(1);

      this.owner.deviceName = deviceName;

      return deviceName;
    }

    /**
     * Getter for domId40Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId40Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId40Element() {
      return domId40Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId40Element() {
      // Creation section.
      domId40Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId40());
      // Setup section.

      return domId40Element;
    }

    /**
     * Getter for domId41 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId41;
    private java.lang.String get_domId41() {
      return domId41;
    }
    private java.lang.String build_domId41() {
      // Creation section.
      domId41 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId41;
    }

    /**
     * Getter for saveDeviceName called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Button get_saveDeviceName() {
      return build_saveDeviceName();
    }
    private com.google.gwt.user.client.ui.Button build_saveDeviceName() {
      // Creation section.
      final com.google.gwt.user.client.ui.Button saveDeviceName = (com.google.gwt.user.client.ui.Button) GWT.create(com.google.gwt.user.client.ui.Button.class);
      // Setup section.
      saveDeviceName.setText("Save");

      this.owner.saveDeviceName = saveDeviceName;

      return saveDeviceName;
    }

    /**
     * Getter for domId41Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId41Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId41Element() {
      return domId41Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId41Element() {
      // Creation section.
      domId41Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId41());
      // Setup section.

      return domId41Element;
    }

    /**
     * Getter for domId42 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId42;
    private java.lang.String get_domId42() {
      return domId42;
    }
    private java.lang.String build_domId42() {
      // Creation section.
      domId42 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId42;
    }

    /**
     * Getter for freeMemory called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Label get_freeMemory() {
      return build_freeMemory();
    }
    private com.google.gwt.user.client.ui.Label build_freeMemory() {
      // Creation section.
      final com.google.gwt.user.client.ui.Label freeMemory = (com.google.gwt.user.client.ui.Label) GWT.create(com.google.gwt.user.client.ui.Label.class);
      // Setup section.

      this.owner.freeMemory = freeMemory;

      return freeMemory;
    }

    /**
     * Getter for domId42Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId42Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId42Element() {
      return domId42Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId42Element() {
      // Creation section.
      domId42Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId42());
      // Setup section.

      return domId42Element;
    }

    /**
     * Getter for domId43 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId43;
    private java.lang.String get_domId43() {
      return domId43;
    }
    private java.lang.String build_domId43() {
      // Creation section.
      domId43 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId43;
    }

    /**
     * Getter for uiVersion called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Label get_uiVersion() {
      return build_uiVersion();
    }
    private com.google.gwt.user.client.ui.Label build_uiVersion() {
      // Creation section.
      final com.google.gwt.user.client.ui.Label uiVersion = (com.google.gwt.user.client.ui.Label) GWT.create(com.google.gwt.user.client.ui.Label.class);
      // Setup section.

      this.owner.uiVersion = uiVersion;

      return uiVersion;
    }

    /**
     * Getter for domId43Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId43Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId43Element() {
      return domId43Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId43Element() {
      // Creation section.
      domId43Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId43());
      // Setup section.

      return domId43Element;
    }

    /**
     * Getter for domId44 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId44;
    private java.lang.String get_domId44() {
      return domId44;
    }
    private java.lang.String build_domId44() {
      // Creation section.
      domId44 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId44;
    }

    /**
     * Getter for rtVersion called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Label get_rtVersion() {
      return build_rtVersion();
    }
    private com.google.gwt.user.client.ui.Label build_rtVersion() {
      // Creation section.
      final com.google.gwt.user.client.ui.Label rtVersion = (com.google.gwt.user.client.ui.Label) GWT.create(com.google.gwt.user.client.ui.Label.class);
      // Setup section.

      this.owner.rtVersion = rtVersion;

      return rtVersion;
    }

    /**
     * Getter for domId44Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId44Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId44Element() {
      return domId44Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId44Element() {
      // Creation section.
      domId44Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId44());
      // Setup section.

      return domId44Element;
    }

    /**
     * Getter for domId45 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId45;
    private java.lang.String get_domId45() {
      return domId45;
    }
    private java.lang.String build_domId45() {
      // Creation section.
      domId45 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId45;
    }

    /**
     * Getter for osVersion called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Label get_osVersion() {
      return build_osVersion();
    }
    private com.google.gwt.user.client.ui.Label build_osVersion() {
      // Creation section.
      final com.google.gwt.user.client.ui.Label osVersion = (com.google.gwt.user.client.ui.Label) GWT.create(com.google.gwt.user.client.ui.Label.class);
      // Setup section.

      this.owner.osVersion = osVersion;

      return osVersion;
    }

    /**
     * Getter for domId45Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId45Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId45Element() {
      return domId45Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId45Element() {
      // Creation section.
      domId45Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId45());
      // Setup section.

      return domId45Element;
    }

    /**
     * Getter for domId46 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId46;
    private java.lang.String get_domId46() {
      return domId46;
    }
    private java.lang.String build_domId46() {
      // Creation section.
      domId46 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId46;
    }

    /**
     * Getter for dateTime called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.user.client.ui.Label get_dateTime() {
      return build_dateTime();
    }
    private com.google.gwt.user.client.ui.Label build_dateTime() {
      // Creation section.
      final com.google.gwt.user.client.ui.Label dateTime = (com.google.gwt.user.client.ui.Label) GWT.create(com.google.gwt.user.client.ui.Label.class);
      // Setup section.

      this.owner.dateTime = dateTime;

      return dateTime;
    }

    /**
     * Getter for domId46Element called 2 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.uibinder.client.LazyDomElement domId46Element;
    private com.google.gwt.uibinder.client.LazyDomElement get_domId46Element() {
      return domId46Element;
    }
    private com.google.gwt.uibinder.client.LazyDomElement build_domId46Element() {
      // Creation section.
      domId46Element = new com.google.gwt.uibinder.client.LazyDomElement<Element>(get_domId46());
      // Setup section.

      return domId46Element;
    }

    /**
     * Getter for about called 1 times. Type: DEFAULT. Build precedence: 2.
     */
    private com.google.gwt.dom.client.DivElement get_about() {
      return build_about();
    }
    private com.google.gwt.dom.client.DivElement build_about() {
      // Creation section.
      final com.google.gwt.dom.client.DivElement about = new com.google.gwt.uibinder.client.LazyDomElement(get_domId47()).get().cast();
      // Setup section.

      this.owner.about = about;

      return about;
    }

    /**
     * Getter for domId47 called 2 times. Type: DOM_ID_HOLDER. Build precedence: 2.
     */
    private java.lang.String domId47;
    private java.lang.String get_domId47() {
      return domId47;
    }
    private java.lang.String build_domId47() {
      // Creation section.
      domId47 = com.google.gwt.dom.client.Document.get().createUniqueId();
      // Setup section.

      return domId47;
    }
  }
}
