#ifndef ModelInfo_H
#define ModelInfo_H

#include <Model.h>
#include <DBTableInfo.h>

#include <QDate>

class ModelInfo : public Model<DBTableInfo>, public Singleton<ModelInfo> {
public:
    using Model<DBTableInfo>::getWithCache;

public:
    ModelInfo();

    ~ModelInfo();

public:
    /*
    * Initialize the global ModelInfo table on initial call.
    * Resets the global table on subsequent calls.
    * Return the static instance address for ModelInfo table
    * Note: Assigning the address to a local variable can destroy the instance.
    */
    static ModelInfo &instanceDB(QSqlDatabase *db);

public:
    /*
     * Return records for specified date
     */
    DataSet allRecords(const QDate &date);

    /*
     * Return record for specified id
     */
    Data *getRecord(const int queryId);

    /*
     * Return record for query text
     */
    Data *getRecord(const QString &queryText);

    /*
     * Adds or updates an element
     */
    int addUpdate(const int queryId,
                  const QString &queryText,
                  const QDate date = QDate::currentDate(),
                  const QString response = QString());
};

#endif // 
