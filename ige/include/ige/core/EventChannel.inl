#include "EventChannel.hpp"
#include "ige/utility/Assert.hpp"
#include "ige/utility/Types.hpp"
#include <concepts>
#include <ranges>
#include <utility>

namespace ige::core {

/**
 * @brief Create a new EventReader.
 *
 * @param id The index of the EventReader within the EventChannel's list of
 *              EventReaders.
 */
template <Event E>
EventReader<E>::EventReader(usize id, mpsc::Sender<usize> death_notifier)
    : m_id(id)
    , m_death_notifier(death_notifier)
{
}

/**
 * @brief Destroy the EventReader and unregister it from the EventChannel.
 */
template <Event E>
EventReader<E>::~EventReader()
{
    m_death_notifier.send(m_id);
}

/**
 * @brief Push an event to the channel.
 *
 * The event is constructed in place, by forwarding all arguments to the
 * constructor.
 *
 * @tparam Args The type of the arguments.
 */
template <Event E>
template <class... Args>
    requires std::constructible_from<E, Args...>
void EventChannel<E>::emplace(Args&&... args)
{
    if (reader_count() != 0) {
        free_dead_readers();

        m_last_event_id++;
        m_buffer.emplace(std::forward<Args>(args)...);
    }
}

/**
 * @brief Push an event to the channel.
 *
 * @param event The event to push.
 *
 * @sa EventChannel::emplace()
 */
template <Event E>
inline void EventChannel<E>::push(E&& event)
{
    emplace(std::forward<E>(event));
}

/**
 * @brief Create an event reader for this channel.
 *
 * @return Reader A new event reader for this channel.
 */
template <Event E>
auto EventChannel<E>::create_reader() -> Reader
{
    if (m_free_readers.empty()) {
        // no free reader, create a new one
        auto id = m_readers_last_event.size();

        m_readers_last_event.push_back(m_last_event_id);
        return Reader(id, m_death_messages.make_sender());
    } else {
        // use a free reader
        auto id = m_free_readers.back();
        m_free_readers.pop_back();

        m_readers_last_event[id] = m_last_event_id;

        return Reader(id, m_death_messages.make_sender());
    }
}

/**
 * @brief Read a series of events from the channel.
 *
 * In order to read events from a channel, a reader must be first be created
 * by calling EventChannel::create_reader(). The range of events returned by
 * this function is the range of events that were pushed to the channel since
 * the last time the reader was used to read events from this channel.
 *
 * @param reader An EventReader suitable for reading events from this channel.
 * @return A range of events from the channel.
 *
 * @sa EventChannel::create_reader()
 */
template <Event E>
decltype(auto) EventChannel<E>::read(Reader& reader) const
{
    IGE_ASSERT(
        reader.m_id < m_readers_last_event.size(),
        "Invalid EventReader");

    usize last_event_id = m_readers_last_event[reader.m_id];
    usize events_missed = 0;

    if (last_event_id < m_last_event_id) {
        // reader has some events to read
        events_missed = m_last_event_id - last_event_id;
        m_readers_last_event[reader.m_id] = m_last_event_id;
    }

    return std::ranges::subrange(
        m_buffer.end() - events_missed,
        m_buffer.end());
}

/**
 * @brief Get the total number of readers active on this channel.
 *
 * @return The number of readers active on this channel.
 */
template <Event E>
usize EventChannel<E>::reader_count() const
{
    free_dead_readers();
    return m_readers_last_event.size() - m_free_readers.size();
}

template <Event E>
void EventChannel<E>::free_dead_readers() const
{
    while (auto id = m_death_messages.try_receive()) {
        m_free_readers.push_back(*id);
    }
}

}