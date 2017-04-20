#pragma once
#include <agents.h>
#include <ppltasks.h>

template<typename T>
void callAfter(const T& callback, int timeMs)
{
	concurrency::task_completion_event<void> tce;
	auto call = concurrency::call<int>([callback, timeMs](int)
	{
		callback();
		tce.set();
	});

	auto timer = new concurrency::timer<int>(timeMs, 0, call, false);
	concurrency::task<void> event_set(tce);
	event_set.then([timer, call]()
	{
		delete call;
		delete timer;
	});

	timer->start()();
}