#ifndef TRANSACTIONTABLEMODEL_H
#define TRANSACTIONTABLEMODEL_H

#include <QAbstractTableModel>
#include <resttransactiondata.h>

class TransactionTableModel : public QAbstractTableModel {
    Q_OBJECT

    public:
        explicit TransactionTableModel(RestTransactionData *transactionData, QObject *parent = nullptr);
        ~TransactionTableModel();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;

    private:
        static QString getHeader(int column);
        static QString getTypeFrendlyName(QString type);
        QVariant getData(int row, int column) const;
        RestTransactionData *transactionData;
};

#endif // TRANSACTIONTABLEMODEL_H
