#include "transactiontablemodel.h"

TransactionTableModel::TransactionTableModel(RestTransactionData *transactionData, QObject *parent) :
    QAbstractTableModel{parent},
    transactionData(transactionData)
{}

TransactionTableModel::~TransactionTableModel() {
    delete this->transactionData;
}

int TransactionTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return this->transactionData->getCount();
}

int TransactionTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 5;
}

QVariant TransactionTableModel::data(const QModelIndex &index, int role) const {
    switch(role) {
        case Qt::DisplayRole:
            return this->getData(index.row(), index.column());
        case Qt::TextAlignmentRole:
            return Qt::AlignCenter;
    }

    return QVariant();
}

QVariant TransactionTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    Q_UNUSED(role);
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return QVariant(this->getHeader(section));
    }

    return QVariant();
}

Qt::ItemFlags TransactionTableModel::flags(const QModelIndex &index) const {
    Q_UNUSED(index);
    return Qt::ItemIsEnabled;
}

QString TransactionTableModel::getHeader(int column) {
    switch (column) {
        case 0:
            return QStringLiteral("Aika");
        case 1:
            return QStringLiteral("Tyyppi");
        case 2:
            return QStringLiteral("Kortti");
        case 3:
            return QStringLiteral("Tili");
        case 4:
            return QStringLiteral("Summa");
    }

    return QStringLiteral("UNKNOWN");
}

QString TransactionTableModel::getTypeFrendlyName(QString type) {
    if(type == "WITHDRAW") {
        return QStringLiteral("Nosto");
    } else if(type == "CREDIT_WITHDRAW") {
        return QStringLiteral("Lainan Nosto");
    } else if(type == "DEPOSIT") {
        return QStringLiteral("Talletus");
    }

    return QStringLiteral("UNKNOWN");
}

QVariant TransactionTableModel::getData(int row, int column) const {
    BankTransaction *transaction = this->transactionData->getTransaction(row);

    switch (column) {
        case 0:
            return QVariant(transaction->getTimestamp());
        case 1:
            return QVariant(this->getTypeFrendlyName(transaction->getType()));
        case 2:
            return QVariant(transaction->getCardNumber());
        case 3:
            return QVariant(transaction->getAccount());
        case 4:
            return QVariant(QString::number(transaction->getSum(), 'f', 2));
    }

    return QVariant();
}
