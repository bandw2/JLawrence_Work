#pragma once

template <typename TYPE>
class Vector {
	TYPE * data;
	unsigned int size, allocated;
	void setAllocatedSize(const unsigned int a_newSize) {//Change Size
		TYPE * newArr = new TYPE[a_newSize];
		if(data) {
			for(unsigned int i = 0; i < size && i < a_newSize; ++i) {
				newArr[i] = data[i];
			}
			delete [] data;
		}
		data = newArr;
		allocated = a_newSize;
	}
public:
	Vector():data(0),size(0),allocated(0){}
	TYPE get(const int a_index) const {//random access
		return data[a_index];
	}
	void set(const int a_index, const TYPE a_value) {//random set
		data[a_index] = a_value;
	}
	int getSize() const {	return size;	}//Size func
	void add(TYPE a_value) {//Add a new item
		if(size >= allocated) {
			setAllocatedSize(2*allocated);//double size
		}
		data[size++] = a_value;
	}
	Vector(Vector & v):data(0),size(0),allocated(0){//Copy Constructor
		setAllocatedSize(v.size);
		for(unsigned int i = 0; i < v.size; ++i) { data[i] = v.data[i]; }
		size = v.size;
	}
	void release() {//Empty out func/hard reset
		if(data) { delete [] data; }
		size = 0; allocated = 0;
	}
	~Vector() {	release(); }
	Vector & operator=(Vector & v) {//assignment operator
		release();
		this->Vector::Vector(v);
		return *this;
	}
	// "move" constructor
	Vector(Vector && v)//move to v ...?
		:data(v.data),size(v.size),allocated(v.allocated) {
		v.data = 0;
		v.size = 0;
		v.allocated = 0;
	}
	TYPE & operator[](const int a_index) { return data[a_index]; }
};