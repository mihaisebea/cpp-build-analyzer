#include "TimelineEntry.h"

#include <cassert>

TimelineEntry::TimelineEntry(const TEventInstanceId& id,
                             const char* name,
                             std::chrono::nanoseconds startTimestamp,
                             std::chrono::nanoseconds finishTimestamp,
                             const TProcessId& processId,
                             const TThreadId& threadId,
                             const TProcessorIndex processorIndex)
    : m_id(id)
    , m_name(name)
    , m_startTimestamp(startTimestamp)
    , m_finishTimestamp(finishTimestamp)
    , m_processId(processId)
    , m_threadId(threadId)
    , m_processorIndex(processorIndex)
    , m_children()
    , m_properties()
{
    assert(startTimestamp <= finishTimestamp);
}

TimelineEntry::~TimelineEntry()
{
}

void TimelineEntry::AddChild(TimelineEntry* entry)
{
    assert(std::find(m_children.begin(), m_children.end(), entry) == m_children.end());

    m_children.push_back(entry);
}

void TimelineEntry::RemoveChild(const TEventInstanceId& id)
{
    auto it = std::find_if(m_children.begin(), m_children.end(), [&](const TimelineEntry* child)
    {
        return child->GetId() == id;
    });
    assert(it != m_children.end());

    m_children.erase(it);
}

void TimelineEntry::SetName(const std::string& name)
{
    m_name = name;
}

void TimelineEntry::SetFinishTimestamp(const std::chrono::nanoseconds& timestamp)
{
    assert(m_startTimestamp <= timestamp);

    m_finishTimestamp = timestamp;
}

bool TimelineEntry::OverlapsWith(const TimelineEntry* entry) const
{
    return m_startTimestamp < entry->GetFinishTimestamp() &&
           entry->GetStartTimestamp() < m_finishTimestamp;
}

bool TimelineEntry::AddProperty(const std::string& key, const std::string& value)
{
    auto result = m_properties.try_emplace(key, value);
    return result.second;
}
