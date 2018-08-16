#pragma once
#include "PresetManager.h"
#include "PresetBank.h"
#include <Application.h>

class ClusterEnforcement
{
  public:
    typedef shared_ptr<PresetBank> tBankPtr;
    struct TreeNode;
    typedef std::shared_ptr<TreeNode> tTreeNodePtr;
    struct TreeNode
    {
        tTreeNodePtr master = nullptr;
        tTreeNodePtr right;
        tTreeNodePtr bottom;
        tTreeNodePtr left;
        tTreeNodePtr top;
        tBankPtr bank;

        void assignMaster(const tTreeNodePtr& master) {
            this->master = master;

            if(right)
                right->assignMaster(master);
            if(bottom)
                bottom->assignMaster(master);
        }

        size_t getClusterDepth() {
            if(left)
                return 1 + left->getClusterDepth();
            if(top)
                return 1 + top->getClusterDepth();

            return 0;
        }
    };
    typedef std::shared_ptr<TreeNode> tCluster;
  public:
    ClusterEnforcement();
    virtual ~ClusterEnforcement();
    void enforceClusterRuleOfOne(UNDO::Scope::tTransactionPtr transaction);
    void sanitizeBankThatWillBeDeleted(UNDO::Scope::tTransactionPtr transaction, tBankPtr bank);
    vector<shared_ptr<PresetBank>> sortBanks();

    static void sortBankNumbers();
private:
    std::shared_ptr<PresetManager> m_presetManager;
    std::vector<tCluster> m_clusters;
    std::map<Glib::ustring, tTreeNodePtr> m_uuidToTreeNode;

    bool applyRule(UNDO::Scope::tTransactionPtr transaction);
    bool applyRule(UNDO::Scope::tTransactionPtr transaction, tTreeNodePtr node);
    void appendToBottomChildOfNode(UNDO::Scope::tTransactionPtr transaction, tTreeNodePtr node, tTreeNodePtr target);
    void buildClusterStructure();
    tTreeNodePtr findTreeNode(Glib::ustring uuid);
    void addCluster(tTreeNodePtr clusterRoot);
    void connectToClusterStructure(tTreeNodePtr masterNode, tTreeNodePtr myNode);
    void ruleDelete(UNDO::Scope::tTransactionPtr transaction, tTreeNodePtr bottom, tTreeNodePtr right, Glib::ustring newMasterUuid,
                    PresetBank::AttachmentDirection dir);


    std::vector<tBankPtr> getClusterMasters();
};

