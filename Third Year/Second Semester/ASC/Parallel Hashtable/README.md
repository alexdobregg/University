Name: Dobre Gigi-Alexandru
Group: 341C5

# Homework 3 - Parallel Hashtable - 2023

Solution
-
* The solution starts from the given skel, to which I added the Hashtable and Entry structures.
* After every call to one of the cuda functions, there is a verification that it has been executed successfully.
* For the linear probing, I had to use the hash = (hash + 1) % size because otherwise there would be an out of bounds memory access in most of the cases due do hash becoming greater than the size of entries.
* The constructor is used to allocate memory and to set the current size to 0 and maximum size to the given size. The destructor frees the memory allocated for the entries.
* To implement the hash function I've used the Murmur Hash algorithm. The len parameter was disregarded and the seed was set to a random value.
* reshape -> it allocates the memory for the new array of entries that have a bigger capacity and then copies the keys and values from the current array of entries, freeing the memory in the end (it uses reshape_kernel).
* insertBatch -> it allocates the memory for the necessary variables; if the capacity after adding the number of elements to be inserted is greater than the maximum capacity, then the array is reshaped to have more space; then for each of the elements to be inserted, it verifies if the key is new or it exists already: for new, the key-value pair is inserted and for a key that exists, the value is updated and from the current size is subtracted 1 (it uses kernel_insert).
* get -> it allocates the memory for the necessary variables; a search is done through the entries and the values for the specified keys are returned (it uses kernel_get).

Hashtable representation
-
* Entry: a structure having key and value variables - this represents the key-value pair
* Hashtable: a structure having an array of entries, current size and maximum size - this represents the hashtable

Tests
-
------- Test T1 START   ----------

* HASH_BATCH_INSERT   count: 500000           speed: 97M/sec          loadfactor: 70%
* HASH_BATCH_INSERT   count: 500000           speed: 84M/sec          loadfactor: 70%
* HASH_BATCH_GET      count: 500000           speed: 120M/sec         loadfactor: 70%
* HASH_BATCH_GET      count: 500000           speed: 116M/sec         loadfactor: 70%
* AVG_INSERT: 90 M/sec,   AVG_GET: 118 M/sec,     MIN_SPEED_REQ: 0 M/sec

------- Test  T1 END    ----------       [ OK RESULT:  15  pts ]

------- Test T2 START   ----------

* HASH_BATCH_INSERT   count: 1000000          speed: 88M/sec          loadfactor: 70%
* HASH_BATCH_INSERT   count: 1000000          speed: 78M/sec          loadfactor: 70%
* HASH_BATCH_GET      count: 1000000          speed: 99M/sec          loadfactor: 70%
* HASH_BATCH_GET      count: 1000000          speed: 115M/sec         loadfactor: 70%
* AVG_INSERT: 83 M/sec,   AVG_GET: 107 M/sec,     MIN_SPEED_REQ: 20 M/sec

------- Test  T2 END    ----------       [ OK RESULT:  15  pts ]

------- Test T3 START   ----------

* HASH_BATCH_INSERT   count: 1000000          speed: 88M/sec          loadfactor: 70%
* HASH_BATCH_INSERT   count: 1000000          speed: 78M/sec          loadfactor: 70%
* HASH_BATCH_INSERT   count: 1000000          speed: 69M/sec          loadfactor: 70%
* HASH_BATCH_INSERT   count: 1000000          speed: 34M/sec          loadfactor: 93%
* HASH_BATCH_GET      count: 1000000          speed: 145M/sec         loadfactor: 93%
* HASH_BATCH_GET      count: 1000000          speed: 134M/sec         loadfactor: 93%
* HASH_BATCH_GET      count: 1000000          speed: 134M/sec         loadfactor: 93%
* HASH_BATCH_GET      count: 1000000          speed: 75M/sec          loadfactor: 93%
* AVG_INSERT: 67 M/sec,   AVG_GET: 122 M/sec,     MIN_SPEED_REQ: 40 M/sec

------- Test  T3 END    ----------       [ OK RESULT:  15  pts ]

------- Test T4 START   ----------

* HASH_BATCH_INSERT   count: 20000000         speed: 129M/sec         loadfactor: 70%
* HASH_BATCH_INSERT   count: 20000000         speed: 109M/sec         loadfactor: 70%
* HASH_BATCH_INSERT   count: 20000000         speed: 91M/sec          loadfactor: 70%
* HASH_BATCH_INSERT   count: 20000000         speed: 27M/sec          loadfactor: 93%
* HASH_BATCH_GET      count: 20000000         speed: 141M/sec         loadfactor: 93%
* HASH_BATCH_GET      count: 20000000         speed: 140M/sec         loadfactor: 93%
* HASH_BATCH_GET      count: 20000000         speed: 102M/sec         loadfactor: 93%
* HASH_BATCH_GET      count: 20000000         speed: 27M/sec          loadfactor: 93%
* AVG_INSERT: 89 M/sec,   AVG_GET: 103 M/sec,     MIN_SPEED_REQ: 50 M/sec

