#pragma once

namespace Graphics
{
  //------------------------------------------------------------------------------
  // IMutex
  //------------------------------------------------------------------------------
  /**
   * An interface to implement simple platform-independent mutexes used to protect critical sections.
   * /sa Graphics::Object::setRefCountMutex().
  */
  class IMutex
  {
  public:
    //! Locks the mutex.
    virtual void lock() = 0;

    //! Unlocks the mutex.
    virtual void unlock() = 0;

    //! Returns 1 if locked, 0 if non locked, -1 if unknown.
    virtual int isLocked() const = 0;
  };
}

