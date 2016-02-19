#include "AbstractTaskData.h"

AbstractTaskData::AbstractTaskData(QSqlRecord record, DateMode dateMode):
    m_record(record), m_dateMode(dateMode)
{

}

AbstractTaskData::AbstractTaskData(DateMode dateMode):
    m_dateMode(dateMode), m_ID(-1)
{

}

QSqlRecord AbstractTaskData::record() const
{
    return m_record;
}

void AbstractTaskData::setRecord(const QSqlRecord &record)
{
    m_record = record;
}

AbstractTaskData::DateMode AbstractTaskData::dateMode() const
{
    return m_dateMode;
}

void AbstractTaskData::setDateMode(const DateMode &dateMode)
{
    m_dateMode = dateMode;
}

QDate AbstractTaskData::taskStart() const
{
    return m_taskStart;
}

void AbstractTaskData::setTaskStart(const QDate &taskStart)
{
    m_taskStart = taskStart;
}

QDate AbstractTaskData::taskEnd() const
{
    return m_taskEnd;
}

void AbstractTaskData::setTaskEnd(const QDate &taskEnd)
{
    m_taskEnd = taskEnd;
}

QString AbstractTaskData::taskName() const
{
    return m_taskName;
}

void AbstractTaskData::setTaskName(const QString &taskName)
{
    m_taskName = taskName;
}

int AbstractTaskData::taskType() const
{
    return m_taskType;
}

void AbstractTaskData::setTaskType(int taskType)
{
    m_taskType = taskType;
}

int AbstractTaskData::ID() const
{
    return m_ID;
}

void AbstractTaskData::setID(int ID)
{
    m_ID = ID;
}

int AbstractTaskData::percent() const
{
    return m_percent;
}

void AbstractTaskData::setPercent(int percent)
{
    m_percent = percent;
}

void AbstractTaskData::saveToDB()
{

}

void AbstractTaskData::removeFromDB()
{

}
