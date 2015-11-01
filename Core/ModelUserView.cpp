#include <ModelUserView.h>

ModelUserView::ModelUserView()
        : Model<DBTableUserView>(),
          Singleton<ModelUserView>(*this) {
};

ModelUserView::~ModelUserView() {

};

/*
* Initialize the global ModelUserView table.
* Reset the ModelUserView table or create the table if it does not exist.
*/
ModelUserView &ModelUserView::instanceDB(QSqlDatabase *db) {
    ModelUserView &ins = instance();
    ins.m_db = db;
    ins.ensure(db);

    return ins;
}

void ModelUserView::clear() {
    const ModelUserView::DataSet records = all();
    if (records.isEmpty())
        return;

    for (const auto &it : records)
        remove(it.id());
}

ModelUserView::Data *ModelUserView::getRecord(const int itemId) {
    Data *hist = getCachedOnly(ITEMID(itemId));
    if (hist)
        return hist;

    auto items = find(ITEMID(itemId));
    if (!items.empty())
        hist = getWithCache(items[0].id(), m_db);

    return hist;
}

int ModelUserView::addUpdate(const int itemId,
                             const QString &artistName,
                             const QString &collectionName,
                             const QString &pixmapURL,
                             const quint64 trackId) {
    Data *record = getRecord(itemId);
    if (!record)
        record = create();

    record->ARTISTNAME = artistName;
    record->COLLECTIONNAME = collectionName;
    record->PIXMAPURL = pixmapURL;
    record->TRACKID = trackId;

    return save(record);
}