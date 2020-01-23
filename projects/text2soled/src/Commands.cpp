#include <Commands.h>
#include <Command.h>
#include <algorithm>
#include <FrameBuffer.h>
#include <Font.h>

extern char *selfPath;

namespace
{
  class ClearCommand : public Command
  {
    public:
      virtual int getNumArguments () const override
      {
        return 0;
      }

      virtual void execute (char **argv) override
      {
        FrameBuffer::get ().clear ();
      }

      virtual std::string getHelp () const override
      {
        return "clear => entirely clears the framebuffer";
      }

      virtual std::string getName () const override
      {
        return "clear";
      }
  };

  class TextCommand : public Command
  {
    public:
      virtual int getNumArguments () const override
      {
        return 3;
      }

      virtual void execute (char **argv) override
      {
        auto app = Gio::File::create_for_path (selfPath);
        auto parent = app->get_parent ();
        auto parentPath = parent->get_path ();
        auto resourcePath = parentPath + "/resources/";

        Font f (resourcePath + "Emphase_8_Regular.ttf", 8);
        FrameBuffer::get ().setColor (FrameBuffer::C255);
        f.draw (argv[0], atoi (argv[1]), atoi (argv[2]));
        g_usleep (10000);
      }

      virtual std::string getHelp () const override
      {
        return "'foo bar' x y => writes 'foo bar' at position x/y";
      }

      virtual std::string getName () const override
      {
        return "text";
      }
  };

  class HorizontalLineCommand : public Command
  {
    public:
      virtual int getNumArguments () const override
      {
        return 3;
      }

      virtual void execute (char **argv) override
      {
        FrameBuffer::get ().drawHorizontalLine (atoi (argv[0]), atoi (argv[1]), atoi (argv[2]));
      }

      virtual std::string getHelp () const override
      {
        return "x y l => draws a horizontal line of 'l' pixels length at x/y";
      }

      virtual std::string getName () const override
      {
        return "horizontal-line";
      }
  };

  class VerticalLineCommand : public Command
  {
    public:
      virtual int getNumArguments () const override
      {
        return 3;
      }

      virtual void execute (char **argv) override
      {
        FrameBuffer::get ().drawVerticalLine (atoi (argv[0]), atoi (argv[1]), atoi (argv[2]));
      }

      virtual std::string getHelp () const override
      {
        return "x y l => draws a vertical line of 'l' pixels length at x/y";
      }

      virtual std::string getName () const override
      {
        return "vertical-line";
      }
  };
}

Commands::Commands ()
{
  m_commands.push_back (new ClearCommand ());
  m_commands.push_back (new TextCommand ());
  m_commands.push_back (new HorizontalLineCommand ());
  m_commands.push_back (new VerticalLineCommand ());
}

Commands::~Commands ()
{
}

void Commands::executeCommand (Command* c, int numArgs, char** argv)
{
  numArgs--;
  if (numArgs == c->getNumArguments ())
    c->execute (argv + 1);
  else
    showHelp ();
}

void Commands::execute (int numArgs, char **argv)
{
  try
  {
    if (numArgs > 0)
    {
      if (auto c = findCommand (*argv))
      {
        executeCommand (c, numArgs, argv);
      }
      else
      {
        ClearCommand clear;
        clear.execute(nullptr);

        TextCommand text;
        text.execute(argv);
      }
    }
  }
  catch (...)
  {
    showHelp ();
  }
}

Command *Commands::findCommand (const std::string &name) const
{
  auto c = std::find_if (m_commands.begin (), m_commands.end (), [&](Command *c)
  {
    return c->getName() == name;
  });

  if(c != m_commands.end())
    return *c;

  return nullptr;
}

void Commands::showHelp ()
{
  for (auto c : m_commands)
  {
    std::cout << c->getName () << std::endl;
    std::cout << '\t' << c->getHelp () << std::endl << std::endl;
  }
}

