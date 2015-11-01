#ifndef DBTableAbstract_H
#define DBTableAbstract_H

#include <algorithm>
#include <functional>
#include <assert.h>

#include <QVector>
#include <QMap>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QString>
#include <QStringList>
#include <QDebug>

enum OP {
    EQUAL = 0, GREATER, LESS, GREATER_OR_EQUAL, LESS_OR_EQUAL, NOT_EQUAL
};

#define DBTableAbstract_SelDefine(Type) \
    struct Data; \
    typedef Type Self; \
    typedef QVector<Self::Data> DataSet; \
    typedef QVector<Self::Data *> Cache; \
    typedef QMap<int, Self::Data *> IndexById; \
    Cache m_cache; \
    IndexById m_indexById; \
    \
    /* Destructor: clears any data records stored in memory */ \
    ~Type() { \
        destroyCache(); \
    } \
    /* Removes all records stored in memory (cache) for the table */ \
    void destroyCache() { \
        std::for_each(m_cache.begin(), m_cache.end(), std::mem_fun(&Data::destroy)); \
        m_cache.clear(); \
        /* no memory release since it just stores pointer and the according objects are in cache */ \
        m_indexById.clear(); \
    } \


template<class V>
struct DB_Column {
    V v_;
    OP op_;

    DB_Column(const V &v, const OP op = EQUAL) : v_(v), op_(op) { }
};

struct DBTableAbstract {
    explicit DBTableAbstract()
            : m_hit(0), m_miss(0), m_skip(0) {

    };

    virtual ~DBTableAbstract() {

    };

    virtual QString name() const = 0;

    virtual size_t numColumns() const = 0;

    virtual QString query() const = 0;

    virtual QString ensureQuery() const = 0;

    virtual QString ensureIndexQuery() const = 0;

    bool exists(QSqlDatabase *db) const {
        return db->tables().contains(this->name());
    }

    /** Creates the database table if the table does not exist*/
    bool ensure(QSqlDatabase *db) {
        if (!db)
            return false;

        if (!exists(db)) {
            QSqlQuery q(*db);
            q.prepare(ensureQuery());
            if (!q.exec())
                return false;
        }

        this->ensureIndex(db);

        return true;
    }

    bool ensureIndex(QSqlDatabase *db) {
        if (!db)
            return false;

        QSqlQuery q(*db);
        q.prepare(this->ensureIndexQuery());
        return q.exec();
    }

protected:
    size_t m_hit, m_miss, m_skip;
};

template<typename Arg1>
void condition(QString &out, bool /*op_and*/, const Arg1 &arg1) {
    out += Arg1::name();
    switch (arg1.op_) {
        case GREATER:
            out += " > ? ";
            break;
        case GREATER_OR_EQUAL:
            out += " >= ? ";
            break;
        case LESS:
            out += " < ? ";
            break;
        case LESS_OR_EQUAL:
            out += " <= ? ";
            break;
        case NOT_EQUAL:
            out += " != ? ";
            break;
        default:
            out += " = ? ";
            break;
    }
}

template<typename Arg1, typename... Args>
void condition(QString &out, bool op_and, const Arg1 &arg1, const Args &... args) {
    out += Arg1::name();
    switch (arg1.op_) {
        case GREATER:
            out += " > ? ";
            break;
        case GREATER_OR_EQUAL:
            out += " >= ? ";
            break;
        case LESS:
            out += " < ? ";
            break;
        case LESS_OR_EQUAL:
            out += " <= ? ";
            break;
        case NOT_EQUAL:
            out += " != ? ";
            break;
        default:
            out += " = ? ";
            break;
    }
    out += op_and ? " AND " : " OR ";
    condition(out, op_and, args...);
}

template<typename Arg1>
void bindValue(QSqlQuery &stmt, int index, const Arg1 &arg1) {
    stmt.bindValue(index, arg1.v_);
}

template<typename Arg1, typename... Args>
void bindValue(QSqlQuery &stmt, int index, const Arg1 &arg1, const Args &... args) {
    stmt.bindValue(index, arg1.v_);
    bindValue(stmt, index + 1, args...);
}

template<typename TABLE, typename... Args>
const typename TABLE::DataSet findBy(TABLE *table, QSqlDatabase *db, bool op_and, const Args &... args) {
    typename TABLE::DataSet result;

    if (!db)
        return result;

    QString query = table->query() + " WHERE ";
    condition(query, op_and, args...);

    QSqlQuery stmt(*db);
    stmt.prepare(query);

    bindValue(stmt, 0, args...);

    stmt.exec();

    while (stmt.next()) {
        typename TABLE::Data entity(stmt, table);
        result.push_back(entity);
    }

    return result;
}

template<class DATA, typename Arg1>
bool match(const DATA *data, const Arg1 &arg1) {
    return data->match(arg1);
}

template<class DATA, typename Arg1, typename... Args>
bool match(const DATA *data, const Arg1 &arg1, const Args &... args) {
    if (data->match(arg1))
        return match(data, args...);
    else
        return false; // Short-circuit evaluation
}

struct SorterByACCESSINFO {
    template<class DATA>
    bool operator()(const DATA &x, const DATA &y) {
        return x.ACCESSINFO < y.ACCESSINFO;
    }
};

#endif