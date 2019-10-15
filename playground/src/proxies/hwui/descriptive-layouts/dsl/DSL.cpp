#include "DSL.h"
#include <proxies/hwui/descriptive-layouts/TemplateEnums.h>
#include <proxies/hwui/HWUIEnums.h>
#include <testing/TestDriver.h>
#include <nltools/logging/Log.h>

#include <glib.h>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_symbols.hpp>
#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <limits>

namespace DescriptiveLayouts
{
#warning "TODO: fix hard coded path"
  const auto testsDir = "/home/hhoegelo/dev/nl/C15/C15/playground/src/proxies/hwui/descriptive-layouts/dsl/tests/";
  const int maxPos = std::numeric_limits<int16_t>::max();

  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

  using ascii::char_;
  using ascii::string;
  using boost::spirit::ascii::space;
  using qi::double_;
  using qi::int_;
  using qi::lexeme;
  using qi::lit;
  using iterator_type = std::string::const_iterator;

  using Rule = qi::rule<iterator_type, std::string(), ascii::space_type>;
  using Strings = std::vector<std::string>;

  static constexpr int invalidInt = std::numeric_limits<int>::min();

  struct Element
  {
    Element(Element* parent = nullptr, const std::string& n = "root")
        : parent(parent)
        , name(n)
    {
    }

    Element* parent = nullptr;

    std::string name;
    std::string refersTo;
    Rect position{ 0, 0, maxPos, maxPos };
    EventProviders eventProvider = EventProviders::None;

    Strings fires;
    Strings reactions;
    Strings inits;
    Strings styles;
    Strings conditions;

    std::vector<Element> children;
  };

  template <typename... V> static void dump(const std::string& indent, const std::string& name, V&&... v)
  {
    nltools::Log::notify(indent, name, v...);
  }

  static void dump(Element* e, size_t indent = 0)
  {
    std::string i(indent, ' ');
    dump(i, "Element: ", e->name);
    dump(i, " pos: ", e->position.getX(), e->position.getY());
    dump(i, " size: ", e->position.getWidth(), e->position.getHeight());
    dump(i, " ref: ", e->refersTo);
    dump(i, " event provider: ", toString(e->eventProvider));

    if(!e->conditions.empty())
    {
      dump(i, " conditions: ", std::to_string(e->conditions.size()));
      for(auto c : e->conditions)
        dump(i, "   Condition: ", c);
    }

    if(!e->fires.empty())
    {
      dump(i, " fires: ", std::to_string(e->fires.size()));
      for(auto c : e->fires)
        dump(i, "   Fires: ", c);
    }

    if(!e->inits.empty())
    {
      dump(i, " inits: ", std::to_string(e->inits.size()));
      for(auto c : e->inits)
        dump(i, "   Initializes: ", c);
    }

    if(!e->styles.empty())
    {
      dump(i, " styles: ", std::to_string(e->styles.size()));
      for(auto c : e->styles)
        dump(i, "   Style: ", c);
    }

    if(!e->reactions.empty())
    {
      dump(i, " reacts: ", std::to_string(e->reactions.size()));
      for(auto c : e->reactions)
        dump(i, "   Reacts to: ", c);
    }

    if(!e->children.empty())
    {
      dump(i, " children: ", std::to_string(e->children.size()));
      for(auto& c : e->children)
        dump(&c, indent + 4);
    }
  }

  static const Element& findChild(const Element& in, const Strings& path)
  {
    if(path.empty())
      return in;

    for(auto& c : in.children)
      if(c.name == path.front())
        return findChild(c, Strings(std::next(path.begin()), path.end()));

    throw std::runtime_error("Reference not found");
  }

  static const Element& findReference(const Element& in, const Strings& path)
  {
    if(in.name == path.front())
      return findChild(in, Strings(std::next(path.begin()), path.end()));

    for(auto& c : in.children)
      if(c.name == path.front())
        return findChild(c, Strings(std::next(path.begin()), path.end()));

    if(!in.parent)
      throw std::runtime_error("Reference not found");

    return findReference(*in.parent, path);
  }

  template <typename T> static void concat(T& out, const T& in)
  {
    out.insert(out.end(), in.begin(), in.end());
  }

