#ifndef zenic_Rtti_h
#define zenic_Rtti_h

#include "../Debug/Assert.h"

namespace zenic
{

class Rtti
{
public:

	Rtti(const Rtti* parent);

	const Rtti* parent() const;

private:

	const Rtti* m_parent;
};

#include "Rtti.inl"

}

#define ZENIC_RTTI \
	public: \
		static const zenic::Rtti ms_rtti; \
		virtual const zenic::Rtti* rtti() const;

#define ZENIC_ROOT_RTTI ZENIC_RTTI

#define ZENIC_CREATE_RTTI(curr, parent) \
		const zenic::Rtti type::ms_rtti(&parent::ms_rtti); \
		const zenic::Rtti* type::rtti() const { return &ms_rtti; }

#define ZENIC_CREATE_ROOT_RTTI(type) \
		const zenic::Rtti type::ms_rtti(0); \
		const zenic::Rtti* type::rtti() const { return &ms_rtti; }

#endif
