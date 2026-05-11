#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>

typedef void (*Callback)();

class Tasks {
 public:
  Tasks& delay(Callback callback, int delay, bool enabled = true);
  Tasks& then(Callback callback, int delay, bool enabled = true);

  struct Handler;
  Tasks::Handler getHandler();

  void update();

 private:
  struct Task {
    Callback callback;          // 2
    const int delay;            // 0
    unsigned long previousTime; // 2
    Task* next;                 // 2
  };

  struct ExecutionBlock {
    Task* currentTask;     // 2 bytes
    int delay;             // 2 bytes
    bool enabled;          // 1 byte
    ExecutionBlock* next;  // 2 bytes
    Task* head;            // 2 bytes
    Task* tail;            // 2 bytes
  };

  ExecutionBlock* head = nullptr;
  ExecutionBlock* tail = nullptr;

  void createBlock(bool enabled);

  void addTask(Callback callback, int delay);

 public:
  struct Handler {
    Handler(ExecutionBlock* block) : _block(block) {};
    Handler() {};
    void pause();
    void resume();
    void skip();

   private:
    ExecutionBlock* _block;
  };
};
#endif
