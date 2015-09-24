#include <iostream>

#include "statistic_rb_tree.h"

#include <time.h>

template< class T >
void test_const( const T& t ) {
    for( auto i = t.begin(); i != t.end(); ++i ) {
        std::cout << i.key() << std::endl;
    }
}

int main() {

    OrderStatisticTree< int, int, std::greater< int > > t;

    test_const( t );

    clock_t count = clock();

    std::cout << std::endl << "begin tests" << std::endl;

    int size = 1e6;
    for( int i = 0; i < size; ++i )
        t.insertMulti( 1, i );

    std::cout << size << " nodes inserted in " << clock() - count << " clocks" << std::endl;

    assert( t.size() == size );
    assert( t.valid() );

    count = clock();

    for( int i = 0; i < size / 2; ++i )
        t.removeOne( 1 );

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
        t.insertMulti( rand() % size, i );

    std::cout << size << " randomly nodes inserted in "
              << clock() - count << " clocks" << std::endl;

    assert( t.size() == size );
    assert( t.valid() );

    count = clock();

    for( int i = 0; i < size; ++i )
        t.removeOne( rand() % size );

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
        t.insertMulti( rand() % size, i );

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

    std::cout << "all tests passed" << std::endl << std::endl;

    t.clear();

    t.insertMulti( 1, 1 );
    t.insertMulti( 2, 1 );
    assert( t.erase( t.find( 2 ) ).key() == 1 );
    assert( t.size() == 1 );

    return 0;
}
