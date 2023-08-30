#ifndef _HASHCPU_
#define _HASHCPU_

#define LOAD_FACTOR_MIN 0.7f
#define LOAD_FACTOR_MAX 0.5f

struct Entry {
	int key;
	int value;
};

struct Hashtable {
	Entry *entries;
	int max_size;
	int curr_size;
};

/**
 * Class GpuHashTable to implement functions
 */
class GpuHashTable {
private:
	Hashtable hashtable;
public:
	GpuHashTable(int size);
	void reshape(int sizeReshape);

	bool insertBatch(int *keys, int* values, int numKeys);
	int* getBatch(int* key, int numItems);
	
	~GpuHashTable();
};

#endif
