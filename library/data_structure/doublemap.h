#ifndef DOUBLEMAP_H
#define DOUBLEMAP_H

#include <QMap>

template <typename T1, typename T2>
class DoubleMap
{
public:
    const T1 operator[](const T2& key) const
    {
        return backwardMap[key];
    }
    const T2 operator[](const T1& key) const
    {
        return forwardMap[key];
    }
    T1& operator[](const T2& key)
    {
        return backwardMap[key];
    }
    T2& operator[](const T1& key)
    {
        return forwardMap[key];
    }
    bool has(const T1& key) const
    {
        return forwardMap.contains(key);
    }
    bool has(const T2& key) const
    {
        return backwardMap.contains(key);
    }
    void update(const T1& key1, const T2& key2)
    {
        if (forwardMap.contains(key1)) {
            backwardMap.remove(forwardMap[key1]);
        }
        if (backwardMap.contains(key2)) {
            forwardMap.remove(backwardMap[key2]);
        }
        forwardMap[key1]  = key2;
        backwardMap[key2] = key1;
    }
    void clear()
    {
        forwardMap.clear();
        backwardMap.clear();
    }

private:
    QMap<T1, T2> forwardMap;
    QMap<T2, T1> backwardMap;
};

template <typename T>
class DoubleMap<T, T>
{
public:
    DoubleMap() = delete;
};

#endif  // DOUBLEMAP_H
