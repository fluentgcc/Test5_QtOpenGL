#pragma once

//只能用于 Object 及其子类的引用计数;

namespace Graphics
{
	//------------------------------------------------------------------------------
	// ref
	//------------------------------------------------------------------------------
	/**
	* The ref<> class is used to reference-count an Object.
	* When the last ref<> that points to an Object is deallocated also the pointed Object is deallocated.
	* @note IMPORTANT: assigning to a ref<> 'washes aways' the constness of an object.
	*/
	template<class T>
	class ref
	{
	public:
		// 'const' is required as the copy constructor must have this signature.
		ref(const ref& other)
		{
			mObject = NULL;
			*this = other;
		}

		ref(const T* object = NULL)
		{
			mObject = const_cast<T*>(object);
			if (mObject)
				mObject->incReference();
		}

		template<class T2> ref(const ref<T2>& other)
		{
			mObject = NULL;
			*this = other;
		}

		~ref()
		{
			if (mObject)
				mObject->decReference();
			mObject = NULL;
		}

		// 'const' is required because operator= must have this signature.
		ref& operator=(const ref& other)
		{
			if (other)
				other->incReference();
			if (mObject)
				mObject->decReference();
			mObject = const_cast<T*>(other.get());
			return *this;
		}

		// 'const' is required because operator= must have this signature.
		ref& operator=(const T* other)
		{
			if (other)
				other->incReference();
			if (mObject)
				mObject->decReference();
			mObject = const_cast<T*>(other);
			return *this;
		}

		// 'const' is required because operator= must have this signature.
		template<class T2> ref& operator=(const ref<T2>& other)
		{
			if (other)
				other->incReference();
			if (mObject)
				mObject->decReference();
			mObject = const_cast<T2*>(other.get());
			return *this;
		}

		void swap(ref& other)
		{
			T* tmp = other.mObject;
			other = mObject;
			mObject = tmp;
		}

		//! This is mainly useful when using ref<> with std::map, std::set, etc.
		T* get_writable() const { return mObject; }

		const T* get() const { return mObject; }
		const T* operator->() const { VL_CHECK(mObject); return mObject; }
		const T& operator*() const { VL_CHECK(mObject); return *mObject; }

		T* get() { return mObject; }
		T* operator->() { VL_CHECK(mObject); return mObject; }
		T& operator*() { VL_CHECK(mObject); return *mObject; }

		bool operator<(const ref& other) const { return mObject < other.mObject; }

		operator bool() const { return mObject != NULL; }

	protected:
		T* mObject;
	};
	// interaction with the other types
	template<class T1, class T2> inline bool operator==(const ref<T1> & o1, const ref<T2> & o2) { return o1.get() == o2.get(); }
	template<class T1, class T2> inline bool operator!=(const ref<T1> & o1, const ref<T2> & o2) { return o1.get() != o2.get(); }
	template<class T1, class T2> inline bool operator==(const ref<T1> & o1, T2 * o2) { return o1.get() == o2; }
	template<class T1, class T2> inline bool operator!=(const ref<T1> & o1, T2 * o2) { return o1.get() != o2; }
	template<class T1, class T2> inline bool operator==(T1 * o1, const ref<T2> & o2) { return o1 == o2.get(); }
	template<class T1, class T2> inline bool operator!=(T1 * o1, const ref<T2> & o2) { return o1 != o2.get(); }
}