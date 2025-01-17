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

    sc_lambda.cpp - implementation of dynamically created lambdas

    Original Author: Stan Y. Liao. Synopsys, Inc. (stanliao@synopsys.com)

******************************************************************************/

/******************************************************************************

    MODIFICATION LOG - modifiers, enter your name, affliation and
    changes you are making here:

    Modifier Name & Affiliation:
    Description of Modification:
    

******************************************************************************/


#include <stdio.h>
#ifndef _MSC_VER
#include <iostream>
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
#include <assert.h>
#include "sc_lambda.h"

sc_lambda sc_lambda::dummy_lambda;

sc_lambda::sc_lambda( sc_lambda_rator_e op,
                      sc_lambda_rand*   o1,
                      sc_lambda_rand*   o2 )
    : rator_ty(op), op1(o1), op2(o2)
{
    ref_count = 0;
}

sc_lambda::sc_lambda( const sc_signal_edgy& b )
    : rator_ty(SC_LAMBDA_BOOL), op1(new sc_lambda_rand(b)), op2(0)
{
    ref_count = 0;
}

// This version should be used for creating the dummy_lambda only
sc_lambda::sc_lambda()
    : rator_ty(SC_LAMBDA_FALSE), op1(0), op2(0)
{
    static int one = 0;

    // Make sure this constructor doesn't 
    assert(one == 0);
    one++;
    // Make sure it doesn't get deleted
    ref_count = 1;
}

sc_lambda::~sc_lambda()
{
    assert((ref_count == 0) || (this == &dummy_lambda));
    delete op1;
    delete op2;
}

bool
sc_lambda::eval() const
{
    // The compiler should optimize the switch statement to a computed
    // branch.  Otherwise, we should create a table of functions to
    // speed things up.

    switch (rator_ty) {

    /* relational operators for sc_logic type */
    case SC_LAMBDA_SUL_EQ:
        return (op1->sc_logic_read() == op2->sc_logic_read());
    case SC_LAMBDA_SUL_NE:
        return (op1->sc_logic_read() != op2->sc_logic_read());

    /* relational operators for integer type */
    case SC_LAMBDA_INT_EQ:
        return (op1->int_read() == op2->int_read());
    case SC_LAMBDA_INT_NE:
        return (op1->int_read() != op2->int_read());
    case SC_LAMBDA_INT_LE:
        return (op1->int_read() <= op2->int_read());
    case SC_LAMBDA_INT_GE:
        return (op1->int_read() >= op2->int_read());
    case SC_LAMBDA_INT_LT:
        return (op1->int_read() < op2->int_read());
    case SC_LAMBDA_INT_GT:
        return (op1->int_read() > op2->int_read());

    /* boolean operators */
    case SC_LAMBDA_AND:
        return (op1->bool_read() && op2->bool_read());
    case SC_LAMBDA_OR:
        return (op1->bool_read() || op2->bool_read());
    case SC_LAMBDA_NOT:
        return (! op1->bool_read());
    case SC_LAMBDA_BOOL:
        return op1->bool_read();
    case SC_LAMBDA_BOOL_EQ:
        return (op1->bool_read() == op2->bool_read());
    case SC_LAMBDA_BOOL_NE:
        return (op1->bool_read() != op2->bool_read());

    case SC_LAMBDA_FALSE:
        return false;

    case SC_LAMBDA_TRUE:
        return true;

    default:
        cerr << "sc_lambda::eval(): operator 0x" << hex << int(rator_ty)
             << "does not return boolean" << endl;
        return false;
    }
}

void
sc_lambda::replace_ports( void (*replace_fn)(sc_port_manager*, sc_lambda_rand*),
                          sc_port_manager* port_manager )
{
    if (op1) op1->replace_ports( replace_fn, port_manager );
    if (op2) op2->replace_ports( replace_fn, port_manager );
}

int
sc_lambda::int_eval() const
{
    switch (rator_ty) {

    case SC_LAMBDA_INT_ADD:
        return (op1->int_read() + op2->int_read());
    case SC_LAMBDA_INT_SUB:
        return (op1->int_read() - op2->int_read());
    case SC_LAMBDA_INT_MUL:
        return (op1->int_read() * op2->int_read());
    case SC_LAMBDA_INT_DIV:
        return (op1->int_read() / op2->int_read());
    case SC_LAMBDA_INT_REM:
        return (op1->int_read() % op2->int_read());
    case SC_LAMBDA_INT_BITAND:
        return (op1->int_read() & op2->int_read());
    case SC_LAMBDA_INT_BITOR:
        return (op1->int_read() | op2->int_read());
    case SC_LAMBDA_INT_BITNOT:
        return (~ op1->int_read());
    case SC_LAMBDA_INT_BITXOR:
        return (op1->int_read() ^ op2->int_read());
       
    default:
        cerr << "sc_lambda::int_eval(): rator_ty 0x"
             << hex << int(rator_ty) << "does not return int" << endl;
        return 0;
    }
}

