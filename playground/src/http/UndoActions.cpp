#include "UndoActions.h"
#include "libundo/undo/Scope.h"
#include <libundo/undo/Algorithm.h>
#include <http/NetworkRequest.h>

UndoActions::UndoActions(UNDO::Scope &scope)
    : super("/undo/")
{
  addAction("undo", [&](std::shared_ptr<NetworkRequest> request) { scope.undo(); });

  addAction("undo-jump", [&](std::shared_ptr<NetworkRequest> request) mutable {
    Glib::ustring target = request->get("target");
    scope.undoJump(target);
  });

  addAction("redo", [&](std::shared_ptr<NetworkRequest> request) {
    int way = std::stoi(request->get("way", "-1"));
    scope.redo(way);
  });

  addAction("erase-branch", [&](std::shared_ptr<NetworkRequest> request) mutable {
    Glib::ustring id = request->get("id");
    scope.eraseBranch(id);
  });

  addAction("rebase", [&](std::shared_ptr<NetworkRequest> request) mutable {
    Glib::ustring id = request->get("id");

    if(auto newBase = UNDO::Algorithm::find(scope.getRootTransaction(), id))
    {
      scope.rebase(newBase);
    }
  });
}

UndoActions::~UndoActions()
{
}
