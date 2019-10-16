#include "DSL.h"
#include <proxies/hwui/descriptive-layouts/TemplateEnums.h>
#include <proxies/hwui/HWUIEnums.h>
#include <testing/TestDriver.h>
#include <nltools/logging/Log.h>
#include "Parser.h"
#include "Algorithm.h"

#include <glib.h>

#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <limits>
#include <variant>

namespace DescriptiveLayouts
{
  static Element parseFile(const std::string& fileName)
  {
    std::ifstream in(fileName);
    std::string content((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));
    return Parser::parseString(content);
  }

  void DSL::registerTests()
  {
    g_test_add_func("/DescriptiveLayouts/DSL/simplestElement", [] {
      auto e = Parser::parseString("TheElement");
      g_assert(e.children.size() == 1);
      g_assert(e.children[0].name == "TheElement");
    });

    g_test_add_func("/DescriptiveLayouts/DSL/simplestElement/resolve", [] {
      Element e = Parser::parseString("TheElement");
      Element flat = flatten(e);
      g_assert(e.children.size() == 1);
      g_assert(flat.children.size() == 0);
    });

    g_test_add_func("/DescriptiveLayouts/DSL/simpleElement/eventProvider/global", [] {
      auto e = Parser::parseString("TheElement uses Global");
      g_assert(e.children[0].eventProvider == EventProviders::Global);
    });

    g_test_add_func("/DescriptiveLayouts/DSL/simpleElement/position/point", [] {
      auto e = Parser::parseString("TheElement at (12, 34)");
      g_assert(e.children[0].position == Rect(12, 34, Element::maxPos, Element::maxPos));
    });

    g_test_add_func("/DescriptiveLayouts/DSL/simpleElement/position/rect", [] {
      auto e = Parser::parseString("TheElement at (12, 34, 56, 78)");
      g_assert(e.children[0].position == Rect(12, 34, 56, 78));
    });

    g_test_add_func("/DescriptiveLayouts/DSL/nestedElement/position-moved", [] {
      Element flat = flatten(
          Parser::parseString("TheElement at (12, 34, 500, 600) { NestedElement of @Bar at (100, 100, 20, 30) }"));
      g_assert(flat.children.size() == 1);
      g_assert(flat.children[0].position == Rect(112, 134, 20, 30));
      g_assert(flat.children[0].name == "/root/TheElement/NestedElement");
    });

    g_test_add_func("/DescriptiveLayouts/DSL/nestedElement/position-clamped-1", [] {
      Element flat
          = flatten(Parser::parseString("TheElement at (10, 10) { NestedElement of @Bar at (20, 20, 100, 100) }"));
      g_assert(flat.children.size() == 1);
      g_assert(flat.children[0].position == Rect(30, 30, 100, 100));
      g_assert(flat.children[0].name == "/root/TheElement/NestedElement");
    });

    g_test_add_func("/DescriptiveLayouts/DSL/nestedElement/position-clamped-2", [] {
      Element flat = flatten(
          Parser::parseString("TheElement at (10, 10, 50, 50) { NestedElement of @Bar at (20, 20, 100, 100) }"));
      g_assert(flat.children.size() == 1);
      g_assert(flat.children[0].position == Rect(30, 30, 30, 30));
      g_assert(flat.children[0].name == "/root/TheElement/NestedElement");
    });

    g_test_add_func("/DescriptiveLayouts/DSL/styles/Color", [] {
      Element flat = flatten(Parser::parseString("TheElement of @Bar style (Color=C103)"));
      g_assert(flat.children.size() == 1);
      g_assert(std::get<0>(flat.children[0].style) == StyleValues::Color::C103);
    });

    g_test_add_func("/DescriptiveLayouts/DSL/nested-primitives", [] {
      Element flat = flatten(Parser::parseString("TheElement of @Bar { NestedElement of @Text }"));
      g_assert(flat.children.size() == 2);
    });

    g_test_add_func("/DescriptiveLayouts/DSL/styles/Color-nested", [] {
      Element flat = flatten(Parser::parseString("TheElement of @Bar style (Color=C103) { NestedElement of @Text }"));
      g_assert(flat.children.size() == 2);
      g_assert(std::get<0>(flat.children[0].style) == StyleValues::Color::C103);
      g_assert(std::get<0>(flat.children[1].style) == StyleValues::Color::C103);
    });

    g_test_add_func("/DescriptiveLayouts/DSL/styles/Color-nested-overriding", [] {
      Element flat = flatten(
          Parser::parseString("TheElement of @Bar style (Color=C103) { NestedElement of @Text style (Color=C128)}"));
      g_assert(flat.children.size() == 2);
      g_assert(std::get<0>(flat.children[0].style) == StyleValues::Color::C103);
      g_assert(std::get<0>(flat.children[1].style) == StyleValues::Color::C128);
    });

    g_test_add_func("/DescriptiveLayouts/DSL/styles/multiple", [] {
      Element flat = flatten(Parser::parseString(
          "TheElement of @Bar style (Color=C103 BackgroundColor=C128) { NestedElement of @Text style (Font=Bold)}"));
      g_assert(flat.children.size() == 2);
      g_assert(std::get<0>(flat.children[0].style) == StyleValues::Color::C103);
      g_assert(std::get<1>(flat.children[0].style) == StyleValues::Color::C128);
      g_assert(std::get<8>(flat.children[1].style) == StyleValues::Font::Bold);
    });

    g_test_add_func("/DescriptiveLayouts/DSL/styles/fontsize", [] {
      Element flat = flatten(Parser::parseString("TheElement of @Bar style (FontSize=12)"));
      g_assert(flat.children.size() == 1);
      g_assert(std::get<9>(flat.children[0].style) == 12);
    });

    g_test_add_func("/DescriptiveLayouts/DSL/if/1", [] {
      Element flat = flatten(Parser::parseString(
          "TheElement of @Bar if (UIFocus=Sound and UIFocus=Sound or (UIFocus=Sound and UIFocus=Sound))"));
      g_assert(flat.children.size() == 1);
    });
  }

  // static TestDriver<DSL> tests;
}