  static Strings toPath(const std::string& path)
  {
    Strings p;
    boost::split(p, path, boost::is_any_of("/"));
    return p;
  }

  template <typename Out> static void recurseAndProduceFlat(const Element& in, Element collector, Out out)
  {
    collector.name += "/" + in.name;

    concat(collector.conditions, in.conditions);
    concat(collector.fires, in.fires);
    concat(collector.inits, in.inits);
    concat(collector.reactions, in.reactions);
    concat(collector.styles, in.styles);

    auto movedInPos = in.position.getMovedBy(collector.position.getLeftTop());
    collector.position = collector.position.getIntersection(movedInPos);

    collector.eventProvider = in.eventProvider;
    collector.refersTo = in.refersTo;

    if(!in.refersTo.empty())
    {
      out(collector);
    }
    else
    {
      for(auto& c : in.children)
      {
        recurseAndProduceFlat(c, collector, out);
      }
    }
  }

  static void makeFlat(const Element& in, Element& out)
  {
    Element collector(nullptr, "");
    recurseAndProduceFlat(in, collector, [&](const Element& e) { out.children.push_back(e); });
  }

  static Strings addPrefix(const Strings& in, const std::string& prefix)
  {
    Strings out;
    std::transform(in.begin(), in.end(), std::back_inserter(out), [&](auto& a) { return prefix + a; });
    return out;
  }

  enum class ResolveResult
  {
    Done,
    Error,
    Again
  };

  static bool isUnresolvedReference(const Element& e)
  {
    return !e.refersTo.empty() && e.refersTo[0] != '@';
  }

  static bool containsUnresolvedReferences(const Element& e)
  {
    if(isUnresolvedReference(e))
      return true;

    for(auto& c : e.children)
      if(containsUnresolvedReferences(c))
        return true;

    return false;
  }

  static const Element& findRelativeElement(const Element& e)
  {
    return findReference(e, toPath(e.refersTo));
  }

  static bool deepMerge(Element& tgt, const Element& src)
  {
    // TODO check cross references
    // TODO check paranet/child references

    for(auto& c : src.children)
      tgt.children.push_back(c);

    concat(tgt.conditions, src.conditions);
    concat(tgt.fires, src.fires);
    concat(tgt.inits, src.inits);
    concat(tgt.reactions, src.reactions);
    concat(tgt.styles, src.styles);

    if(tgt.eventProvider == EventProviders::None)
      tgt.eventProvider = src.eventProvider;

    tgt.position = src.position;

    tgt.refersTo.clear();
    return true;
  }

  static ResolveResult resolve(Element& e)
  {
    for(auto& c : e.children)
    {
      if(isUnresolvedReference(c))
      {
        const auto& referencedElement = findRelativeElement(c);

        if(!containsUnresolvedReferences(referencedElement))
        {
          if(!deepMerge(c, referencedElement))
            return ResolveResult::Error;

          return ResolveResult::Again;
        }
      }

      ResolveResult childResult = resolve(c);
      if(childResult != ResolveResult::Done)
        return childResult;
    }
    return ResolveResult::Done;
  }

  template <typename TStyle> struct StyleRule
  {
    using Style = TStyle;
    std::string name;
  };

  static boost::spirit::qi::symbols<char, std::string> stringsToRule(const Strings& items)
  {
    return boost::spirit::qi::symbols<char, std::string>(items, items);
  }

  template <typename R> static Rule createStyleRule(const R& r)
  {
    auto theStyleRule = stringsToRule(StyleValues::getAllStrings<typename R::Style>());
    return string(r.name) >> string("=") >> std::move(theStyleRule);
  }

  template <typename... E> static Rule createEnumStylesRule(E... e)
  {
    return (createStyleRule(e) | ...);
  }

