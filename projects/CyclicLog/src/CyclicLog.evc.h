#ifndef __CYCLICLOG_EVC_H__
#define __CYCLICLOG_EVC_H__

#include "BasicEventHandling/BasicEventHandling.h"

BEGIN_EVENT_DECL_NAMESPACE(CyclicLogEvents)
	STATIC_EVENT_DECL(ev_file_is_read_only)
	STATIC_EVENT_DECL(ev_invalid_file_size)
	STATIC_EVENT_DECL(ev_changing_file_size_failure)
	STATIC_EVENT_DECL(ev_impossible_to_create_file)
	STATIC_EVENT_DECL(ev_impossible_to_create_dir)
	STATIC_EVENT_DECL(ev_cannot_add_entry)
	STATIC_EVENT_DECL(ev_cannot_init)
	STATIC_EVENT_DECL(ev_not_initialized)
END_EVENT_NAMESPACE()


#endif // __CYCLICLOG_EVC_H__