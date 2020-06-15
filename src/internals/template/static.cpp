/*
 * internals/<TEMPLATE>/static.cpp - id2 library
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

/*
 * TODO: please edit description
 *
 * ToraNova 2020
 * chia_jason96@live.com
 *
 * this is for internal use only!
 */

#include "static.hpp"

#include "../../utils/bufhelp.h"
#include "../../utils/debug.h"

//include general constant and macros
#include "../cmacro.h"

// implementation includes (archlinux os stored under /usr/include/sodium)
//Nacl Finite Field Arithmetic on top of Curve25519
#include <sodium.h>

//#include <sodium/crypto_core_ristretto255.h>
//#include <sodium/crypto_scalarmult_ristretto255.h>
//#include <sodium/randombytes.h>
////512bit hash (64byte)
//#include <sodium/crypto_hash_sha512.h>
//#include <sodium/crypto_verify_32.h>

// standard lib
#include <cstdlib>
#include <cstdio>
#include <cstring>

/*
 * Key storage detail (as of 2020 Mar 22)
 *
 * skey - < 256 a >< 256 B >< 256 P1 >< 256 P2 > (msk)
 * pkey - < 256 B >< 256 P1 >< 256 P2 > (mpk)
 * sig  - < 256 s >< 256 x >< 256 U >< 256 V >< 256 B > (does not need to store pkey anymore)
 *
 */

namespace <TEMPLATE>{

	void randomkey(void **out){
		//declare and allocate memory for key
		int rc; struct seckey *tmp;
		tmp = (struct seckey *)malloc( sizeof(struct seckey) );
		//allocate memory for pubkey
		tmp->pub = (struct pubkey *)malloc( sizeof(struct pubkey) );

		//recast and return
		*out = (void *) tmp; return;
	}

	void signatgen(
		void *vkey,
		unsigned char *mbuffer, size_t mlen,
		void **out
	){
		//key recast
		int rc; struct signat *tmp;
		struct seckey *key = (struct seckey *)vkey;
		//declare and allocate for signature struct
		tmp = (struct signat *)malloc( sizeof( struct signat) );

		*out = (void *) tmp; return;
	}

	int signatchk(
		void *vpar,
		void *vsig,
		unsigned char *mbuffer, size_t mlen
	){
		//key recast
		int rc; unsigned char *xp;
		struct pubkey *par = (struct pubkey *)vpar;
		struct signat *sig = (struct signat *)vsig;

		hashfree(xp);
		return 0;
	}

	unsigned char *hashexec(
		unsigned char *mbuffer, size_t mlen,
		unsigned char *ubuffer,
		unsigned char *vbuffer
	){
		crypto_hash_sha512_state eh_state;
		unsigned char hshe[RS_HSSZ]; //hash
		unsigned char *out = (unsigned char *)malloc( RS_SCSZ );

		//compute hash
		crypto_hash_sha512_init( &eh_state );
		crypto_hash_sha512_update( &eh_state, mbuffer, mlen);
		crypto_hash_sha512_update( &eh_state, ubuffer, RS_EPSZ);
		crypto_hash_sha512_update( &eh_state, vbuffer, RS_EPSZ);
		crypto_hash_sha512_final( &eh_state, hshe);
		crypto_core_ristretto255_scalar_reduce(
			out, (const unsigned char *)hshe
		);
		return out;
	}

	void hashfree(unsigned char *hash){
		sodium_memzero(hash, RS_SCSZ);
		free(hash); return;
	}

	size_t secserial(void *in, unsigned char **sbuffer, size_t *slen){
		size_t rs;
		struct seckey *ri = (struct seckey *)in; //recast the key
		//set size and allocate
		*slen = SKEY_SZ;
		*sbuffer = (unsigned char *)malloc( *(slen) );

		//a, B, P1, P2
		rs = copyskip( *sbuffer, ri->a, 	0, 	RS_SCSZ);
		rs = copyskip( *sbuffer, ri->pub->B, 	rs, 	RS_EPSZ);
		rs = copyskip( *sbuffer, ri->pub->P1, 	rs, 	RS_EPSZ);
		rs = copyskip( *sbuffer, ri->pub->P2, 	rs, 	RS_EPSZ);

		return rs;
	}

	size_t pubserial(void *in, unsigned char **pbuffer, size_t *plen){
		size_t rs;
		struct seckey *ri = (struct seckey *)in; //recast the key
		//set size and allocate
		*plen = PKEY_SZ;
		*pbuffer = (unsigned char *)malloc( *(plen) );

		//B, P1, P2
		rs = copyskip( *pbuffer, ri->pub->B, 	0, 	RS_EPSZ);
		rs = copyskip( *pbuffer, ri->pub->P1, 	rs, 	RS_EPSZ);
		rs = copyskip( *pbuffer, ri->pub->P2, 	rs, 	RS_EPSZ);

		return rs;
	}

	size_t sigserial(void *in, unsigned char **obuffer, size_t *olen){
		size_t rs;
		struct signat *ri = (struct signat *)in; //recast the key
		//set size and allocate
		*olen = SGNT_SZ;
		*obuffer = (unsigned char *)malloc( *(olen) );

		//s,x,U,V,B
		rs = copyskip( *obuffer, ri->s, 	0, 	RS_SCSZ);
		rs = copyskip( *obuffer, ri->x, 	rs, 	RS_SCSZ);
		rs = copyskip( *obuffer, ri->U, 	rs, 	RS_EPSZ);
		rs = copyskip( *obuffer, ri->V, 	rs, 	RS_EPSZ);
		rs = copyskip( *obuffer, ri->B, 	rs, 	RS_EPSZ);

		return rs;
	}

