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

namespace DescriptiveLayouts
{

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

  struct Element
  {
    Element(Element* parent, const std::string& n)
        : parent(parent)
        , name(n)
    {
    }

    Element* parent = nullptr;

    std::string name;
    std::string refersTo;
    std::string position;
    std::string eventProvider;

    Strings fires;
    Strings reactions;
    Strings inits;
    Strings styles;
    Strings conditions;

    std::vector<Element> children;
  };

  void dump(const std::string& indent, const std::string& name, const std::string& v)
  {
    if(!v.empty())
      nltools::Log::notify(indent, name, v);
  }

  void dump(Element* e, size_t indent = 0)
  {
    std::string i(indent, ' ');
    dump(i, "Element: ", e->name);
    dump(i, " pos: ", e->position);
    dump(i, " ref: ", e->refersTo);
    dump(i, " event provider: ", e->eventProvider);

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

  decltype(auto) stringsToRule(const Strings& items)
  {
    return boost::spirit::qi::symbols<char, std::string>(items, items);
  }

  const Element& findChild(const Element& in, const Strings& path)
  {
    if(path.empty())
      return in;

    for(auto& c : in.children)
      if(c.name == path.front())
        return findChild(c, Strings(std::next(path.begin()), path.end()));

    throw std::runtime_error("Reference not found");
  }

  const Element& findReference(const Element& in, const Strings& path)
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

  template <typename T> void concat(T& out, const T& in)
  {
    out.insert(out.end(), in.begin(), in.end());
  }

  void resolveReferences(const Element& in, Element& out);

  void mergeProperties(const Element& in, Element& out)
  {
    for(auto& c : in.children)
      resolveReferences(c, out.children.emplace_back(&out, c.name));

    concat(out.conditions, in.conditions);
    concat(out.fires, in.fires);
    concat(out.inits, in.inits);
    concat(out.reactions, in.reactions);
    concat(out.styles, in.styles);

    out.eventProvider = in.eventProvider;
    out.position = in.position;
    out.refersTo = in.refersTo;
  }

  Strings toPath(const std::string& path)
  {
    Strings p;
    boost::split(p, path, boost::is_any_of("/"));
    return p;
  }

  void resolveReferences(const Element& in, Element& out)
  {
    bool resolvableReference = !in.refersTo.empty() && in.refersTo[0] != '@';

    if(resolvableReference)
      mergeProperties(findReference(in, toPath(in.refersTo)), out);

    mergeProperties(in, out);

    if(resolvableReference)
      out.refersTo.clear();
  }

  template <typename Out> void recurseAndProduceFlat(const Element& in, Element collector, Out out)
  {
    collector.name += "/" + in.name;

    concat(collector.conditions, in.conditions);
    concat(collector.fires, in.fires);
    concat(collector.inits, in.inits);
    concat(collector.reactions, in.reactions);
    concat(collector.styles, in.styles);
    collector.eventProvider = in.eventProvider;
    collector.position = in.position;
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

  void makeFlat(const Element& in, Element& out)
  {
    Element collector(nullptr, "");
    recurseAndProduceFlat(in, collector, [&](const Element& e) { out.children.push_back(e); });
  }

  Strings addPrefix(const Strings& in, const std::string& prefix)
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

  bool isUnresolvedReference(const Element& e)
  {
    return !e.refersTo.empty() && e.refersTo[0] != '@';
  }

  bool containsUnresolvedReferences(const Element& e)
  {
    if(isUnresolvedReference(e))
      return true;

    for(auto& c : e.children)
      if(containsUnresolvedReferences(c))
        return true;

    return false;
  }

  const Element& findRelativeElement(const Element& e)
  {
    return findReference(e, toPath(e.refersTo));
  }

  bool deepMerge(Element& tgt, const Element& src)
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

    tgt.eventProvider = src.eventProvider;
    tgt.position = src.position;
    tgt.refersTo.clear();

    return true;
  }

  ResolveResult resolve(Element& e)
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

  void testSpirit()
  {
    Element root(nullptr, "root");
    Element* current = &root;
    std::stack<Element*> stack;
    stack.push(&root);

    std::ifstream in("/home/hhoegelo/dev/my/scratchbook/src/testdata.nl");
    std::string content((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));

    auto onRef = [&](auto s) { current->refersTo = s; };
    auto onIf = [&](auto s) { current->conditions.push_back(s); };
    auto onPoint = [&](auto s) { current->position = s; };
    auto onRect = [&](auto s) { current->position = s; };
    auto onInit = [&](auto s) { current->inits.push_back(s); };
    auto onStyle = [&](auto s) { current->styles.push_back(s); };
    auto onFire = [&](auto s) { current->fires.push_back(s); };
    auto onReaction = [&](auto s) { current->reactions.push_back(s); };
    auto onUses = [&](auto s) { current->eventProvider = s; };
    auto diveIn = [&](auto) { stack.push(current); };
    auto diveOut = [&](auto) {
      stack.pop();
      current = stack.top();
    };
    auto newElement = [&](auto s) { current = &(stack.top()->children.emplace_back(stack.top(), s)); };

    Rule element;

    Rule delim = string("/");
    Rule number = -(lit('+') | lit('-')) >> *qi::digit;
    Rule comma = string(",");
    Rule point = '(' >> number >> comma >> number >> ')';
    Rule rect = '(' >> number >> comma >> number >> comma >> number >> comma >> number >> ')';
    Rule identifier = lexeme[qi::alpha >> +qi::alnum];

    auto primitiveProperty = stringsToRule(DescriptiveLayouts::getAllPrimitivePropertyStrings());
    auto uiFocusOptions = stringsToRule(getAllUIFocusStrings());
    auto primitives = stringsToRule(addPrefix(DescriptiveLayouts::getAllPrimitiveClassesStrings(), "@"));
    auto eventSourceName = stringsToRule(DescriptiveLayouts::getAllEventSourcesStrings());
    auto eventSinkName = stringsToRule(DescriptiveLayouts::getAllEventSinksStrings());
    auto buttonName = stringsToRule(getAllButtonsStrings());

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

    // TODO
    Rule style = identifier >> string("=") >> identifier;

    Rule ofRule = lit("of") >> (reference[onRef] | '(' >> reference[onRef] >> ')');
    Rule ifRule = lit("if") >> (condition[onIf] | '(' >> +condition[onIf] >> ')');
    Rule atRule = lit("at") >> (point[onPoint] | rect[onRect]);
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
    {
      dump(&root);

      while(resolve(root) == ResolveResult::Again)
        ;

      nltools::Log::notify("RESOLVED:");
      dump(&root);

      Element flat(nullptr, "flat");
      makeFlat(root, flat);

      nltools::Log::notify("Flat:");
      dump(&flat);
    }
    else
    {
      nltools::Log::warning("String contains crap:", std::string(iter, end));
      g_warn_if_reached();
    }
  }

  void DSL::registerTests()
  {
    g_test_add_func("/DescriptiveLayouts/DSL/a", [] { testSpirit(); });
  }

  static TestDriver<DSL> tests;
}
