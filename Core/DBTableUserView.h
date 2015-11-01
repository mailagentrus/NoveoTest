#ifndef DBTableUserView_H
#define DBTableUserView_H

#include <DBTableAbstract.h>

struct DBTableUserView : public DBTableAbstract {
    DBTableAbstract_SelDefine(DBTableUserView);

    struct ITEMID : public DB_Column<int> {
        static QString name() { return "ITEMID"; }

        explicit ITEMID(const int &v, OP op = EQUAL) : DB_Column<int>(v, op) { }
    };

    struct ARTISTNAME : public DB_Column<QString> {
        static QString name() { return "ARTISTNAME"; }

        explicit ARTISTNAME(const QString &v, OP op = EQUAL) : DB_Column<QString>(v, op) { }
    };

    struct COLLECTIONNAME : public DB_Column<QString> {
        static QString name() { return "COLLECTIONNAME"; }

        explicit COLLECTIONNAME(const QString &v, OP op = EQUAL) : DB_Column<QString>(v, op) { }
    };

    struct PIXMAPURL : public DB_Column<QString> {
        static QString name() { return "PIXMAPURL"; }

        explicit PIXMAPURL(const QString &v, OP op = EQUAL) : DB_Column<QString>(v, op) { }
    };

    struct TRACKID : public DB_Column<quint64> {
        static QString name() { return "TRACKID"; }

        explicit TRACKID(const int &v, OP op = EQUAL) : DB_Column<quint64>(v, op) { }
    };

    typedef ITEMID PRIMARY;
    enum COLUMN {
        COL_ITEMID = 0, COL_ARTISTNAME = 1, COL_COLLECTIONNAME = 2, COL_PIXMAPURL = 3, COL_TRACKID = 4
    };

    /*
     * Returns the column name as a string
     */
    static QString column_to_name(COLUMN col) {
        switch (col) {
            case COL_ITEMID:
                return "ITEMID";
            case COL_ARTISTNAME:
                return "ARTISTNAME";
            case COL_COLLECTIONNAME:
                return "COLLECTIONNAME";
            case COL_PIXMAPURL:
                return "PIXMAPURL";
            case COL_TRACKID:
                return "TRACKID";
            default:
                break;
        }

        return "UNKNOWN";
    }

    /*
     * Returns the column number from the given column name
     */
    static COLUMN name_to_column(const QString &name) {
        if ("ITEMID" == name) return COL_ITEMID;
        else if ("ARTISTNAME" == name) return COL_ARTISTNAME;
        else if ("COLLECTIONNAME" == name) return COL_COLLECTIONNAME;
        else if ("PIXMAPURL" == name) return COL_PIXMAPURL;
        else if ("TRACKID" == name) return COL_TRACKID;

        return COLUMN(-1);
    }

    /*
     * Data is a single record in the database table
     */
    struct Data {
        friend struct DBTableUserView;
        /** This is a instance pointer to itself in memory. */
        Self *m_view;

        //  primary key
        int ITEMID;

        QString ARTISTNAME;
        QString COLLECTIONNAME;
        QString PIXMAPURL;
        quint64 TRACKID;

        int id() const { return ITEMID; }

        void id(int id) { ITEMID = id; }

        bool operator<(const Data &r) const {
            return this->id() < r.id();
        }

        bool operator<(const Data *r) const {
            return this->id() < r->id();
        }

        explicit Data(Self *view = 0) {
            m_view = view;
            ITEMID = -1;
            TRACKID = -1;
        }

        explicit Data(QSqlQuery &q, Self *view = 0) {
            m_view = view;

            ITEMID = q.value(0).toInt(); // ITEMID
            ARTISTNAME = q.value(1).toString(); // ARTISTNAME
            COLLECTIONNAME = q.value(2).toString(); // COLLECTIONNAME
            PIXMAPURL = q.value(3).toString(); // PIXMAPURL
            TRACKID = q.value(4).toULongLong(); // TRACKID
        }

