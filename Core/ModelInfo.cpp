#include <ModelInfo.h>

ModelInfo::ModelInfo() :
        Model<DBTableInfo>(),
        Singleton<ModelInfo>(*this) {
};

ModelInfo::~ModelInfo() {

};

/*
* Initialize the global ModelInfo table.
* Reset the ModelInfo table or create the table if it does not exist.
*/
ModelInfo &ModelInfo::instanceDB(QSqlDatabase *db) {
    ModelInfo &ins = instance();
    ins.m_db = db;
    ins.ensure(db);

    return ins;
}

ModelInfo::DataSet ModelInfo::allRecords(const QDate &date) {
    return find(DBTableInfo::USERQUERYDATE(date.toString("yyyy-MM-dd")));
}

ModelInfo::Data *ModelInfo::getRecord(const int queryId) {
    Data *hist = getCachedOnly(USERQUERYID(queryId));
    if (hist)
        return hist;

    auto items = find(USERQUERYID(queryId));
    if (!items.empty())
        hist = getWithCache(items[0].id(), m_db);

    return hist;
}

ModelInfo::Data *ModelInfo::getRecord(const QString &queryText) {
    Data *hist = getCachedOnly(USERQUERYTEXT(queryText));
    if (hist)
        return hist;

    auto items = find(USERQUERYTEXT(queryText));
    if (!items.empty())
        hist = getWithCache(items[0].id(), m_db);

    return hist;
}

int ModelInfo::addUpdate(const int queryId, const QString &queryText, const QDate date, const QString response) {
    Data *record = getRecord(queryId);
    if (!record)
        record = create();

    record->USERQUERYTEXT = queryText;
    record->USERQUERYDATE = date.toString("yyyy-MM-dd");
    record->RESPONSE = response;
    return save(record);
}