sc_logic
sc_lambda::sc_logic_eval() const
{
    switch (rator_ty) {

    case SC_LAMBDA_SUL_BITAND:
        return (op1->sc_logic_read() & op2->sc_logic_read());

    case SC_LAMBDA_SUL_BITOR:
        return (op1->sc_logic_read() | op2->sc_logic_read());

    case SC_LAMBDA_SUL_BITNOT:
        return (~ op1->sc_logic_read());

    case SC_LAMBDA_SUL_BITXOR:
        return (op1->sc_logic_read() ^ op2->sc_logic_read());

    default:
        cerr << "sc_lambda::sc_logic_eval(): rator_ty 0x"
             << int(rator_ty) << "does not return sc_logic\n" << endl;
        return sc_logic();
    }
}

/*****************************************************************************/

static const char* sc_lambda_rand_names[] = {
    "SC_LAMBDA_RAND_LAMBDA",
    "SC_LAMBDA_RAND_SIGNAL_INT",
    "SC_LAMBDA_RAND_SIGNAL_SUL",
    "SC_LAMBDA_RAND_SIGNAL_BOOL",
    "SC_LAMBDA_RAND_INT",
    "SC_LAMBDA_RAND_SUL",
    "SC_LAMBDA_RAND_BOOL"
};

sc_lambda_rand::~sc_lambda_rand()
{
#if 0
  	if (SC_LAMBDA_RAND_LAMBDA == rand_ty) {
	        ((sc_lambda_ptr*) lamb_space)->~sc_lambda_ptr();
	} else if (SC_LAMBDA_RAND_SUL == rand_ty) {
	        ((sc_logic*) ch_space)->~sc_logic();
	}
#endif
    switch( rand_ty ) {

    case SC_LAMBDA_RAND_LAMBDA:{
        ((sc_lambda_ptr*) lamb_space)->~sc_lambda_ptr();
        break;
    }
    case SC_LAMBDA_RAND_SUL:{
        ((sc_logic*) ch_space)->~sc_logic();
	break;
    }
    default:
      break;
    }
    return;
}

int
sc_lambda_rand::int_read() const
{
    switch( rand_ty ) {

    case SC_LAMBDA_RAND_INT:
        return val;

    case SC_LAMBDA_RAND_SIGNAL_INT:
        return int_sig->read();

    case SC_LAMBDA_RAND_LAMBDA:
        return (*((sc_lambda_ptr*) lamb_space))->int_eval();

    default:
        cerr << "sc_lambda_rand::int_read(): operand "
             << sc_lambda_rand_names[rand_ty]
             << "is not integer." << endl;
        return 0;
    }
}

sc_logic
sc_lambda_rand::sc_logic_read() const
{
    switch (rand_ty) {

    case SC_LAMBDA_RAND_SUL:
        return *((sc_logic*) ch_space);

    case SC_LAMBDA_RAND_SIGNAL_SUL:
        return sul_sig->read();

    case SC_LAMBDA_RAND_LAMBDA:
        return (*((sc_lambda_ptr*) lamb_space))->sc_logic_eval();

    default:
        cerr << "sc_lambda_rand::sc_logic_read(): operand "
             << sc_lambda_rand_names[rand_ty]
             << "is not sc_logic." << endl;
        return sc_logic();
    }
}

bool
sc_lambda_rand::bool_read() const
{
    switch (rand_ty) {

    case SC_LAMBDA_RAND_BOOL:
    case SC_LAMBDA_RAND_INT:
        return (val != 0);

    case SC_LAMBDA_RAND_SIGNAL_BOOL:
        return edgy_sig->edgy_read();

    case SC_LAMBDA_RAND_LAMBDA:
        return (*((sc_lambda_ptr*) lamb_space))->eval();

    default:
        cerr << "sc_lambda_rand::bool_read(): operand "
             << sc_lambda_rand_names[rand_ty]
             << "is not bool." << endl;
        return false;
    }
}

void
sc_lambda_rand::replace_ports(  void (*replace_fn)(sc_port_manager*, sc_lambda_rand*),
                                sc_port_manager* port_manager )
{
    switch (rand_ty) {
    case SC_LAMBDA_RAND_LAMBDA:
        (*((sc_lambda_ptr*) lamb_space))->replace_ports( replace_fn, port_manager );
        break;
    case SC_LAMBDA_RAND_SIGNAL_BOOL:
        (*replace_fn)(port_manager, this);
        break;
    default:
        break;
    }
}
