#ifndef NODESELECTOR_H
#define NODESELECTOR_H

#include "ButtonIconManager.h"
#include "NodeNameSetterDialog/NewFolderDialog.h"
#include "NodeSelectorTreeViewWidget.h"
#include "megaapi.h"

#include <QDialog>
#include <QItemSelection>
#include <QTimer>

#include <memory>


class NodeSelectorProxyModel;
class NodeSelectorModel;
class NodeSelectorTreeViewWidgetCloudDrive;
class NodeSelectorTreeViewWidgetIncomingShares;
class NodeSelectorTreeViewWidgetBackups;
class NodeSelectorTreeViewWidgetSearch;

namespace Ui {
class NodeSelector;
}

class NodeSelector : public QDialog
{
    Q_OBJECT

public:
    enum TabItem{
        CLOUD_DRIVE = 0,
        SHARES,
        VAULT,
    };

    static const int LABEL_ELIDE_MARGIN;
    explicit NodeSelector(QWidget *parent = 0);

    ~NodeSelector();
    void showDefaultUploadOption(bool show = true);
    void setDefaultUploadOption(bool value);
    bool getDefaultUploadOption();
    void setSelectedNodeHandle(std::shared_ptr<mega::MegaNode> node = nullptr);
    mega::MegaHandle getSelectedNodeHandle();
    QList<mega::MegaHandle> getMultiSelectionNodeHandle();
    void closeEvent(QCloseEvent* event) override;

protected:
    void changeEvent(QEvent * event) override;
    void showEvent(QShowEvent* event) override;
    void keyPressEvent(QKeyEvent *e) override;
    void addBackupsView();
    void hideSelector(TabItem item);
    bool nodeExistWarningMsg(int &access);
    void makeConnections(SelectTypeSPtr selectType);

    NodeSelectorTreeViewWidgetCloudDrive* mCloudDriveWidget;
    NodeSelectorTreeViewWidgetIncomingShares* mIncomingSharesWidget;
    NodeSelectorTreeViewWidgetBackups* mBackupsWidget;
    NodeSelectorTreeViewWidgetSearch* mSearchWidget;
    mega::MegaApi* mMegaApi;
    Ui::NodeSelector *ui;

private slots:
    void onbOkClicked();
    void onViewReady(bool isEmpty);
    void onbShowIncomingSharesClicked();
    void onbShowCloudDriveClicked();
    void onbShowBackupsFolderClicked();
    void onOptionSelected(int index);
    void updateNodeSelectorTabs();
    void onSearch(const QString& text);

#ifdef Q_OS_MAC
    void onTabSelected(int index);
#endif

private:
    void processCloseEvent(NodeSelectorProxyModel *proxy, QCloseEvent* event);
    QModelIndex getParentIncomingShareByIndex(QModelIndex idx);
    virtual bool isSelectionCorrect() = 0;
#ifdef Q_OS_MAC
    void hideTabSelector(const QString& tabText);
#endif
    void shortCutConnects(int ignoreThis);
    ButtonIconManager mButtonIconManager;

    bool mManuallyResizedColumn;
};

class UploadNodeSelector : public NodeSelector
{
public:
    explicit UploadNodeSelector(QWidget *parent = 0);

private:
    bool isSelectionCorrect() override;
};

class DownloadNodeSelector : public NodeSelector
{
public:
    explicit DownloadNodeSelector(QWidget *parent = 0);

private:
    bool isSelectionCorrect() override;
};

class SyncNodeSelector : public NodeSelector
{
public:
    explicit SyncNodeSelector(QWidget *parent = 0);

private:
    bool isSelectionCorrect() override;
};

class StreamNodeSelector : public NodeSelector
{
public:
    explicit StreamNodeSelector(QWidget *parent = 0);

private:
    bool isSelectionCorrect() override;
};

#endif // NODESELECTOR_H
