#include "Object.h"


using namespace Graphics;

//------------------------------------------------------------------------------
// Object
//------------------------------------------------------------------------------


Object::Object(const Object & other)
{
	// copy the name, the ref count mutex and the user data.
	mObjectName = other.mObjectName;
	mRefCountMutex = other.mRefCountMutex;

	// mReferenceCount and mAutomaticDelete are not copiable.
	mReferenceCount = 0;
	mAutomaticDelete = true;
}

Object & Graphics::Object::operator=(const Object & other)
{
	// copy the name, the ref count mutex and the user data.
	mObjectName = other.mObjectName;
	mRefCountMutex = other.mRefCountMutex;

	// mReferenceCount and mAutomaticDelete are not copiable.
	// ...

	return *this;
}

void Object::incReference() const
{

	// Lock mutex
	if (refCountMutex())
		const_cast<IMutex*>(refCountMutex())->lock();

	++mReferenceCount;

	// Unlock mutex
	if (refCountMutex())
		const_cast<IMutex*>(refCountMutex())->unlock();

}

void Object::decReference()
{
	// Save local copy in case of deletion.
	IMutex* mutex = mRefCountMutex;

	// Lock mutex.
	if (mutex)
		mutex->lock();

	assert(mReferenceCount);
	--mReferenceCount;
	if (mReferenceCount == 0 && automaticDelete())
		delete this;

	// Unlock mutex.
	if (mutex)
		mutex->unlock();

}

Object::~Object()
{
	if (mReferenceCount && !automaticDelete())
	{
		//bug!!!
		//Log::bug(Say(
		//	"Object '%s' is being deleted having still %n references! Pissible causes:\n"
		//	"- illegal use of the 'delete' operator on an Object. Use ref<> instead.\n"
		//	"- explicit call to Object::incReference().\n"
		//) << mObjectName << mReferenceCount);

	}
}
//------------------------------------------------------------------------------
