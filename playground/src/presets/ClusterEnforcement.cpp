#include <presets/ClusterEnforcement.h>
#include <tools/PerformanceTimer.h>

ClusterEnforcement::ClusterEnforcement()
{
  m_presetManager = Application::get().getPresetManager();
}

ClusterEnforcement::~ClusterEnforcement()
{

}


std::vector<ClusterEnforcement::tBankPtr> ClusterEnforcement::getClusterMasters()
{
  auto presetManager = Application::get().getPresetManager();
  auto allBanks = presetManager->getBanks();
  std::vector<tBankPtr> clusterMaster;
  for (auto bank : allBanks)
  {
    if (bank->getAttached().uuid == "")
      clusterMaster.push_back(bank);
  }
  return clusterMaster;
}

void ClusterEnforcement::buildClusterStructure()
{
  m_clusters.clear();
  m_uuidToTreeNode.clear();

  auto waitingList = m_presetManager->getBanks();

  std::vector<tBankPtr> anotherWaitingList;

  while (true)
  {
    anotherWaitingList.clear();

    for (auto bank : waitingList)
    {
      if (bank->getAttached().uuid == "")
      {
        auto clusterRoot = std::make_shared<TreeNode>();
        clusterRoot->bank = bank;
        clusterRoot->master = nullptr;
        addCluster(clusterRoot);
      }
      else if (auto masterNode = findTreeNode(bank->getAttached().uuid))
      {
        auto myNode = std::make_shared<TreeNode>();
        myNode->bank = bank;
        connectToClusterStructure(masterNode, myNode);
      }
      else
      {
        anotherWaitingList.push_back(bank);
      }
    }

    if (waitingList.size() == anotherWaitingList.size())
    {
      for(const auto& master: m_clusters) {
        master->assignMaster(master);
      }
      break;
    }

    waitingList = anotherWaitingList;
  }
}

void ClusterEnforcement::enforceClusterRuleOfOne(UNDO::Scope::tTransactionPtr transaction)
{
  do
  {
    buildClusterStructure();
  }
  while (applyRule(transaction));
}

void ClusterEnforcement::sanitizeBankThatWillBeDeleted(UNDO::Scope::tTransactionPtr transaction, ClusterEnforcement::tBankPtr bank)
{
  buildClusterStructure();

  if (auto deletedTreeNode = findTreeNode(bank->getUuid()))
  {
    auto left = deletedTreeNode->left;
    auto right = deletedTreeNode->right;
    auto top = deletedTreeNode->top;
    auto bottom = deletedTreeNode->bottom;

    constexpr auto topDir = PresetBank::AttachmentDirection::top;
    constexpr auto leftDir = PresetBank::AttachmentDirection::left;
    constexpr auto noneDir = PresetBank::AttachmentDirection::none;

    if (top)
    {
      ruleDelete(transaction, bottom, right, top->bank->getUuid(), topDir);
    }
    else if (left)
    {
      ruleDelete(transaction, bottom, right, left->bank->getUuid(), leftDir);
    }
    else
    {
      ruleDelete(transaction, bottom, right, "", noneDir);

      if (bottom)
      {
        bottom->bank->undoableSetPosition(transaction, bank->getX(), bank->getY());
      }
      else if (right)
      {
        right->bank->undoableSetPosition(transaction, bank->getX(), bank->getY());
      }
    }
  }
}

void ClusterEnforcement::ruleDelete(UNDO::Scope::tTransactionPtr transaction, ClusterEnforcement::tTreeNodePtr bottom,
                                    ClusterEnforcement::tTreeNodePtr right,
                                    Glib::ustring newMasterUuid,
                                    PresetBank::AttachmentDirection dir)
{
  if (bottom)
  {
    bottom->bank->undoableAttachBank(transaction, newMasterUuid, dir);
    if (right)
    {
      right->bank->undoableAttachBank(transaction, bottom->bank->getUuid(), PresetBank::AttachmentDirection::left);
    }
  }
  else if (right)
  {
    right->bank->undoableAttachBank(transaction, newMasterUuid, dir);
  }
}

bool ClusterEnforcement::applyRule(UNDO::Scope::tTransactionPtr transaction)
{
  for (auto clusterMaster : m_clusters)
  {
    if (applyRule(transaction, clusterMaster))
    {
      return true;
    }
  }
  return false;
}

