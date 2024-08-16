#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractListModel>
#include <qnamespace.h>
#include "apidata.h"

class FileModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum FileRoles {
        ShaRole = Qt::UserRole + 1,
        FilenameRole,
        StatusRole,
        AdditionsRole,
        DeletionsRole,
        ChangesRole,
        PatchRole
    };

    explicit FileModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void loadData(QByteArray data);
    File *get(int index) const;

private:
    std::vector<File*> files;
};

#endif