        Data &operator=(const Data &other) {
            if (this == &other) return *this;

            ITEMID = other.ITEMID;
            ARTISTNAME = other.ARTISTNAME;
            COLLECTIONNAME = other.COLLECTIONNAME;
            PIXMAPURL = other.PIXMAPURL;
            TRACKID = other.TRACKID;
            return *this;
        }

        template<typename C>
        bool match(const C &c) const {
            return false;
        }

        bool match(const Self::ITEMID &in) const {
            return this->ITEMID == in.v_;
        }

        bool match(const Self::ARTISTNAME &in) const {
            bool res = this->ARTISTNAME.toUpper() == in.v_.toUpper();
            return res;
        }

        bool match(const Self::COLLECTIONNAME &in) const {
            bool res = this->COLLECTIONNAME.toUpper() == in.v_.toUpper();
            return res;
        }

        bool match(const Self::PIXMAPURL &in) const {
            bool res = this->PIXMAPURL.toUpper() == in.v_.toUpper();
            return res;
        }

        bool match(const Self::TRACKID &in) const {
            return this->TRACKID == in.v_;
        }

        /*
         * Save the record instance in memory to the database
         */
        bool save(QSqlDatabase *db) {
            if (!db)
                return false;

            if (!db->isOpen())
                return false;

            if (!m_view)
                return false;

            return m_view->save(this, db);
        }

        /*
         * Remove the record instance from memory and the database
         */
        bool remove(QSqlDatabase *db) {
            if (!db->isOpen())
                return false;

            if (!m_view)
                return false;

            return m_view->remove(this, db);
        }

        void destroy() {
            delete this;
        }
    };

    enum {
        NUM_COLUMNS = 4
    };

    size_t numColumns() const { return NUM_COLUMNS; }

    /*
     * Name of the table
     */
    QString name() const Q_DECL_OVERRIDE { return "USERVIEW"; }

    QString query() const Q_DECL_OVERRIDE {
        return "SELECT * FROM USERVIEW ";
    }

    QString ensureQuery() const Q_DECL_OVERRIDE {
        return "CREATE TABLE USERVIEW(ITEMID INTEGER PRIMARY KEY, ARTISTNAME TEXT, COLLECTIONNAME TEXT, PIXMAPURL TEXT, TRACKID INTEGER NOT NULL)";
    }

    QString ensureIndexQuery() const Q_DECL_OVERRIDE {
        return "CREATE INDEX IF NOT EXISTS IDX_USERVIEW_ITEMID ON USERVIEW(ITEMID)";
    }

    explicit DBTableUserView() {

    }

    /*
     * Create a new Data record and add to memory table (cache)
     */
    Self::Data *create() {
        Self::Data *entity = new Self::Data(this);
        m_cache.push_back(entity);
        return entity;
    }

    /*
     * Create a copy of the Data record and add to memory table (cache)
     */
    Self::Data *clone(const Data *e) {
        Self::Data *entity = create();
        *entity = *e;
        entity->id(-1);
        return entity;
    }

    /*
    * Saves the Data record to the database table.
    * Either create a new record or update the existing record.
    * Remove old record from the memory table (cache)
    */
    bool save(Self::Data *entity, QSqlDatabase *db) {
        if (!entity || !db)
            return false;

        db->transaction();

        QString sql;
        if (entity->id() <= 0) //  new & insert
        {
            sql = "INSERT INTO USERVIEW(ARTISTNAME, COLLECTIONNAME, PIXMAPURL, TRACKID) VALUES(?, ?, ?, ?)";
        }
        else {
            sql = "UPDATE USERVIEW SET ARTISTNAME = ?, COLLECTIONNAME = ?, PIXMAPURL = ?, TRACKID = ? WHERE ITEMID = ?";
        }

        QSqlQuery stmt(*db);
        stmt.prepare(sql);

        stmt.bindValue(0, entity->ARTISTNAME);
        stmt.bindValue(1, entity->COLLECTIONNAME);
        stmt.bindValue(2, entity->PIXMAPURL);
        stmt.bindValue(3, entity->TRACKID);
        if (entity->id() > 0)
            stmt.bindValue(4, entity->ITEMID);

        bool sql_ok = stmt.exec();

        if (!sql_ok) {
            db->rollback();
            return false;
        }

        db->commit();

        if (entity->id() > 0) // existent
        {
            for (Cache::iterator it = m_cache.begin(); it != m_cache.end(); ++it) {
                Self::Data *e = *it;
                if (e->id() == entity->id())
                    *e = *entity;  // in-place update
            }
        }

        if (entity->id() <= 0) {
            entity->id(stmt.lastInsertId().toInt());
            m_indexById.insert(entity->id(), entity);
        }
        return true;
    }

