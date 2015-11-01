#ifndef MODEL_H
#define MODEL_H

#ifdef _MSC_VER
#pragma warning (disable:4100)
#endif

#include <algorithm>
#include <Singleton.h>
#include <QDate>
#include <QSharedPointer>
#include <QString>
#include <QMap>
#include <DBTableAbstract.h>

class ModelBase {
public:
    ModelBase()
            : m_db(Q_NULLPTR) {

    };

    virtual ~ModelBase() {

    };

protected:
    QSqlDatabase *m_db;
};

template<class DB_TABLE>
class Model : public ModelBase, public DB_TABLE {
public:
    using DB_TABLE::all;
    using DB_TABLE::getWithCache;
    using DB_TABLE::save;
    using DB_TABLE::remove;

    typedef typename DB_TABLE::COLUMN COLUMN;

    /*
     * Return a list of Data record addresses (DataSet) derived directly from the database
     */
    const typename DB_TABLE::DataSet all(COLUMN col = COLUMN(0), bool asc = true) {
        this->ensure(this->m_db);
        return all(m_db, col, asc);
    }

    template<typename... Args>
    /*
    * Command: find(const Args&... args)
    * Args: One or more Specialised Parameters creating SQL statement conditions used after the WHERE statement.
    * Specialised Parameters: Table_Column_Name(content)[, Table_Column_Name(content)[, ...]]
    * Example:
    * Model::ASSETID(2), Model::ASSETTYPE(Model::TYPE_JEWELLERY)
    * produces SQL statement condition: ASSETID = 2 AND ASSETTYPE = "Jewellery"
    * Returns a DataSet containing the addresses of the items found.
    * The DataSet is empty when nothing found.
    */
    const typename DB_TABLE::DataSet find(const Args &... args) {
        return findBy(this, m_db, true, args...);
    }

    template<typename... Args>
    /*
    * Command: find_or(const Args&... args)
    * Args: One or more Specialised Parameters creating SQL statement conditions used after the WHERE statement.
    * Specialised Parameters: Table_Column_Name(content)[, Table_Column_Name(content)[, ...]]
    * Example:
    * Model::ASSETID(2), Model::ASSETTYPE(Model::TYPE_JEWELLERY)
    * produces SQL statement condition: ASSETID = 2 OR ASSETTYPE = "Jewellery"
    * Returns a DataSet containing the addresses of the items found.
    * The DataSet is empty when nothing found.
    */
    const typename DB_TABLE::DataSet find_or(const Args &... args) {
        return findBy(this, m_db, false, args...);
    }

    /*
    * Return the Data record pointer for the given ID
    * from either memory cache or the database.
    */
    typename DB_TABLE::Data *getWithCache(const int id) {
        return this->getWithCache(id, this->m_db);
    }

    /*
     * Save the Data record memory instance to the database.
     */
    int save(typename DB_TABLE::Data *r) {
        r->save(this->m_db);
        return r->id();
    }

    int save(typename DB_TABLE::Data &r) {
        r.save(this->m_db);
        return r.id();
    }

    /*
    * Save all Data record memory instances contained
    * in the record list (DataSet) to the database.
    */
    template<class DATA>
    int save(const QVector<DATA> &rows) {
        for (auto &r : rows) {
            if (r.id() < 0)
                qDebug() << QString("Incorrect function call to save");
            this->save(&r);
        }

        return rows.size();
    }

    template<class DATA>
    int save(const QVector<DATA *> &rows) {
        for (auto &r : rows)
            this->save(r);

        return rows.size();
    }

    /*
     * Remove the Data record instance from memory and the database.
     */
    bool remove(const int id) {
        return this->remove(id, m_db);
    }

public:
    void preload(const int max_num = 1000) {
        int i = 0;
        for (const auto &item : all()) {
            getWithCache(item.id());
            if (++i >= max_num) break;
        }
    }
};

#endif // 