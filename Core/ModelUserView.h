#ifndef ModelUserView_H
#define ModelUserView_H

#include <Model.h>
#include <DBTableUserView.h>

#include <QDate>

class ModelUserView : public Model<DBTableUserView>, public Singleton<ModelUserView> {
public:
    using Model<DBTableUserView>::getWithCache;

public:
    ModelUserView();

    ~ModelUserView();

public:
    /*
    * Initialize the global ModelInfo table on initial call.
    * Resets the global table on subsequent calls.
    * Return the static instance address for ModelUserView table
    * Note: Assigning the address to a local variable can destroy the instance.
    */
    static ModelUserView &instanceDB(QSqlDatabase *db);

public:
    void clear();

    /*
     * Return record for specified id
     */
    Data *getRecord(const int itemId);

    /*
     * Adds or updates an element
     */
    int addUpdate(const int itemId,
                  const QString &artistName,
                  const QString &collectionName,
                  const QString &pixmapURL,
                  const quint64 trackId);
};

#endif // 
