inline Timer::Timer()
{
	reset();
}

inline u64 Timer::tick() const
{
	return m_tick;
}

inline u64 Timer::deltaTick() const
{
	return m_deltaTick;
}

inline f32 Timer::toSeconds(u64 tick)
{
	return f32(tick * (1.0/Resolution));
}
