#include <iostream>
#include <thread> // Required for thread
#include <mutex> // Required for mutex
#include <vector> // Required for vector
#include <chrono> // Required for time handling
#include <atomic> // Required for atomic variables

using namespace std; // This allows us to avoid using std:: prefix

// Define a mutex to synchronize access to shared resources
mutex mtx; // Mutex to prevent race conditions
atomic<int> completed_tasks(0); // Atomic counter to safely track completed tasks


// Function that simulates a long-running task
void perform_task(int task_id, int duration) {
	// Lock the mutex to ensure only one thread prints at a time
	lock_guard<mutex> lock(mtx); // Automatic unlock when going out of scope

	cout << "Task " << task_id << " started. Estimated duration: " << duration << " seconds.\n";

	// Simulate doing some work (sleep for the task's duration)
	this_thread::sleep_for(chrono::seconds(duration));

	// After the task finishes, update the counter and notify the user
	completed_tasks++;
	cout << "Task " << task_id << " completed!\n";
}

int main() {
	// Ask the user how many tasks they want to run
	int num_tasks;
	cout << "Enter the number of tasks to run: ";
	cin >> num_tasks;

	// Vector to hold the thread objects
	vector<thread> threads;

	// Start all tasks as threads
	for (int i = 0; i < num_tasks; ++i) {
		int duration = rand() % 5 + 1; // Random task duration between 1 and 5 seconds
		threads.push_back(thread(perform_task, i + 1, duration));
	}

	// Provide updates to the user about overall progress
	while (completed_tasks < num_tasks) {
		{
			// Locking to safely print the progress without race conditions
			lock_guard<mutex> lock(mtx);
			cout << "Completed tasks: " << completed_tasks << " / " << num_tasks << "\n";
		}

		// Sleep for a second to avoid spamming the user with too many updates
		this_thread::sleep_for(chrono::seconds(1));
	}

	// Wait for all threads to complete
	for (auto& t : threads) {
		if (t.joinable()) {
			t.join();
		}
	}

	// Final message once all tasks are completed
	cout << "All tasks completed successfully!\n";

	return 0;
}