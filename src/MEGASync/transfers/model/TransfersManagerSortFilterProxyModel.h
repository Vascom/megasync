#ifndef TRANSFERSSORTFILTERPROXYMODEL_H
#define TRANSFERSSORTFILTERPROXYMODEL_H

#include "TransferItem.h"
#include "TransfersSortFilterProxyBaseModel.h"

#include <QSortFilterProxyModel>
#include <QReadWriteLock>
#include <QFutureWatcher>
#include <QMutex>

class TransferBaseDelegateWidget;
class TransfersModel;

class TransfersManagerSortFilterProxyModel : public TransfersSortFilterProxyBaseModel
{
        Q_OBJECT

public:
        TransfersManagerSortFilterProxyModel(QObject *parent = nullptr);
        ~TransfersManagerSortFilterProxyModel();

        void initProxyModel(SortCriterion sortCriterion, Qt::SortOrder order);

        bool moveRows(const QModelIndex& proxyParent, int proxyRow, int count,
                      const QModelIndex& destinationParent, int destinationChild) override;
        void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
        void setSourceModel(QAbstractItemModel *sourceModel) override;
        void setFilterFixedString(const QString &pattern);
        void refreshFilterFixedString();
        void textSearchTypeChanged();
        void setFilters(const TransferData::TransferTypes transferTypes,
                        const TransferData::TransferStates transferStates,
                        const Utilities::FileTypes fileTypes);
        void updateFilters();
        void resetAllFilters();
        int  getNumberOfItems(TransferData::TransferType transferType);

        TransferBaseDelegateWidget* createTransferManagerItem(QWidget *) override;

        int  getPausedTransfers() const;
        bool areAllPaused() const;
        bool isAnyActive() const;
        bool areAllActive() const;
        bool areAllSync() const;
        bool areAllCompleted() const;
        bool isAnyCompleted() const;

        int  activeTransfers() const;

        bool isModelProcessing() const;
signals:
        void modelAboutToBeChanged();
        void modelChanged();
        void searchNumbersChanged();
        void pauseResumeTransfer(bool);
        void transferRetry();
        void transferCancelClear();

protected slots:
        void onCancelClearTransfer();
        void onPauseResumeTransfer();
        void onRetryTransfer();
        void onOpenTransfer();

protected:
        bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
        bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
        QMimeData* mimeData(const QModelIndexList &indexes) const override;

protected:
        TransferData::TransferStates mTransferStates;
        TransferData::TransferTypes mTransferTypes;
        Utilities::FileTypes mFileTypes;
        TransferData::TransferStates mNextTransferStates;
        TransferData::TransferTypes mNextTransferTypes;
        Utilities::FileTypes mNextFileTypes;
        SortCriterion mSortCriterion;
        Qt::SortOrder mSortOrder;

        mutable QSet<int> mDlNumber;
        mutable QSet<int> mUlNumber;
        mutable QSet<int> mNoSyncTransfers;
        mutable QSet<int> mActiveTransfers;
        mutable QSet<int> mPausedTransfers;
        mutable QSet<int> mCompletedTransfers;

private slots:
        void onRowsAboutToBeRemoved(const QModelIndex& parent, int first, int last);
        void onModelSortedFiltered();

private:
        ThreadPool* mThreadPool;
        mutable QMutex mMutex;
        QFutureWatcher<void> mFilterWatcher;
        QString mFilterText;
        bool mIsFiltering;

        void removeActiveTransferFromCounter(TransferTag tag) const;
        void removePausedTransferFromCounter(TransferTag tag) const;
        void removeNonSyncedTransferFromCounter(TransferTag tag) const;
        void removeCompletedTransferFromCounter(TransferTag tag) const;
        bool updateTransfersCounterFromTag(QExplicitlySharedDataPointer<TransferData> transfer) const;

        void invalidateModel();

        void resetAllCounters();
        void resetTransfersStateCounters();
};

#endif // TRANSFERSSORTFILTERPROXYMODEL_H
