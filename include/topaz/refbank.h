/*
Copyright (c) 2020, Johnathan Corkery. (jcorkery@umich.edu)
All rights reserved.

This file is part of the topaz project (https://github.com/jcorks/topaz)
topaz was released under the MIT License, as detailed below.



Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is furnished 
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall
be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.


*/


#ifndef H_TOPAZDC__REFBANK__INCLUDED
#define H_TOPAZDC__REFBANK__INCLUDED


#include <stdint.h>
#include <topaz/containers/table.h>

///    A simple, generic reference counter
///
///    Refbank represents a generic, multi-instance reference counter.
///    Refbank manages a number of accounts. Each account 
///    signifies a set of references. Accounts are uniquely identified
///    by the user supplied object T and are automatically created once
///    topaz_refbank_deposit is called. Each account has a reference 
///    balance associated with it. Once an account balance has reached 
///    0 or less, the account is removed. If an AccountRemover is set,
///    the AccountRemover callback will be run on the account before removal.
///
typedef struct topazRefbank_t topazRefbank_t;



/// Creates a new, empty refbank 
///
topazRefbank_t * topaz_refbank_create();

/// Destroys a Refbank
///
void topaz_refbank_destroy(
    /// The refbank to destroy.
    topazRefbank_t * bank
);






/// Adds a reference count amount.
/// 
/// If a reference count deposite call would yeild a deposit greater than
/// UINT32_MAX, the account is set to UINT32_MAX. (UINT32_MAX is defined in cstdint.h)
void topaz_refbank_deposit(
    /// The refbank to deposit a reference to.
    topazRefbank_t * bank, 

    /// The object representing the reference to add counts to
    const void * account, 

    /// The reference count amount to add. The default is one.
    uint32_t amount
);

/// Same as topaz_refbank_deposit, except it only enters one reference.
///
void topaz_refbank_increment(
    /// The refbank to modify.
    topazRefbank_t * bank, 

    /// The account within the refbank to increment the count.
    const void * account
);



/// Deduct a reference count amount.
///
/// If a reference withdrawl would result in a balance of 0 or lower, the 
/// AccountRemover is called with the account and is removed.
void topaz_refbank_withdraw(
    /// The refbank to modify.
    topazRefbank_t * bank, 

    /// The object representing the reference to remove counts from.
    const void * account, 

    /// The reference count amount to deduct. The default is one.
    uint32_t amount
);

/// Same as topaz_refbank_widthdraw, except only one reference is deducted
///
void topaz_refbank_decrement(
    /// The refbank to modify.
    topazRefbank_t * bank, 

    /// The object representing the reference to remove counts from.
    const void * account
);






/// Returns the number of reference counts accumulated in the account.
///
/// If the count does not exist, 0 is returned.
uint32_t topaz_refbank_balance(
    /// The refbank to query
    const topazRefbank_t *, 

    /// The account to query.
    const void * account
);


/// Removes all accounts and runs the AccountRemover on all
/// accounts.
/// 
void topaz_refbank_remove_all(
    /// The refbank to modify
    topazRefbank_t * bank
);

/// Function to be called when a refbank's account reaches 0.
typedef void (*topaz_refbank_account_remover)(
    /// The refbank that triggered the event.
    topazRefbank_t * bank, 

    /// The account that triggered the event.
    const void * account, 

    /// The bound data to the callback.
    void * userData
);


/// Sets the account remover callback.
///
/// The lifetime of the source object should match the lifetime of this RefBank
void topaz_refbank_set_account_remover(
    /// The refbank to modify.
    topazRefbank_t * bank,

    /// Function to be called when the account has exhausted all references
    topaz_refbank_account_remover remover,

    /// Data to be passed to the function
    void * data
);






#endif
