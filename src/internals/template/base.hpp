/*
 * internals/<TEMPLATE>/base.hpp - id2 library
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Chia Jason
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _<TEMPLATE>_BASE_HPP_
#define _<TEMPLATE>_BASE_HPP_

#include "../internal.hpp"
#include "static.hpp"
#include "proto.hpp"

namespace <TEMPLATE>{
	const struct algostr ftable = {
		&randomkey,
		&signatgen,
		&signatchk,
		&hashexec,
		&hashfree,
		&secserial,
		&pubserial,
		&sigserial,
		&secstruct,
		&pubstruct,
		&sigstruct,
		&secdestroy,
		&pubdestroy,
		&sigdestroy,
		&secprint,
		&pubprint,
		&sigprint,
		&signatprv,
		&signatvrf,
		&prototest
	};
}

#endif