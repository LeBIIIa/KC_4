/******************************************************************************
    Copyright (c) 1996-2000 Synopsys, Inc.    ALL RIGHTS RESERVED

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC(TM) Open Community License Software Download and
  Use License Version 1.1 (the "License"); you may not use this file except
  in compliance with such restrictions and limitations. You may obtain
  instructions on how to receive a copy of the License at
  http://www.systemc.org/. Software distributed by Original Contributor
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

******************************************************************************/

/******************************************************************************

    sc_logic_vector.h -- implementation of types sc_logic_vector and
    sc_signal_logic_vector, much like HDLs.

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#ifndef SC_LOGIC_VECTOR_H
#define SC_LOGIC_VECTOR_H

#ifndef _MSC_VER
#include <iostream>
 #ifdef __BCPLUSPLUS__
  #pragma hdrstop
 #endif
using std::ostream;
using std::istream;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::hex;
#else
// MSVC6.0 has bugs in standard library
#include <iostream.h>
#endif

#include "sc_array.h"
#include "sc_signal_array.h"
#include "sc_logic.h"
#include "numeric_bit/sc_nbdefs.h"

class sc_logic_vector;
class sc_signal_logic_vector;

typedef sc_logic_vector sulv;
typedef sc_signal_logic_vector signal_sulv;

class sc_unsigned;
class sc_signed;
template<class A, class T> class sc_channel_array_gnu;

namespace sc_bv_ns
{
  class sc_bv_base;
  class sc_lv_base;
}
//
//  <summary> Array of sc_logics </summary>
//
//  sc_logic_vector is derived from sc_array&lt;sc_logic&gt;.  In addition
//  to inherited functionality, it also provides bitwise operators as well as
//  operations involving char strings.
//
class sc_logic_vector : public sc_array<sc_logic> {
    friend class sc_bool_vector;
    friend class sc_unsigned;
    friend class sc_signed;
    friend class sc_2d< sc_logic_vector >;
    friend class sc_channel_array_gnu<sc_logic_vector, sc_logic>;

    friend sulv operator&( const sulv&, const sulv& );
    friend sulv operator&( const sulv&, const char* );
    friend sulv operator&( const sulv&, const sc_bool_vector& );
    friend sulv operator|( const sulv&, const sulv& );
    friend sulv operator|( const sulv&, const char* );
    friend sulv operator|( const sulv&, const sc_bool_vector& );
    friend sulv operator^( const sulv&, const sulv& );
    friend sulv operator^( const sulv&, const char* );
    friend sulv operator^( const sulv&, const sc_bool_vector& );
    friend sulv operator~( const sulv& a );

    friend const sulv& operator,( const sulv&, const char* );
    friend const sulv& operator,( const char*, const sulv& );

public:
        // Constructors
    explicit sc_logic_vector( int len );
    sc_logic_vector( int len, const sc_logic_vector& ref );
    sc_logic_vector( const sc_logic_vector& y );

protected:
    sc_logic_vector( sc_logic** data_ptrs, int len );

public:
        // Destructor
    ~sc_logic_vector();

        /*
         * Note that defining addtional operator=(const char*), &c.,
         * clobbers the definitions in the base class (Stroustrup p.587).
         * Hence, we need to redo these operators.
         */  

        // Various operators with different types of operands
        // <group>
    sc_logic_vector& operator=( const sc_logic_vector& nv );
    sc_logic_vector& operator=( const sc_bool_vector& nv );
    sc_logic_vector& operator=( const char* nv );
    sc_logic_vector& operator=( const sc_logic* nv );
    sc_logic_vector& operator=( const sc_unsigned& nv );
    sc_logic_vector& operator=( const sc_signed& nv );
    sc_logic_vector& operator=( long nv );
    sc_logic_vector& operator=( unsigned long nv );
    sc_logic_vector& operator=( int nv )      { return operator=(long(nv)); }
    sc_logic_vector& operator=( unsigned nv ) { return operator=((unsigned long) nv); }
    sc_logic_vector& operator=( int64 nv );
    sc_logic_vector& operator=( uint64 nv);
    sc_logic_vector& operator=( const sc_bv_ns::sc_bv_base&);
    sc_logic_vector& operator=( const sc_bv_ns::sc_lv_base&);


    bool operator==( const sc_logic_vector& rhs ) const;
    bool operator==( const char* rhs ) const;
    bool operator==( const sc_bool_vector& rhs ) const;
    bool operator!=( const sc_logic_vector& rhs ) const;
    bool operator!=( const char* rhs ) const;
    bool operator!=( const sc_bool_vector& rhs ) const;
        // </group>

    sc_logic and_reduce() const;
#if defined(__BCPLUSPLUS__)
#pragma warn -8027
#endif
    sc_logic nand_reduce() const { return (char)(!and_reduce()); }
    sc_logic or_reduce() const;
    sc_logic nor_reduce() const  { return (char)(!or_reduce()); }
    sc_logic xor_reduce() const;
    sc_logic xnor_reduce() const { return (char)(!xor_reduce()); }
