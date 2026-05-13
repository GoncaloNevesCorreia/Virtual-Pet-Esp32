#include "Tasks.h"

/**
 * 13 bytes por Execution Block
 * 12 bytes por Task
 */
Tasks& Tasks::delay(Callback callback, int delay, bool enabled) {
  createBlock(enabled);

  addTask(callback, delay);

  return *this;
}

Tasks& Tasks::then(Callback callback, int delay, bool enabled) {
  if (!head) {
    return Tasks::delay(callback, delay, enabled);
  }

  addTask(callback, delay);

  return *this;
}

void Tasks::update() {
  for (ExecutionBlock* block = head; block != NULL; block = block->next) {
    if (!block->enabled) continue;

    const unsigned long currentTime = millis();

    if (currentTime - block->currentTask->previousTime >= (unsigned long)block->delay) {
      Task* currentTask = block->currentTask;

      block->delay = currentTask->delay;
      block->currentTask = currentTask->next;
      block->currentTask->previousTime = currentTime;

      currentTask->callback();
    }
  }
}

Tasks::Handler Tasks::getHandler() {
  return Handler(tail);
}

void Tasks::Handler::pause() {
  if (!_block) return;
  _block->enabled = false;
}

void Tasks::Handler::resume() {
  if (!_block) return;
  _block->enabled = true;
}

void Tasks::Handler::skip() {
  if (!_block) return;
  _block->delay = 0;
}

void Tasks::addTask(Callback callback, int delay) {
  Task* newTask = new Task{callback, delay, 0, NULL};

  ExecutionBlock* block = tail;

  if (!block->head) {
    block->head = newTask;
    block->tail = newTask;
    block->currentTask = newTask;
  } else {
    block->tail->next = newTask;
    block->tail = newTask;
  }

  block->tail->next = block->head;
}

void Tasks::createBlock(bool enabled) {
  ExecutionBlock* newBlock = new ExecutionBlock{NULL, 0, enabled, NULL, NULL, NULL};

  if (!head) {
    head = tail = newBlock;
  } else {
    tail->next = newBlock;
    tail = newBlock;
  }
}