bool ClusterEnforcement::applyRule(UNDO::Scope::tTransactionPtr transaction, tTreeNodePtr node)
{
  tTreeNodePtr mostTopNode = nullptr;

  while (node != nullptr)
  {
    if (node->right != nullptr)
    {
      if (mostTopNode != nullptr)
      {
        appendToBottomChildOfNode(transaction, node->right, mostTopNode);
        return true;
      }
      else
      {
        mostTopNode = node->right;
        applyRule(transaction, mostTopNode);
      }
    }
    node = node->bottom;
  }
  return false;
}

void ClusterEnforcement::appendToBottomChildOfNode(UNDO::Scope::tTransactionPtr transaction, tTreeNodePtr node, tTreeNodePtr target)
{
  while (target->bottom != nullptr)
  {
    target = target->bottom;
  }

  node->bank->undoableAttachBank(transaction, target->bank->getUuid(), PresetBank::AttachmentDirection::top);
}

ClusterEnforcement::tTreeNodePtr ClusterEnforcement::findTreeNode(Glib::ustring uuid)
{
  auto it = m_uuidToTreeNode.find(uuid);

  if (it != m_uuidToTreeNode.end())
    return it->second;
  else
    return nullptr;
}

void ClusterEnforcement::addCluster(tTreeNodePtr clusterRoot)
{
  m_uuidToTreeNode[clusterRoot->bank->getUuid()] = clusterRoot;
  m_clusters.push_back(clusterRoot);
}

void ClusterEnforcement::connectToClusterStructure(tTreeNodePtr masterNode, tTreeNodePtr myNode)
{
  switch (myNode->bank->getAttached().direction)
  {
  case PresetBank::AttachmentDirection::left:
    myNode->left = masterNode;
    masterNode->right = myNode;

    break;
  case PresetBank::AttachmentDirection::top:
    myNode->top = masterNode;
    masterNode->bottom = myNode;
  }

  m_uuidToTreeNode[myNode->bank->getUuid()] = myNode;
}

bool inCluster(const ClusterEnforcement::tTreeNodePtr& node) {
  return node->top || node->bottom || node->left || node->right;
}

vector<ClusterEnforcement::tTreeNodePtr> prepareNodeVector(const std::map<Glib::ustring, ClusterEnforcement::tTreeNodePtr>& nodeMap) {
    vector<ClusterEnforcement::tTreeNodePtr> ret;
    for(auto x: nodeMap)
        ret.push_back(x.second);
    return ret;
}

bool handleBothBanksInCluster(const ClusterEnforcement::tTreeNodePtr& lhs,
                              const ClusterEnforcement::tTreeNodePtr& rhs) {
    if(lhs->master == rhs->master) {
      auto lhsCol = lhs->getColumn();
      auto rhsCol = rhs->getColumn();
      auto lhsRow = lhs->getRow();
      auto rhsRow = rhs->getRow();

      if(lhsCol == rhsCol)
        return lhsRow < rhsRow;

      return lhsCol < rhsCol;
    } else {
      return stoi(lhs->master->bank->getX()) < stoi(rhs->master->bank->getX());
    }
}

vector<shared_ptr<PresetBank>> buildVectorFromNodeVector(const vector<ClusterEnforcement::tTreeNodePtr>& nodeVec) {
    auto ret = vector<shared_ptr<PresetBank>>();
    for(const auto& x: nodeVec)
        ret.push_back(x->bank);
    return ret;
}

void ClusterEnforcement::sortBankNumbers() {
  PerformanceTimer t("Sort Banks");

  auto scope = Application::get().getPresetManager()->getUndoScope().startTransaction("Sort Bank Numbers");
  auto transaction = scope->getTransaction();

  ClusterEnforcement ce;
  auto newBanks = ce.sortBanks();
  Application::get().getPresetManager()->undoableSetBanks(transaction, newBanks);
}

vector<shared_ptr<PresetBank>> ClusterEnforcement::sortBanks() {

  buildClusterStructure();
  auto treeNodes = prepareNodeVector(m_uuidToTreeNode);

  std::sort(treeNodes.begin(), treeNodes.end(), [&](const tTreeNodePtr& lhs, const tTreeNodePtr& rhs) {
      auto lhsInCluster = inCluster(lhs);
      auto rhsInCluster = inCluster(rhs);

      if(lhsInCluster && !rhsInCluster)
        return true;
      else if(!lhsInCluster && rhsInCluster)
        return false;
      else if(lhsInCluster && rhsInCluster)
          return handleBothBanksInCluster(lhs, rhs);
      else
        return stoi(lhs->bank->getX()) < stoi(rhs->bank->getX());
  });

  return buildVectorFromNodeVector(treeNodes);

}