#if defined(__BCPLUSPLUS__)
#pragma warn .8027
#endif

        /* operator[](int i) inherited from sc_array<sc_logic> */

        // Returns the subarray indexed by logical indices i and j
    DECLARE_PARTSEL(sc_logic_vector)

public:
        // Various op= bitwise operators
        // <group>
    sc_logic_vector& operator&=( const sc_logic_vector& rhs );
    sc_logic_vector& operator&=( const sc_bool_vector& rhs );
    sc_logic_vector& operator&=( const char* rhs );
    sc_logic_vector& operator|=( const sc_logic_vector& rhs );
    sc_logic_vector& operator|=( const sc_bool_vector& rhs );
    sc_logic_vector& operator|=( const char* rhs );
    sc_logic_vector& operator^=( const sc_logic_vector& rhs );
    sc_logic_vector& operator^=( const sc_bool_vector& rhs );
    sc_logic_vector& operator^=( const char* rhs );
    sc_logic_vector& negate();
        // </group>

        // Conversion to character string
    const char* to_string() const;
    void to_string( char* buffer ) const;

    void print(ostream& os) const;
    void dump(ostream& os) const;

    int to_signed() const;
    unsigned to_unsigned() const;

protected:
        // "Virtual constructor" required by sc_array_base
    sc_array_base* vctor( const sc_array_base* p, int i, int j ) const;

        // Actual constructor called by the virtual constructor
    sc_logic_vector( const sc_array_base* p, int i, int j );

    sc_logic_vector( sc_logic* initialized_data, int len );

    sc_logic_vector( sc_logic_vector* v1, sc_logic_vector* v2 );
    sc_logic_vector( sc_logic_vector* v1, sc_logic* v2 );
    sc_logic_vector( sc_logic* v1, sc_logic_vector* v2 );
    sc_logic_vector( sc_logic* v1, sc_logic* v2 );
    sc_logic_vector( const sc_logic_vector* v1, const char* v2 );
    sc_logic_vector( const char* v1, const sc_logic_vector* v2 );

    void assign_concat_with_string( const sc_logic_vector* v1, const char* v2 );
    void assign_concat_with_string( const char* v1, const sc_logic_vector* v2 );

private:
    mutable char* str;

public:
    static sc_logic_vector* get_concat_array( sc_logic_vector*, sc_logic_vector* );
    static sc_logic_vector* get_concat_array( sc_logic_vector*, sc_logic* );
    static sc_logic_vector* get_concat_array( sc_logic*, sc_logic_vector* );
    static sc_logic_vector* get_concat_array( sc_logic*, sc_logic* );
    static const sc_logic_vector* get_concat_array( sc_logic_vector*, const char* );
    static const sc_logic_vector* get_concat_array( const char*, sc_logic_vector* );

protected:
    static sc_array_base* new_array_vv( sc_array_base* x, sc_array_base* y );
    static sc_array_base* new_array_vs( sc_array_base* x, void* y );
    static sc_array_base* new_array_sv( void* x, sc_array_base* y );
    static sc_array_base* new_array_ss( void* x, void* y );
    static sc_array_base* new_array_vc( sc_array_base* x, const char* y );
    static sc_array_base* new_array_cv( const char* y, sc_array_base* x );

private:
    typedef const char* asgn_type1;
    typedef const sc_logic* asgn_type2;
    class asgn_type3{ }; sc_logic_vector& operator=(const asgn_type3&) { return *this; }
    class asgn_type4{ }; sc_logic_vector& operator=(const asgn_type4&) { return *this; }
    class asgn_type5{ }; sc_logic_vector& operator=(const asgn_type5&) { return *this; }

public:
    typedef sc_logic elemtype;
};

extern sc_logic_vector& operator,( sc_logic_vector&, sc_logic_vector& );
extern sc_logic_vector& operator,( sc_logic_vector&, sc_logic& );
extern sc_logic_vector& operator,( sc_logic&, sc_logic_vector& );
extern const sc_logic_vector& operator,( const sc_logic_vector&, const char* );
extern const sc_logic_vector& operator,( const char*, const sc_logic_vector& );

extern const sc_logic_vector& operator,( const sc_logic_vector&, const sc_logic_vector& );
extern const sc_logic_vector& operator,( const sc_logic_vector&, const sc_logic& );
extern const sc_logic_vector& operator,( const sc_logic&, const sc_logic_vector& );
extern const sc_logic_vector& operator,( const sc_logic_vector&, const sc_signal<sc_logic>& );
extern const sc_logic_vector& operator,( const sc_signal<sc_logic>&, const sc_logic_vector& );

/*---------------------------------------------------------------------------*/