  static Element parseString(const std::string& content)
  {
    Element root(nullptr, "root");
    Element* current = &root;
    std::stack<Element*> stack;
    stack.push(&root);

    auto onRef = [&](auto s) { current->refersTo = s; };
    auto onIf = [&](auto s) { current->conditions.push_back(s); };
    auto onX = [&](int x) { current->position.setLeft(x); };
    auto onY = [&](int y) { current->position.setTop(y); };
    auto onWidth = [&](int w) { current->position.setWidth(w); };
    auto onHeight = [&](int h) { current->position.setHeight(h); };
    auto onInit = [&](auto s) { current->inits.push_back(s); };
    auto onStyle = [&](auto s) { current->styles.push_back(s); };
    auto onFire = [&](auto s) { current->fires.push_back(s); };
    auto onReaction = [&](auto s) { current->reactions.push_back(s); };
    auto onUses = [&](auto s) { current->eventProvider = to<EventProviders>(s); };
    auto diveIn = [&](auto) { stack.push(current); };
    auto diveOut = [&](auto) {
      stack.pop();
      current = stack.top();
    };
    auto newElement = [&](auto s) { current = &(stack.top()->children.emplace_back(stack.top(), s)); };

    Rule element;

    Rule delim = string("/");
    Rule comma = string(",");
    Rule point = '(' >> qi::int_[onX] >> comma >> qi::int_[onY] >> ')';
    Rule rect = '(' >> qi::int_[onX] >> comma >> qi::int_[onY] >> comma >> qi::int_[onWidth] >> comma
        >> qi::int_[onHeight] >> ')';
    Rule identifier = lexeme[qi::alpha >> +qi::alnum];

    auto primitiveProperty = stringsToRule(DescriptiveLayouts::getAllStrings<PrimitiveProperty>());
    auto uiFocusOptions = stringsToRule(::getAllStrings<UIFocus>());
    auto primitives = stringsToRule(addPrefix(DescriptiveLayouts::getAllStrings<PrimitiveClasses>(), "@"));
    auto eventSourceName = stringsToRule(DescriptiveLayouts::getAllStrings<EventSources>());
    auto eventSinkName = stringsToRule(DescriptiveLayouts::getAllStrings<EventSinks>());
    auto buttonName = stringsToRule(::getAllStrings<Buttons>());

    Rule uiFocusCondition = string("UIFocus") >> string("=") >> uiFocusOptions;

    Rule children = lit("{")[diveIn] >> *element >> lit('}')[diveOut];
    Rule condition = uiFocusCondition;
    Rule referencedElement;
    referencedElement = identifier >> -(delim >> referencedElement);
    Rule reference = referencedElement | primitives;

    Rule primitiveValue = lexeme['"' >> +(char_ - '"') >> '"'];

    Rule reaction = eventSourceName >> string(":") >> identifier >> string("[") >> primitiveProperty >> string("]");
    Rule init = (identifier >> string("[") >> primitiveProperty >> string("]") >> string("=") >> primitiveValue)
        | (primitiveProperty >> string("=") >> primitiveValue);
    Rule fire = buttonName >> string("=") >> eventSinkName;

    Rule enumStyleRules = createEnumStylesRule(
        StyleRule<StyleValues::Color>{ "Color" }, StyleRule<StyleValues::Color>{ "BackgroundColor" },
        StyleRule<StyleValues::Alignment>{ "TextAlign" }, StyleRule<StyleValues::BorderStyle>{ "BorderStyle" },
        StyleRule<StyleValues::Font>{ "FontDecoration" }, StyleRule<StyleValues::Color>{ "SuffixColor" },
        StyleRule<StyleValues::Color>{ "BorderColor" }, StyleRule<StyleValues::Color>{ "HighlightBackgroundColor" },
        StyleRule<StyleValues::Color>{ "HighlightColor" });

    Rule fontSizeRule = string("FontSize") >> string("=") >> qi::int_;
    Rule style = enumStyleRules | fontSizeRule;

    Rule ofRule = lit("of") >> (reference[onRef] | '(' >> reference[onRef] >> ')');
    Rule ifRule = lit("if") >> (condition[onIf] | '(' >> +condition[onIf] >> ')');
    Rule atRule = lit("at") >> (point | rect);
    Rule usesRule = lit("uses") >> (identifier[onUses] | '(' >> identifier[onUses] >> ')');
    Rule reactRule = lit("react") >> (reaction[onReaction] | '(' >> +reaction[onReaction] >> ')');
    Rule initRule = lit("init") >> (init[onInit] | '(' >> +init[onInit] >> ')');
    Rule styleRule = lit("style") >> (style[onStyle] | '(' >> +style[onStyle] >> ')');
    Rule fireRule = lit("fire") >> (fire[onFire] | '(' >> +fire[onFire] >> ')');
    Rule rule = ofRule | atRule | ifRule | usesRule | reactRule | initRule | fireRule | styleRule;

    element = identifier[newElement] >> *rule >> *children;

    qi::rule<iterator_type, ascii::space_type> ui = +element;
    qi::grammar<iterator_type, ascii::space_type> g(ui);

    std::string::const_iterator iter = content.begin();
    std::string::const_iterator end = content.end();

    if(phrase_parse(iter, end, g, boost::spirit::ascii::space) && iter == end)
      return root;
    else
      nltools::Log::warning("String contains crap:", std::string(iter, end));

    throw std::runtime_error("Could not parse file completely.");
  }

