#include <iostream>

#include "statistic_rb_tree.h"

#include <time.h>

int main() {

    RbTree< int, int > t;

    clock_t count = clock();

    std::cout << std::endl << "begin tests" << std::endl;

    int size = 1e6;
    for( int i = 0; i < size; ++i )
        t.insert( 1, i );

    std::cout << size << " nodes inserted in " << clock() - count << " clocks" << std::endl;

    assert( t.size() == size );
    assert( t.valid() );

    count = clock();

    for( int i = 0; i < size / 2; ++i )
        t.remove( 1 );

    std::cout << size / 2 << " nodes removed in " << clock() - count << " clocks" << std::endl;

    assert( t.size() == size / 2 );
    assert( t.valid() );

    count = clock();

    t.clear();

    std::cout << size / 2 << " nodes cleared in " << clock() - count << " clocks" << std::endl;

    assert( t.size() == 0 );
    assert( t.valid() );

    for( int i = 0; i < size; ++i )
        t.insert( rand() % size, i );

    std::cout << size << " nodes inserted in " << clock() - count << " clocks" << std::endl;

    assert( t.size() == size );
    assert( t.valid() );

    count = clock();

    for( int i = 0; i < size; ++i )
        t.remove( rand() % size );

    std::cout << size - t.size() << " nodes removed in " << clock() - count << " clocks" << std::endl;
    assert( t.valid() );

    count = clock();

    int r_size = t.size();
    t.clear();

    std::cout << r_size << " nodes cleared in " << clock() - count << " clocks" << std::endl;

    assert( t.size() == 0 );
    assert( t.valid() );


    return 0;
}