//
//  <summary> Signal array of sc_logic </summary>
//
//  This class represents the signal version of sc_logic_vector.  In addition
//  to the functionality derived from sc_logic_vector, this class also has
//  the properties derived from the template class sc_signal_array, primarily
//  the update function.  No new member variables are introduced.
//
class sc_signal_logic_vector : public sc_signal_array< sc_logic_vector >
{
    friend class sc_clock;

public:
    typedef sc_signal_array<sc_logic_vector> templ_type;
    typedef templ_type super;

        // Constructor - takes length of logic vector
    explicit sc_signal_logic_vector( int len );
    sc_signal_logic_vector( const char* nm, int len );
    sc_signal_logic_vector( int len, const sc_signal_logic_vector& ref );

    sc_signal_logic_vector( int len, sc_signal<sc_logic>* (*create_sig)(void *) )
        : sc_signal_array<sc_logic_vector>(len, create_sig)
    {
        /*EMPTY*/
    }

    /* This form of the constructor is used by resolved signal arrays */
    sc_signal_logic_vector( const char *nm, int len, sc_signal<sc_logic>* (*create_sig)(void *) )
        : sc_signal_array<sc_logic_vector>(nm, len, create_sig)
    {
        /*EMPTY*/
    }

protected:
    sc_signal_logic_vector( const sc_signal_logic_vector& );

public:
        // Destructor
    ~sc_signal_logic_vector();

        /* sc_signal<sc_logic>& operator[](int i) const; inherited. */

        // Returns the subarray indexed by logical indices i and j
    DECLARE_PARTSEL(sc_signal_logic_vector)

protected:
        /* void update(); inherited from 
         * sc_signal_array<sc_logic_vector>
         */

#define DECL_WRITE(T) \
    sc_signal_logic_vector& write(T nv); \
    sc_signal_logic_vector& operator=(T nv) \
    { \
        return write(nv); \
    }

public:
        // Write a future value to the sc_signal_logic_vector
        // <group>
    DECL_WRITE( const sc_array<sc_logic>& )
    sc_signal_logic_vector& operator=( const sc_signal_logic_vector& v )
    {   /* Always remember to define this assignment operator */
        return write( v.read() );
    }
    DECL_WRITE( const sc_logic* )
    DECL_WRITE( const char* )
    DECL_WRITE( const sc_signed& )
    DECL_WRITE( const sc_unsigned& )
    DECL_WRITE( int )
    DECL_WRITE( unsigned )
    DECL_WRITE( int64 )
    DECL_WRITE( uint64 )
    DECL_WRITE( const sc_bool_vector& )
        // </group>

#undef DECL_WRITE

protected:
        // "Virtual constructor" required by sc_array_base
    sc_array_base* vctor( const sc_array_base* p, int i, int j ) const;

        // Actual constructor called by the virtual constructor
    sc_signal_logic_vector( const sc_array_base* p, int i, int j );

private:
        // Not supposed to be used: declared to forbid their use.
        // <group>
    void operator=(void*)  const { assert(false); }
    void operator&=(void*) const { assert(false); }
    void operator|=(void*) const { assert(false); }
    void operator^=(void*) const { assert(false); }
    void negate(void*)     const { assert(false); }
        // </group>

protected:
    sc_signal_logic_vector( signal_sulv*, signal_sulv* );
    sc_signal_logic_vector( signal_sulv*, sc_signal<sc_logic>* );
    sc_signal_logic_vector( sc_signal<sc_logic>*, signal_sulv* );
    sc_signal_logic_vector( sc_signal<sc_logic>*, sc_signal<sc_logic>* );

public:
    static signal_sulv* get_concat_array( signal_sulv*, signal_sulv* );
    static signal_sulv* get_concat_array( signal_sulv*, sc_signal<sc_logic>* );
    static signal_sulv* get_concat_array( sc_signal<sc_logic>*, signal_sulv* );
    static signal_sulv* get_concat_array( sc_signal<sc_logic>*, sc_signal<sc_logic>* );

protected:
    static sc_array_base* new_array_vv( sc_array_base* x, sc_array_base* y );
    static sc_array_base* new_array_vs( sc_array_base* x, void* y );
    static sc_array_base* new_array_sv( void* x, sc_array_base* y );
    static sc_array_base* new_array_ss( void* x, void* y );

public:
    void print(ostream& os) const;
    void dump(ostream& os) const;
    void trace( sc_trace_file* tf ) const;
};

extern signal_sulv& operator,( signal_sulv&, signal_sulv& );
extern signal_sulv& operator,( signal_sulv&, sc_signal<sc_logic>& );
extern signal_sulv& operator,( sc_signal<sc_logic>&, signal_sulv& );

extern const signal_sulv& operator,( const signal_sulv&, const signal_sulv& );
extern const signal_sulv& operator,( const signal_sulv&, const sc_signal<sc_logic>& );
extern const signal_sulv& operator,( const sc_signal<sc_logic>&, const signal_sulv& );

extern ostream& operator<<(ostream&, const sc_logic_vector&);

/*****************************************************************************/

#include "sc_logic_vector_defs.h_"

#endif
