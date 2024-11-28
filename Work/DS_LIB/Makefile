TARGET= libds.so
CC= gcc
CFLAGS= -g -pedantic -fPIC
OBJS= dynamicVector.o new_gen_dlist.o hashmap.o stack.o genQueue.o BSTree.o genheap.o

VECTOR_PATH=../DynamicVector/
HEAP_PATH=../Heap/
DLIST_PATH=../DoubleLinkedList/
QUEUE_PATH=../Queue/
STACK_PATH=../Stack/
HASH_MAP_PATH=../HashMap/
BSTREE_PATH=../BSTree/

.PHONY: clean

$(TARGET): $(OBJS)
		$(CC) $(CFLAGS) -shared -o $(TARGET) $(OBJS)


dynamicVector.o: $(VECTOR_PATH)dynamicVector.c $(VECTOR_PATH)dynamicVector.h
		$(CC) $(CFLAGS) -c $(VECTOR_PATH)dynamicVector.c

new_gen_dlist.o: $(DLIST_PATH)new_gen_dlist.c $(DLIST_PATH)new_gen_dlist.h
		$(CC) $(CFLAGS) -c $(DLIST_PATH)new_gen_dlist.c

hashmap.o: $(HASH_MAP_PATH)hashmap.c $(HASH_MAP_PATH)hashmap.h $(DLIST_PATH)new_gen_dlist.h
		$(CC) $(CFLAGS) -c $(HASH_MAP_PATH)hashmap.c

stack.o: $(STACK_PATH)stack.c $(STACK_PATH)stack.h $(STACK_PATH)dynamicVector.h
		$(CC) $(CFLAGS) -c $(STACK_PATH)stack.c

genQueue.o: $(QUEUE_PATH)genQueue.c $(QUEUE_PATH)genQueue.h
		$(CC) $(CFLAGS) -c $(QUEUE_PATH)genQueue.c

genheap.o: $(HEAP_PATH)genheap.c $(HEAP_PATH)genheap.h $(VECTOR_PATH)dynamicVector.h
		$(CC) $(CFLAGS) -c $(HEAP_PATH)genheap.c

BSTree.o: $(BSTREE_PATH)BSTree.c $(BSTREE_PATH)BSTree.h
		$(CC) $(CFLAGS) -c $(BSTREE_PATH)BSTree.c

clean:
		rm -f $(TARGET) $(OBJS)