------- Test  T4 END    ----------       [ OK RESULT:  15  pts ]

------- Test T5 START   ----------

* HASH_BATCH_INSERT   count: 50000000         speed: 129M/sec         loadfactor: 69%
* HASH_BATCH_INSERT   count: 50000000         speed: 104M/sec         loadfactor: 69%
* HASH_BATCH_GET      count: 50000000         speed: 141M/sec         loadfactor: 69%
* HASH_BATCH_GET      count: 50000000         speed: 110M/sec         loadfactor: 69%
* AVG_INSERT: 116 M/sec,  AVG_GET: 125 M/sec,     MIN_SPEED_REQ: 50 M/sec

------- Test  T5 END    ----------       [ OK RESULT:  10  pts ]

------- Test T6 START   ----------

* HASH_BATCH_INSERT   count: 10000000         speed: 128M/sec         loadfactor: 70%
* HASH_BATCH_INSERT   count: 10000000         speed: 108M/sec         loadfactor: 70%
* HASH_BATCH_INSERT   count: 10000000         speed: 92M/sec          loadfactor: 70%
* HASH_BATCH_INSERT   count: 10000000         speed: 35M/sec          loadfactor: 93%
* HASH_BATCH_INSERT   count: 10000000         speed: 69M/sec          loadfactor: 69%
* HASH_BATCH_INSERT   count: 10000000         speed: 57M/sec          loadfactor: 84%
* HASH_BATCH_INSERT   count: 10000000         speed: 4M/sec           loadfactor: 97%
* HASH_BATCH_INSERT   count: 10000000         speed: 44M/sec          loadfactor: 70%
* HASH_BATCH_INSERT   count: 10000000         speed: 65M/sec          loadfactor: 78%
* HASH_BATCH_INSERT   count: 10000000         speed: 33M/sec          loadfactor: 87%
* HASH_BATCH_GET      count: 10000000         speed: 138M/sec         loadfactor: 87%
* HASH_BATCH_GET      count: 10000000         speed: 139M/sec         loadfactor: 87%
* HASH_BATCH_GET      count: 10000000         speed: 138M/sec         loadfactor: 87%
* HASH_BATCH_GET      count: 10000000         speed: 137M/sec         loadfactor: 87%
* HASH_BATCH_GET      count: 10000000         speed: 138M/sec         loadfactor: 87%
* HASH_BATCH_GET      count: 10000000         speed: 138M/sec         loadfactor: 87%
* HASH_BATCH_GET      count: 10000000         speed: 136M/sec         loadfactor: 87%
* HASH_BATCH_GET      count: 10000000         speed: 90M/sec          loadfactor: 87%
* HASH_BATCH_GET      count: 10000000         speed: 64M/sec          loadfactor: 87%
* HASH_BATCH_GET      count: 10000000         speed: 33M/sec          loadfactor: 87%
* AVG_INSERT: 64 M/sec,   AVG_GET: 115 M/sec,     MIN_SPEED_REQ: 50 M/sec

------- Test  T6 END    ----------       [ OK RESULT:  10  pts ]

Total: 80 / 80

Problems encountered:
-
* Reshaping the memory on insert: the problem was that I was using the same load factors for min and max as in the test file (0.5, 1.0) and I was getting bad performance and memory issues. Solved it by changing the max load factor to 0.7f due to this factor being generally used as 0.7f.
* Problem with the updates: initially I didn't take into considerations we can update the value for some keys and it was causing issues. Solved it by counting how many updates there are and substracting them after the device synchronization.

Conclusions
-
* I've used the Murmur Hash because of its performance which is know to be high when operating with big data, being based on multiple operations such as multiplication, bit shifting and XOR. It does well with collisions despite it not being a cryptographic hash function.
* For the tests, the results are satisfactory, with the average speed for GET always being higher than 100 M/sec. Also, the average speed for INSERT is good as well, being over 60 M/sec everytime.

Resources
-
* https://ocw.cs.pub.ro/courses/asc/teme/tema3
* https://ocw.cs.pub.ro/courses/asc/laboratoare/07
* https://ocw.cs.pub.ro/courses/asc/laboratoare/08
* https://ocw.cs.pub.ro/courses/asc/laboratoare/09
* https://en.wikipedia.org/wiki/MurmurHash
* https://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__MEMORY.html
* http://docs.seqan.de/seqan1/1.3/FUNCTION.atomic_Cas.html