    /*
     * Remove the Data record from the database and the memory table (cache)
     */
    bool remove(int id, QSqlDatabase *db) {
        if (id <= 0 || !db)
            return false;

        db->transaction();

        QString sql = "DELETE FROM USERVIEW WHERE ITEMID = ?";

        QSqlQuery stmt(*db);
        stmt.prepare(sql);
        stmt.bindValue(0, id);

        bool sql_ok = stmt.exec();

        if (!sql_ok) {
            db->rollback();
            return false;
        }

        db->commit();

        Cache c;
        for (Cache::iterator it = m_cache.begin(); it != m_cache.end(); ++it) {
            Self::Data *entity = *it;
            if (entity->id() == id) {
                m_indexById.remove(entity->id());
                delete entity;
            }
            else {
                c.push_back(entity);
            }
        }
        m_cache.clear();
        m_cache.swap(c);

        return true;
    }

    /*
     * Remove the Data record from the database and the memory table (cache)
     */
    bool remove(Self::Data *entity, QSqlDatabase *db) {
        if (remove(entity->id(), db)) {
            entity->id(-1);
            return true;
        }

        return false;
    }

    template<typename... Args>
    Self::Data *getCachedOnly(const Args &... args) {
        for (IndexById::iterator it = m_indexById.begin(); it != m_indexById.end(); ++it) {
            Self::Data *item = it.value();
            if (item->id() > 0 && match(item, args...)) {
                ++m_hit;
                return item;
            }
        }

        ++m_miss;

        return Q_NULLPTR;
    }

    /*
    * Search the memory table (Cache) for the data record.
    * If not found in memory, search the database and update the cache.
    */
    Self::Data *getWithCache(int id, QSqlDatabase *db) {
        if (id <= 0) {
            ++m_skip;
            return Q_NULLPTR;
        }

        auto it = m_indexById.find(id);
        if (it != m_indexById.end()) {
            ++m_hit;
            return it.value();
        }

        ++m_miss;
        Self::Data *entity = Q_NULLPTR;
        QString where = QString(" WHERE %1 = ?").arg(PRIMARY::name());

        QSqlQuery stmt(*db);
        stmt.prepare(this->query() + where);
        stmt.bindValue(0, id);
        stmt.exec();

        if (stmt.next()) {
            entity = new Self::Data(stmt, this);
            m_cache.push_back(entity);
            m_indexById.insert(id, entity);
        }

        if (!entity) {
            qDebug() << QString("%1: %2 not found").arg(this->name()).arg(id);
        }

        return entity;
    }

    /*
    * Return a list of Data records (DataSet) derived directly from the database.
    * The DataSet is sorted based on the column number.
    */
    const DataSet all(QSqlDatabase *db, COLUMN col = COLUMN(0), bool asc = true) {
        DataSet result;

        QSqlQuery stmt(*db);
        stmt.prepare(col == COLUMN(0) ? this->query() : this->query() + " ORDER BY " + column_to_name(col) +
                                                        " COLLATE NOCASE " + (asc ? " ASC " : " DESC "));
        stmt.exec();

        while (stmt.next()) {
            Self::Data entity(stmt, this);
            result.push_back(entity);
        }

        return result;
    }
};

#endif //
