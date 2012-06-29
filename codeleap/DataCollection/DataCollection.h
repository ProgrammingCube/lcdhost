#ifndef DATACOLLECTION_H
#define DATACOLLECTION_H

#include <QList>
#include <QStringList>
#include <QDebug>

typedef struct {qreal value; int duration;} DataPoint;

class DataPointCollection : public QList<DataPoint> {
public:
    DataPointCollection() : QList<DataPoint>() {}
};

class DataLine : public DataPointCollection
{
    friend class DataLineCollection;

    int limit_;
    int totalDuration_;
    int popDuration_;

    void setPopDuration(int popDuration) { popDuration_ = popDuration;}

    void copyFrom(DataLine src)
    {
        limit_ = src.limit_;
        totalDuration_ = src.totalDuration_;
        popDuration_ = src.popDuration_;
        name = src.name;

        qDebug() << src.count();
        for(int i = 0; i<src.count(); i++)
            append( src[i] );
    }

#ifndef LH_MONITORING_LIBRARY
    DataLine() : DataPointCollection()
    {
        name = "Unnamed";
        limit_ = 0;
        totalDuration_ = 0;
        popDuration_ = 0;
    }
#endif

public:

#ifdef LH_MONITORING_LIBRARY
    bool aggregate;
    bool group;
    DataLine() : DataPointCollection()
    {
        name = "Unnamed";
        limit_ = DATA_CACHE_MAX_POINTS;
        totalDuration_ = 0;
        popDuration_ = 0;
    }
#endif

    QString name;

    DataLine(QString name, int limit) : DataPointCollection()
    {
        this->name = name;
        limit_ = limit;
        totalDuration_ = 0;
        popDuration_ = 0;
    }

    int limit() { return limit_; }

    qreal averageDuration()
    {
        if(length()==0)
            return 0;
        else
            return ((qreal)totalDuration_)/(length());
    }

    qreal predictedTotalDuration()
    {
        return averageDuration() * limit_;
    }

    void setLimit(int limit)
    {
        limit_ = limit;
        while(limit_<length())
            removeLast();
    }

    void clear()
    {
        DataPointCollection::clear();
        totalDuration_ = 0;
    }

    void addValue(qreal value, int duration)
    {
        int popped_duration = 0;
        bool popped_once = false;
        if (length()>=limit_)
        {
            if(popDuration_ != 0) qDebug() << "popDuration_: " << popDuration_;
            while( (!popped_once) || (popped_duration + last().duration) <= popDuration_ )
            {
                popped_once = true;
                popped_duration += last().duration;
                totalDuration_ -= last().duration;
                pop_back();
                if(popDuration_ != 0) qDebug() << "pop";
            }
            if(popDuration_ != 0) qDebug() << "popped: " << popped_duration;
        }
        push_front( (DataPoint){value, duration} );
        totalDuration_ += duration;
    }
};

class DataLineCollection : private QList<DataLine>
{
    int limit_;

private:
    void setNames(QStringList names)
    {
        this->clear();
        for(int i = 0; i < names.count(); i++)
            add(names[i]);
    }

public:
    DataLineCollection() : QList<DataLine>() {
        limit_ = 0;
    }
    DataLineCollection(int limit) : QList<DataLine>() {
        limit_ = limit;
    }

#ifdef LH_MONITORING_LIBRARY
    DataLine &operator[](QString name)
    {
        for(int i = 0; i<this->count(); i++)
            if(QList<DataLine>::operator [](i).name == name)
                return QList<DataLine>::operator [](i);
        Q_ASSERT_X(false,"DataLineCollection","List out of bounds");
        return QList<DataLine>::operator [](-1);
    }

    bool contains(QString name)
    {
        for(int i = 0; i<this->count(); i++)
            if(QList<DataLine>::operator [](i).name == name)
                return true;
        return false;
    }

    bool indexOf(QString name)
    {
        for(int i = 0; i<this->count(); i++)
            if(QList<DataLine>::operator [](i).name == name)
                return i;
        return -1;
    }
#endif

    //const DataLine &operator[](int i) const;
    DataLine &operator[](int i)
    {
        return  QList<DataLine>::operator [](i);
    }
    const DataLine &operator[](int i) const
    {
        return  QList<DataLine>::operator [](i);
    }
    const DataLine at(int i) const
    {
        return QList<DataLine>::at(i);
    }

    void clear()
    {
        QList<DataLine>::clear();
    }

    int limit() { return limit_; }

    int pointLength()
    {
        return (length()==0? 0 : (*this)[0].length());
    }

    int count() const
    {
        return QList<DataLine>::count();
    }

    void add(QString name)
    {
        QList<DataLine>::append( DataLine(name, limit_) );
    }

    void append(DataLine dl)
    {
        QList<DataLine>::append( dl );
    }

    void setCount(int c, QString nameTemplate = "line %1")
    {
        this->clear();
        for(int i = 0; i < c; i++)
            add(nameTemplate.arg(i));
    }

    QStringList names()
    {
        QStringList names;
        for(int i = 0; i < count(); i++)
            names.append(this->at(i).name);
        return names;
    }

    void setLimit(int limit)
    {
        limit_ = limit;
        for(int i =0; i<length(); i++ )
            (*this)[i].setLimit(limit);
    }

    DataLineCollection averageOver(int desiredDuration)
    {
        DataLineCollection dlc;
        dlc.setLimit(limit_);
        dlc.setNames(this->names());
        //qDebug() << "averageOver " << desiredDuration << " ms";
        for(int lineIndex = 0; lineIndex < dlc.count(); lineIndex++)
        {
            QList<DataLine>::operator [](lineIndex).setPopDuration(desiredDuration);

            DataLine dl = this->at(lineIndex);

            qreal value = 0;
            int duration = 0;
            int count = 0;
            for(int i = dl.length() - 1; i >=0; i--)
            {
                value += dl.at(i).value;
                duration += dl.at(i).duration;
                count ++;

                if(duration == desiredDuration || (i > 0 && duration + dl.at(i-1).duration > desiredDuration))
                {
                    dlc[lineIndex].addValue(value / count, desiredDuration);
                    value = 0;
                    duration = 0;
                    count = 0;
                }
            }
        }
        return dlc;
    }

    void copyFrom(DataLineCollection src)
    {
        limit_ = src.limit();
        //setNames(src.names());
        for(int lineIndex = 0; lineIndex < src.count(); lineIndex++)
        {
            DataLine dl;
            dl.copyFrom(src[lineIndex]);
            append( dl );
        }
    }



    class const_iterator
    {
    private:
        int index_;
        const DataLineCollection *dlc_;
    public:
        enum etype{START,END};
        const_iterator(const DataLineCollection *dlc, etype t)
        {
            dlc_ = dlc;
            index_ = (t==START? 0 : dlc_->count());
        }
        const DataLine &operator*() const { return (*dlc_)[index_]; }
        bool operator!=(const const_iterator& it) { return (index_!=it.index_); }
        const_iterator& operator++() { index_++; return *this; }
    };
    const_iterator begin() const { return const_iterator(this, const_iterator::START); }
    const_iterator end() const { return const_iterator(this, const_iterator::END); }
}
;

#endif // DATACOLLECTION_H