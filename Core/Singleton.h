#ifndef Singleton_H_
#define Singleton_H_

#include <QMutex> 

template<class T>
class Singleton {
public:
    Singleton(T &rObject) {
        Q_ASSERT_X(!s_pInstance, "constructor", "Only one instance of this class is permitted.");
        s_pInstance = &rObject;
    }

    ~Singleton() {
        Q_ASSERT_X(s_pInstance, "destructor", "The singleton instance is invalid.");
        s_pInstance = Q_NULLPTR;
    }

    static T &instance() {
        static QMutex mutex;
        /*
         * Use double checking
         */
        if (s_pInstance == Q_NULLPTR) {
            mutex.lock();

            if (s_pInstance == Q_NULLPTR)
                s_pInstance = new T;

            mutex.unlock();
        }

        Q_ASSERT_X(s_pInstance, "instancing", "The singleton has not yet been created.");
        return (*s_pInstance);
    }

private:
    static T *s_pInstance;

    Q_DISABLE_COPY(Singleton);
};

template<class T> T *Singleton<T>::s_pInstance = Q_NULLPTR;

#endif // MM_EX_SINGLETON_H_