	void secstruct(unsigned char *sbuffer, size_t slen, void **out){
		struct seckey *tmp; size_t rs;
		//allocate memory for seckey
		tmp = (struct seckey *)malloc( sizeof(struct seckey));
		//allocate memory for pubkey
		tmp->pub = (struct pubkey *)malloc( sizeof(struct pubkey) );

		//allocate memory for the elements and scalars
		tmp->a = (unsigned char *)malloc( RS_SCSZ );
		tmp->pub->B = (unsigned char *)malloc( RS_EPSZ );
		tmp->pub->P1 = (unsigned char *)malloc( RS_EPSZ );
		tmp->pub->P2 = (unsigned char *)malloc( RS_EPSZ );

		rs = skipcopy( tmp->a,		sbuffer, 0, 	RS_SCSZ);
		rs = skipcopy( tmp->pub->B,	sbuffer, rs, 	RS_EPSZ);
		rs = skipcopy( tmp->pub->P1,	sbuffer, rs, 	RS_EPSZ);
		rs = skipcopy( tmp->pub->P2,	sbuffer, rs, 	RS_EPSZ);

		*out = (void *) tmp; return;
	}


	void pubstruct(unsigned char *pbuffer, size_t plen, void **out){
		struct pubkey *tmp; size_t rs;
		//allocate memory for pubkey
		tmp = (struct pubkey *)malloc( sizeof(struct pubkey) );

		//allocate memory for the elements
		tmp->B = (unsigned char *)malloc( RS_EPSZ );
		tmp->P1 = (unsigned char *)malloc( RS_EPSZ );
		tmp->P2 = (unsigned char *)malloc( RS_EPSZ );

		rs = skipcopy( tmp->B,		pbuffer, 0, 	RS_EPSZ);
		rs = skipcopy( tmp->P1,		pbuffer, rs, 	RS_EPSZ);
		rs = skipcopy( tmp->P2,		pbuffer, rs, 	RS_EPSZ);

		*out = (void *) tmp; return;
	}

	void sigstruct(unsigned char *obuffer, size_t olen, void **out){
		struct signat *tmp; size_t rs;
		//allocate memory for pubkey
		tmp = (struct signat *)malloc( sizeof(struct signat) );

		//allocate for components on signature struct
		tmp->s = (unsigned char *)malloc( RS_SCSZ );
		tmp->x = (unsigned char *)malloc( RS_SCSZ );
		tmp->U = (unsigned char *)malloc( RS_EPSZ );
		tmp->V = (unsigned char *)malloc( RS_EPSZ );
		tmp->B = (unsigned char *)malloc( RS_EPSZ );

		rs = skipcopy( tmp->s,		obuffer, 0, 	RS_SCSZ);
		rs = skipcopy( tmp->x,		obuffer, rs, 	RS_SCSZ);
		rs = skipcopy( tmp->U,		obuffer, rs, 	RS_EPSZ);
		rs = skipcopy( tmp->V,		obuffer, rs, 	RS_EPSZ);
		rs = skipcopy( tmp->B,		obuffer, rs, 	RS_EPSZ);

		*out = (void *) tmp; return;
	}

	//destroy secret key
	void secdestroy(void *in){
		//key recast
		struct seckey *ri = (struct seckey *)in;
		//zero out the secret component
		sodium_memzero(ri->a, RS_SCSZ);

		//free memory
		free(ri->a);
		pubdestroy(ri->pub);
		free(ri); return;
	}

	void pubdestroy(void *in){
		//key recast
		struct pubkey *ri = (struct pubkey *)in;
		//free up memory
		free(ri->B);
		free(ri->P1);
		free(ri->P2);
		free(ri); return;
	}

	void sigdestroy(void *in){
		//key recast
		struct signat *ri = (struct signat *)in;
		//clear the components
		sodium_memzero(ri->s, RS_SCSZ);
		sodium_memzero(ri->x, RS_SCSZ);
		sodium_memzero(ri->U, RS_EPSZ);
		sodium_memzero(ri->V, RS_EPSZ);
		//free memory
		free(ri->s);
		free(ri->x);
		free(ri->U);
		free(ri->V);
		free(ri->B);
		free(ri); return;
	}

	//debugging use only
	void secprint(void *in){
		struct seckey *ri = (struct seckey *)in;
		printf("a :"); ucbprint(ri->a, RS_SCSZ); printf("\n");
		printf("B :"); ucbprint(ri->pub->B, RS_EPSZ); printf("\n");
		printf("P1:"); ucbprint(ri->pub->P1, RS_EPSZ); printf("\n");
		printf("P2:"); ucbprint(ri->pub->P2, RS_EPSZ); printf("\n");
		return;
	}

	void pubprint(void *in){
		struct pubkey *ri = (struct pubkey *)in;
		printf("B :"); ucbprint(ri->B, RS_EPSZ); printf("\n");
		printf("P1:"); ucbprint(ri->P1, RS_EPSZ); printf("\n");
		printf("P2:"); ucbprint(ri->P2, RS_EPSZ); printf("\n");
		return;
	}

	void sigprint(void *in){
		struct signat *ri = (struct signat *)in;
		printf("s :"); ucbprint(ri->s, RS_SCSZ); printf("\n");
		printf("x :"); ucbprint(ri->x, RS_SCSZ); printf("\n");
		printf("U :"); ucbprint(ri->U, RS_EPSZ); printf("\n");
		printf("V :"); ucbprint(ri->V, RS_EPSZ); printf("\n");
		printf("B :"); ucbprint(ri->B, RS_EPSZ); printf("\n");
		return;
	}

}
