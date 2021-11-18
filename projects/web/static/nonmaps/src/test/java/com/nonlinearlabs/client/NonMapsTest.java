package com.nonlinearlabs.client;

import com.google.gwt.junit.client.GWTTestCase;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterFactory;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;

/**
 * GWT JUnit tests must extend GWTTestCase.
 */
public class NonMapsTest extends GWTTestCase {

  /**
   * Must refer to a valid module that sources this class.
   */
  public String getModuleName() {
    return "com.nonlinearlabs.NonMapsJUnit";
  }

  private void assertRouterMapsToPhysical(int routerId, int physicalId) {
    assertEquals(ParameterFactory.getPhysicalControlForRouter(new ParameterId(routerId, VoiceGroup.Global)),
        new ParameterId(physicalId, VoiceGroup.Global));
  }

  public void testParameterFactory() {
    assertRouterMapsToPhysical(255, 254);
    assertRouterMapsToPhysical(256, 254);
    assertRouterMapsToPhysical(257, 254);
    assertRouterMapsToPhysical(258, 254);
    assertRouterMapsToPhysical(373, 254);
    assertRouterMapsToPhysical(374, 254);

    assertRouterMapsToPhysical(260, 259);
    assertRouterMapsToPhysical(261, 259);
    assertRouterMapsToPhysical(262, 259);
    assertRouterMapsToPhysical(263, 259);
    assertRouterMapsToPhysical(375, 259);
    assertRouterMapsToPhysical(376, 259);

    assertRouterMapsToPhysical(265, 264);
    assertRouterMapsToPhysical(266, 264);
    assertRouterMapsToPhysical(267, 264);
    assertRouterMapsToPhysical(268, 264);
    assertRouterMapsToPhysical(377, 264);
    assertRouterMapsToPhysical(378, 264);

    assertRouterMapsToPhysical(270, 269);
    assertRouterMapsToPhysical(271, 269);
    assertRouterMapsToPhysical(272, 269);
    assertRouterMapsToPhysical(273, 269);
    assertRouterMapsToPhysical(379, 269);
    assertRouterMapsToPhysical(380, 269);

    assertRouterMapsToPhysical(275, 274);
    assertRouterMapsToPhysical(276, 274);
    assertRouterMapsToPhysical(277, 274);
    assertRouterMapsToPhysical(278, 274);
    assertRouterMapsToPhysical(381, 274);
    assertRouterMapsToPhysical(382, 274);

    assertRouterMapsToPhysical(280, 279);
    assertRouterMapsToPhysical(281, 279);
    assertRouterMapsToPhysical(282, 279);
    assertRouterMapsToPhysical(283, 279);
    assertRouterMapsToPhysical(383, 279);
    assertRouterMapsToPhysical(384, 279);

    assertRouterMapsToPhysical(285, 284);
    assertRouterMapsToPhysical(286, 284);
    assertRouterMapsToPhysical(287, 284);
    assertRouterMapsToPhysical(288, 284);
    assertRouterMapsToPhysical(385, 284);
    assertRouterMapsToPhysical(386, 284);

    assertRouterMapsToPhysical(290, 289);
    assertRouterMapsToPhysical(291, 289);
    assertRouterMapsToPhysical(292, 289);
    assertRouterMapsToPhysical(293, 289);
    assertRouterMapsToPhysical(387, 289);
    assertRouterMapsToPhysical(388, 289);
  }

}