  static Element parseFile(const std::string& fileName)
  {
    std::ifstream in(fileName);
    std::string content((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));
    return parseString(content);
  }

  void DSL::registerTests()
  {
    g_test_add_func("/DescriptiveLayouts/DSL/simplestElement", [] {
      auto e = parseString("TheElement");
      g_assert(e.children.size() == 1);
      g_assert(e.children[0].name == "TheElement");
    });

    g_test_add_func("/DescriptiveLayouts/DSL/simplestElement/resolve", [] {
      Element e = parseString("TheElement");
      g_assert(resolve(e) == ResolveResult::Done);
      Element flat;
      makeFlat(e, flat);
      g_assert(e.children.size() == 1);
      g_assert(flat.children.size() == 0);
    });

    g_test_add_func("/DescriptiveLayouts/DSL/simpleElement/eventProvider/global", [] {
      auto e = parseString("TheElement uses Global");
      g_assert(e.children[0].eventProvider == EventProviders::Global);
    });

    g_test_add_func("/DescriptiveLayouts/DSL/simpleElement/position/point", [] {
      auto e = parseString("TheElement at (12, 34)");
      g_assert(e.children[0].position == Rect(12, 34, maxPos, maxPos));
    });

    g_test_add_func("/DescriptiveLayouts/DSL/simpleElement/position/rect", [] {
      auto e = parseString("TheElement at (12, 34, 56, 78)");
      g_assert(e.children[0].position == Rect(12, 34, 56, 78));
    });

    g_test_add_func("/DescriptiveLayouts/DSL/nestedElement/position-moved", [] {
      Element flat;
      Element e = parseString("TheElement at (12, 34, 500, 600) { NestedElement of @Bar at (100, 100, 20, 30) }");
      resolve(e);
      makeFlat(e, flat);
      g_assert(flat.children.size() == 1);
      g_assert(flat.children[0].position == Rect(112, 134, 20, 30));
      g_assert(flat.children[0].name == "/root/TheElement/NestedElement");
    });

    g_test_add_func("/DescriptiveLayouts/DSL/nestedElement/position-clamped-1", [] {
      Element flat;
      Element e = parseString("TheElement at (10, 10) { NestedElement of @Bar at (20, 20, 100, 100) }");
      resolve(e);
      makeFlat(e, flat);
      g_assert(flat.children.size() == 1);
      g_assert(flat.children[0].position == Rect(30, 30, 100, 100));
      g_assert(flat.children[0].name == "/root/TheElement/NestedElement");
    });

    g_test_add_func("/DescriptiveLayouts/DSL/nestedElement/position-clamped-2", [] {
      Element flat;
      Element e = parseString("TheElement at (10, 10, 50, 50) { NestedElement of @Bar at (20, 20, 100, 100) }");
      resolve(e);
      makeFlat(e, flat);
      g_assert(flat.children.size() == 1);
      g_assert(flat.children[0].position == Rect(30, 30, 30, 30));
      g_assert(flat.children[0].name == "/root/TheElement/NestedElement");
    });

    g_test_add_func("/DescriptiveLayouts/DSL/styles/Color", [] {
      Element flat;
      Element e = parseString("TheElement of @Bar style (Color=C43) }");
      resolve(e);
      makeFlat(e, flat);
      g_assert(flat.children.size() == 1);
      g_assert(flat.children[0].styles.size() == 1);
      g_assert(flat.children[0].styles[0] == "Color=C43");
    });
  }

  static TestDriver<DSL> tests;
}
