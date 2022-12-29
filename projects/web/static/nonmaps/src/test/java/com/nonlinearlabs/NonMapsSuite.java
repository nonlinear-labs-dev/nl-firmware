package com.nonlinearlabs;

import com.google.gwt.junit.tools.GWTTestSuite;
import com.nonlinearlabs.client.NonMapsTest;

import junit.framework.Test;
import junit.framework.TestSuite;

public class NonMapsSuite extends GWTTestSuite {
  public static Test suite() {
    TestSuite suite = new TestSuite("Tests for NonMaps");
    suite.addTestSuite(NonMapsTest.class);
    return suite;
  }

}
