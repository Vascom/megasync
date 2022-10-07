#ifndef MEGAITEM_H
#define MEGAITEM_H

#include "QTMegaRequestListener.h"

#include <QList>
#include <QIcon>

#include <memory>

namespace UserAttributes{
class FullName;
class Avatar;
}

class MegaItem : public QObject
{
    Q_OBJECT
public:
    static const int ICON_SIZE;

    enum STATUS{
        SYNC,
        SYNC_PARENT,
        SYNC_CHILD,
        BACKUP,
        NONE,
    };

    explicit MegaItem(std::unique_ptr<mega::MegaNode> node, bool showFiles, MegaItem *parentItem = 0);

    std::shared_ptr<mega::MegaNode> getNode();
    void fetchChildren();
    void createChildItems();
    MegaItem *getParent();
    MegaItem *getChild(int i);
    int getNumItemChildren();
    int getNumChildren();
    int indexOf(MegaItem *item);
    QString getOwnerName();
    QString getOwnerEmail();
    void setOwner(std::unique_ptr<mega::MegaUser> user);
    QPixmap getOwnerIcon();
    QIcon getStatusIcons();
    QIcon getFolderIcon();
    int getStatus();
    bool isSyncable();
    bool isRoot();
    void addNode(std::unique_ptr<mega::MegaNode> node);
    void removeNode(std::shared_ptr<mega::MegaNode> node);
    void displayFiles(bool enable);
    void setCameraFolder();
    void setChatFilesFolder();
    int row();

    ~MegaItem();

signals:
    void infoUpdated(int role);

protected:
    QString mOwnerEmail;
    int mStatus;
    bool mCameraFolder;
    bool mChatFilesFolder;
    bool mChildrenSet;

    std::shared_ptr<mega::MegaNode> mNode;
    QList<MegaItem*> mChildItems;
    std::unique_ptr<mega::MegaNodeList> mChildNodes;
    std::unique_ptr<mega::MegaUser> mOwner;

private slots:
    void onFullNameAttributeReady();
    void onAvatarAttributeReady();

private:
    void calculateSyncStatus(const QStringList& folders);
    std::unique_ptr<mega::QTMegaRequestListener> mDelegateListener;
    std::shared_ptr<const UserAttributes::FullName> mFullNameAttribute;
    std::shared_ptr<const UserAttributes::Avatar> mAvatarAttribute;
    bool mShowFiles;

};

#endif // MEGAITEM_H
