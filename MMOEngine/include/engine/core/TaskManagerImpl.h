/*
Copyright (C) 2007 <SWGEmu>. All rights reserved.
Distribution of this file for usage outside of Core3 is prohibited.
*/

#ifndef TASKMANAGERIMPL_H_
#define TASKMANAGERIMPL_H_

#include "system/lang.h"

#include "engine/log/Logger.h"

#include "TaskManager.h"

#include "TaskQueue.h"

namespace engine {
  namespace core {

	class TaskWorkerThread;
	class TaskScheduler;
	class Task;

	class TaskManagerImpl : public TaskManager, public Mutex, public Logger {
		Vector<TaskQueue*> taskQueues;

		Vector<Reference<TaskWorkerThread*> > workers;
		Vector<Reference<TaskScheduler*> > schedulers;

		Vector<Reference<TaskScheduler*> > ioSchedulers;

#ifdef WITH_STM
		TaskWorkerThread* serialWorker;
		TaskQueue serialTaskQueue;
#endif

		int workerThreads;
		int schedulerThreads;

		AtomicInteger currentTaskScheduler;

		bool shuttingDown;

	public:
		static const int DEFAULT_WORKER_THREADS = 8;
		static const int DEFAULT_SCHEDULER_THREADS = 20;
		static const int DEFAULT_IO_SCHEDULER_THREADS = 10;
		static const int DEFAULT_TASK_QUEUES = 10;

		TaskManagerImpl();

		virtual ~TaskManagerImpl();

		void initialize();

		void initialize(int workerCount, int schedulerCount, int ioCount = DEFAULT_IO_SCHEDULER_THREADS);

		void start();

		void shutdown();

		Vector<Locker*>* blockTaskManager();
		void unblockTaskManager(Vector<Locker*>* lockers);

#ifdef WITH_STM
		void retryTaskInSerial(Task* task);

		Thread* getSerialWorker();
#endif

		void executeTask(Task* task);
		void executeTask(Task* task, int taskQueue);
		void executeTasks(const Vector<Task*>& tasks);

		void executeTaskFront(Task* task);
		void executeTaskRandom(Task* task);
		bool getNextExecutionTime(Task* task, Time& nextExecutionTime);

		bool isTaskScheduled(Task* task);

		void scheduleTask(Task* task, uint64 delay = 0);
		void scheduleTask(Task* task, Time& time);

		void scheduleIoTask(Task* task, uint64 delay = 0);
		void scheduleIoTask(Task* task, Time& time);

		void rescheduleTask(Task* task, uint64 delay = 0);
		void rescheduleTask(Task* task, Time& time);

		void rescheduleIoTask(Task* task, uint64 delay = 0);
		void rescheduleIoTask(Task* task, Time& time);

		bool cancelTask(Task* task);

		void flushTasks();

		void testScheduler();

		String getInfo(bool print);

		int getScheduledTaskSize();

		int getIoScheduledTaskSize();

		int getExecutingTaskSize();

	private:
		TaskScheduler* getTaskScheduler();
		TaskScheduler* getIoTaskScheduler();

		void setTaskScheduler(Task* task, TaskScheduler* scheduler);

		/*inline Task* getTask() {
			return tasks.pop();
		}*/

		friend class TaskScheduler;
		friend class TaskWorkerThread;
	};

  } // namespace core
} // namespace engine

using namespace engine::core;

#endif /* TASKMANAGERIMPL_H_ */
