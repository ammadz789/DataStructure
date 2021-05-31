

#define MAX_CAPACITY 512


struct memory_block{

	memory_block* right; 
	memory_block* left; 
	bool used; 
	int size; 
	int starting_address; 

	memory_block()         //default constructor
	{
		right = nullptr;
		left = nullptr;
		used = false;
		size = 0;
		starting_address = 0;
	}
	
	memory_block(memory_block *r, memory_block *l, bool u, int s, int s_a )   //constructor
	{
		right = r;
		left = l;
		used = u;
		size = s;
		starting_address = s_a;

	}
};

class My_heap{

	private:
		memory_block* heap_begin;
		memory_block* blk;
		int used_bytes;

	public:
		My_heap();
		~My_heap();
		memory_block* bump_allocate(int num_bytes);
		memory_block* first_fit_allocate(int num_bytes);
		memory_block* best_fit_allocate(int num_bytes);
		memory_block* first_fit_split_allocate(int num_bytes);
		void deallocate(memory_block* block_address);
		void print_heap();
		float get_fragmantation();
		
};












