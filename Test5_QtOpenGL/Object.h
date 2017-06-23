#pragma once

#include "ref.h"
#include "IMutex.h"

#include <string>
#include <assert.h>


namespace Graphics
{
	/**
	 * The base class for all the reference counted objects.
	 * See also Graphics::ref.
	*/
	class Object
	{

	public:
		//! Constructor.
		Object()
		{
			mRefCountMutex = NULL;
			mReferenceCount = 0;
			mAutomaticDelete = true;
		}

		//! Copy constructor: copies the name, ref count mutex and user data.
		Object(const Object& other);

		//! Copy operator: copies the object's name, ref count mutex and user data.
		Object& operator=(const Object& other);

		//! The name of the object, by default set to the object's class name.
		const std::string& objectName() const { return mObjectName; }

		//! The name of the object, by default set to the object's class name in debug builds.
		void setObjectName(const char* name) { mObjectName = name; }

		//! The name of the object, by default set to the object's class name in debug builds.
		void setObjectName(const std::string& name) { mObjectName = name; }

		//! The mutex used to protect the reference counting of an Object across multiple threads.
		void setRefCountMutex(IMutex* mutex) { mRefCountMutex = mutex; }

		//! The mutex used to protect the reference counting of an Object across multiple threads.
		IMutex* refCountMutex() { return mRefCountMutex; }

		//! The mutex used to protect the reference counting of an Object across multiple threads.
		const IMutex* refCountMutex() const { return mRefCountMutex; }

		//! Returns the number of references of an object.
		int referenceCount() const { return mReferenceCount; }
	
		//! Increments the reference count of an object.
		void incReference() const;

		//! Decrements the reference count of an object and deletes it if both automaticDelete() is \p true the count reaches 0.
		void decReference();

		//! If set to true the Object is deleted when its reference count reaches 0
		void setAutomaticDelete(bool autodel_on) { mAutomaticDelete = autodel_on; }

		//! If set to true the Object is deleted when its reference count reaches 0
		bool automaticDelete() const { return mAutomaticDelete; }

		//! Casts an Object to the specified class.
		template<class T>
		T* as() { return cast<T>(this); }

		//! Casts an Object to the specified class.
		template<class T>
		const T* as() const { return cast<const T>(this); }


		// event 处理机制;
		//virtual void event() = 0;


	protected:
		virtual ~Object();
		std::string mObjectName;

	protected:
		IMutex* mRefCountMutex;
		bool mAutomaticDelete;

		//为在 const 可以修改, 用 mutable 修饰;
		mutable int mReferenceCount;
	};

}

