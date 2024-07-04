#include "viewmessage.h"

ViewMessage::ViewMessage(QObject *parent)
    : QAbstractItemModel(parent)
{}

QVariant ViewMessage::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex ViewMessage::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex ViewMessage::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int ViewMessage::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int ViewMessage::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant ViewMessage::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
