// Copyright (c) 2018-2021 The OduwaCoin Developers
// Copyright (c) 2014-2018 The BlackCoin Developers
// Copyright (c) 2011-2013 The PPCoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

// Stake cache by Qtum
// Copyright (c) 2016-2018 The Qtum developers

#ifndef ODUWACOIN_POS_H
#define ODUWACOIN_POS_H

#include "pos.h"
#include "txdb.h"
#include "main.h"
#include "arith_uint256.h"
#include "consensus/validation.h"
#include "hash.h"
#include "timedata.h"
#include "chainparams.h"
#include "script/sign.h"
#include <stdint.h>

using namespace std;

// MODIFIER_INTERVAL: time to elapse before new modifier is computed
extern unsigned int nModifierInterval;

// MODIFIER_INTERVAL_RATIO:
// ratio of group interval length between the last group and the first group
static const int MODIFIER_INTERVAL_RATIO = 3;

// Whether a given transaction is subject to new v0.5 protocol
bool IsProtocolV05(unsigned int nTimeTx);

/** Compute the hash modifier for proof-of-stake */
bool ComputeNextStakeModifier(const CBlockIndex* pindexPrev, uint64_t& nStakeModifier, bool& fGeneratedStakeModifier);
//uint64_t ComputeStakeModifier(const CBlockIndex* pindexPrev, const uint256& kernel);

struct CStakeCache{
    CStakeCache(uint256 hashBlock_, const CTransaction txPrev_) : hashBlock(hashBlock_), txPrev(txPrev_){
    }
    uint256 hashBlock;
    const CTransaction txPrev;
};

// Check whether the coinstake timestamp meets protocol
bool CheckCoinStakeTimestamp(int64_t nTimeBlock, int64_t nTimeTx);
bool CheckStakeBlockTimestamp(int64_t nTimeBlock);
bool CheckKernel(CBlockIndex* pindexPrev, unsigned int nBits, uint32_t nTime, const COutPoint& prevout);
bool CheckKernel(CBlockIndex* pindexPrev, unsigned int nBits, uint32_t nTime, const COutPoint& prevout, const std::map<COutPoint, CStakeCache>& cache);
bool CheckStakeKernelHash(CBlockIndex* pindexPrev, unsigned int nBits, const CCoins* txPrev, const COutPoint& prevout, unsigned int nTimeTx, uint256& hashProofOfStake, bool fPrintProofOfStake = false);
bool CheckProofOfStake(CBlockIndex* pindexPrev, const CTransaction& tx, unsigned int nBits, CValidationState &state);
bool VerifySignature(const CTransaction& txFrom, const CTransaction& txTo, unsigned int nIn, unsigned int flags, int nHashType);

// Get time weight using supplied timestamps
int64_t GetCoinAgeWeight(int64_t nIntervalBeginning, int64_t nIntervalEnd);

// Get transaction coin age
uint64_t GetCoinAge(const CTransaction& tx);

// Calculate total coin age spent in block
uint64_t GetCoinAge(const CBlock& block);
#endif // ODUWACOIN_POS_H
