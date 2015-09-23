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

    count = clock();

    for( int i = 0; i < size; ++i )
        t.insert( rand() % size, i );

    std::cout << size << " randomly nodes inserted in "
              << clock() - count << " clocks" << std::endl;

    assert( t.size() == size );
    assert( t.valid() );

    count = clock();

    for( int i = 0; i < size; ++i )
        t.remove( rand() % size );

    std::cout << size - t.size() << " nodes randomly removed in "
              << clock() - count << " clocks" << std::endl;
    assert( t.valid() );

    count = clock();

    int r_size = t.size();
    t.clear();

    std::cout << r_size << " nodes cleared in " << clock() - count << " clocks" << std::endl;

    assert( t.size() == 0 );
    assert( t.valid() );

    count = clock();

    for( int i = 0; i < size; ++i )
        t.insert( rand() % size, i );

    auto it = t.begin();
    for( int i = 0; i < size; ++i ) {
        assert( t.getNth( i ) == it++ );
    }

    std::cout << size << " nodes order validated in " << clock() - count << " clocks" << std::endl;

    count = clock();

    int nth = 15;
    int repeat_count = 1e6;
    for( int repeat = 0; repeat < repeat_count; ++repeat ) {
        it = t.begin();
        for( int i = 0; i < nth; ++i )
            ++it;
    }

    std::cout << repeat_count << " nodes founds in " << clock() - count << " clocks" << std::endl;

    count = clock();

    for( int repeat = 0; repeat < repeat_count; ++repeat ) {
        assert( it == ( t.getNth( 0 ) + nth ) );
    }

    std::cout << repeat_count << " nodes founds in " << clock() - count << " clocks" << std::endl;

    return 0;
}
