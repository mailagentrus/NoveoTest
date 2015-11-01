#ifndef DBTableInfo_H
#define DBTableInfo_H

#include <DBTableAbstract.h>

struct DBTableInfo : public DBTableAbstract {
    DBTableAbstract_SelDefine(DBTableInfo);

    struct USERQUERYID : public DB_Column<int> {
        static QString name() { return "USERQUERYID"; }

        explicit USERQUERYID(const int &v, OP op = EQUAL) : DB_Column<int>(v, op) { }
    };

    struct USERQUERYTEXT : public DB_Column<QString> {
        static QString name() { return "USERQUERYTEXT"; }

        explicit USERQUERYTEXT(const QString &v, OP op = EQUAL) : DB_Column<QString>(v, op) { }
    };

    struct USERQUERYDATE : public DB_Column<QString> {
        static QString name() { return "USERQUERYDATE"; }

        explicit USERQUERYDATE(const QString &v, OP op = EQUAL) : DB_Column<QString>(v, op) { }
    };

    struct RESPONSE : public DB_Column<QString> {
        static QString name() { return "RESPONSE"; }

        explicit RESPONSE(const QString &v, OP op = EQUAL) : DB_Column<QString>(v, op) { }
    };

    typedef USERQUERYID PRIMARY;
    enum COLUMN {
        COL_USERQUERYID = 0, COL_USERQUERYTEXT = 1, COL_USERQUERYDATE = 2, COL_RESPONSE = 3
    };

    /*
     * Returns the column name as a string
     */
    static QString column_to_name(COLUMN col) {
        switch (col) {
            case COL_USERQUERYID:
                return "USERQUERYID";
            case COL_USERQUERYTEXT:
                return "USERQUERYTEXT";
            case COL_USERQUERYDATE:
                return "USERQUERYDATE";
            case COL_RESPONSE:
                return "RESPONSE";
            default:
                break;
        }

        return "UNKNOWN";
    }

    /*
     * Returns the column number from the given column name
     */
    static COLUMN name_to_column(const QString &name) {
        if ("USERQUERYID" == name) return COL_USERQUERYID;
        else if ("USERQUERYTEXT" == name) return COL_USERQUERYTEXT;
        else if ("USERQUERYDATE" == name) return COL_USERQUERYDATE;
        else if ("RESPONSE" == name) return COL_RESPONSE;

        return COLUMN(-1);
    }

    /*
     * Data is a single record in the database table
     */
    struct Data {
        friend struct DBTableInfo;
        /** This is a instance pointer to itself in memory. */
        Self *m_view;

        //  primary key
        int USERQUERYID;

        QString USERQUERYTEXT;
        QString USERQUERYDATE;
        QString RESPONSE;

        int id() const { return USERQUERYID; }

        void id(int id) { USERQUERYID = id; }

        bool operator<(const Data &r) const {
            return this->id() < r.id();
        }

        bool operator<(const Data *r) const {
            return this->id() < r->id();
        }

        explicit Data(Self *view = 0) {
            m_view = view;
            USERQUERYID = -1;
            USERQUERYDATE = QDate::currentDate().toString("yyyy-MM-dd");
        }

        explicit Data(QSqlQuery &q, Self *view = 0) {
            m_view = view;

            USERQUERYID = q.value(0).toInt(); // USERQUERYID
            USERQUERYTEXT = q.value(1).toString(); // USERQUERYTEXT
            USERQUERYDATE = q.value(2).toString(); // USERQUERYDATE
            RESPONSE = q.value(3).toString(); // RESPONSE
        }

        Data &operator=(const Data &other) {
            if (this == &other) return *this;

            USERQUERYID = other.USERQUERYID;
            USERQUERYTEXT = other.USERQUERYTEXT;
            USERQUERYDATE = other.USERQUERYDATE;
            RESPONSE = other.RESPONSE;
            return *this;
        }

        template<typename C>
        bool match(const C &c) const {
            return false;
        }

        bool match(const Self::USERQUERYID &in) const {
            return this->USERQUERYID == in.v_;
        }

        bool match(const Self::USERQUERYTEXT &in) const {
            bool res = this->USERQUERYTEXT.toUpper() == in.v_.toUpper();
            return res;
        }

        bool match(const Self::USERQUERYDATE &in) const {
            bool res = this->USERQUERYDATE.toUpper() == in.v_.toUpper();
            return res;
        }

        bool match(const Self::RESPONSE &in) const {
            bool res = this->RESPONSE.toUpper() == in.v_.toUpper();
            return res;
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
    QString name() const Q_DECL_OVERRIDE { return "INFO"; }

    QString query() const Q_DECL_OVERRIDE {
        return "SELECT * FROM INFO ";
    }

    QString ensureQuery() const Q_DECL_OVERRIDE {
        return "CREATE TABLE INFO(USERQUERYID INTEGER PRIMARY KEY, USERQUERYTEXT CLOB NOT NULL, USERQUERYDATE TEXT NOT NULL, RESPONSE CLOB, UNIQUE(USERQUERYTEXT))";
    }

    QString ensureIndexQuery() const Q_DECL_OVERRIDE {
        return "CREATE INDEX IF NOT EXISTS IDX_INFO_USERQUERYID ON INFO(USERQUERYID)";
    }

    explicit DBTableInfo() {

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
            sql = "INSERT INTO INFO(USERQUERYTEXT, USERQUERYDATE, RESPONSE) VALUES(?, ?, ?)";
        }
        else {
            sql = "UPDATE INFO SET USERQUERYTEXT = ?, USERQUERYDATE = ?, RESPONSE = ? WHERE USERQUERYID = ?";
        }

        QSqlQuery stmt(*db);
        stmt.prepare(sql);

        stmt.bindValue(0, entity->USERQUERYTEXT);
        stmt.bindValue(1, entity->USERQUERYDATE);
        stmt.bindValue(2, entity->RESPONSE);
        if (entity->id() > 0)
            stmt.bindValue(3, entity->USERQUERYID);

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

        QString sql = "DELETE FROM INFO WHERE USERQUERYID = ?";

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
