#pragma once

#include <presets/PresetManager.h>
#include <presets/Bank.h>

class ClusterEnforcement
{
 public:
  struct TreeNode;
  typedef std::shared_ptr<TreeNode> tTreeNodePtr;

  class TreeNode : public std::enable_shared_from_this<TreeNode>
  {
   public:
    tTreeNodePtr master = nullptr;
    tTreeNodePtr right;
    tTreeNodePtr bottom;
    tTreeNodePtr left;
    tTreeNodePtr top;
    Bank *bank;

    void assignMaster(const tTreeNodePtr &master);
    size_t getClusterDepth();
    size_t getColumn();
    size_t getRow();
  };
  typedef std::shared_ptr<TreeNode> tCluster;

 public:
  explicit ClusterEnforcement(PresetManager *pm);
  virtual ~ClusterEnforcement();
  void enforceClusterRuleOfOne(UNDO::Transaction *transaction);
  std::vector<Bank *> sortBanks();

 private:
  PresetManager *m_presetManager = nullptr;
  std::vector<tCluster> m_clusters;
  std::map<Uuid, tTreeNodePtr> m_uuidToTreeNode;

  bool applyRule(UNDO::Transaction *transaction);
  bool applyRule(UNDO::Transaction *transaction, tTreeNodePtr node);
  void appendToBottomChildOfNode(UNDO::Transaction *transaction, tTreeNodePtr node, tTreeNodePtr target);
  void buildClusterStructure();
  tTreeNodePtr findTreeNode(const Uuid &uuid);
  void addCluster(tTreeNodePtr clusterRoot);
  void connectToClusterStructure(tTreeNodePtr masterNode, tTreeNodePtr myNode);
  void ruleDelete(UNDO::Transaction *transaction, tTreeNodePtr bottom, tTreeNodePtr right, Uuid newMasterUuid,
                  Bank::AttachmentDirection dir);

  std::vector<Bank *> getClusterMasters();
};
