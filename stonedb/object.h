#ifndef __PROJECT_STONE_OCI20_OBJECT_H__
#define __PROJECT_STONE_OCI20_OBJECT_H__

namespace Oci20 {

	class Object {
	public:
		virtual void Close(bool purge = false) = 0;

	protected:
		Object();
		virtual ~Object();

	private:
		// prohibited
		Object(const Object&);
		void operator = (const Object&);
	};

}
#endif // __PROJECT_STONE_OCI20_OBJECT_H__
