// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

using namespace std;

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
	// Genesis block

	// MainNet:

	//CBlock(hash=000006301df0e22e8c3b5eed7aa53e9d3194652b4c7871390f265b0506c9583e, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=4cb2a8f18bb29e01f10306592dfc03ad28df3cd0260ef65125d5fc1958bc70ef, nTime=1537540522, nBits=1e0fffff, nNonce=1355266, vtx=1, vchBlockSig=)
	//  Coinbase(hash=4cb2a8f18b, nTime=1537540522, ver=1, vin.size=1, vout.size=1, nLockTime=0)
	//    CTxIn(COutPoint(0000000000, 4294967295), coinbase 00012a24323020466562203230313420426974636f696e2041544d7320636f6d6520746f20555341)
	//    CTxOut(empty)
	//  vMerkleTree: 4cb2a8f18b

	CMutableTransaction txNew;
	txNew.nVersion = 1;
	txNew.nTime = nTime;
	txNew.vin.resize(1);
	txNew.vout.resize(1);
	txNew.vin[0].scriptSig = CScript() << 0 << CScriptNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
	txNew.vout[0].nValue = genesisReward;

	CBlock genesis;
	genesis.nTime = nTime;
	genesis.nBits = nBits;
	genesis.nNonce = nNonce;
	genesis.nVersion = nVersion;
	genesis.vtx.push_back(txNew);
	genesis.hashPrevBlock.SetNull();
	genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
	return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
	const char* pszTimestamp = "New Coin developerment ";
	const CScript genesisOutputScript = CScript();
	return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
 /**
  * What makes a good checkpoint block?
  * + Is surrounded by blocks with reasonable timestamps
  *   (no blocks before with a timestamp after, none after with
  *    timestamp before)
  * + Contains no strange transactions
  */

class CMainParams : public CChainParams {
public:
	CMainParams() {
		strNetworkID = "main";
		consensus.nMaxReorganizationDepth = 200;
		consensus.nMajorityEnforceBlockUpgrade = 9500;
		consensus.nMajorityRejectBlockOutdated = 950;
		consensus.nMajorityWindow = 1000;
		consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
		consensus.posLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
		consensus.posLimitV2 = uint256S("000000000000ffffffffffffffffffffffffffffffffffffffffffffffffffff");
		consensus.nTargetTimespan = 16 * 60; // 16 mins
		consensus.nTargetSpacingV1 = 1 * 60;
		consensus.nTargetSpacing = 2 * 60;
		consensus.BIP34Height = -1;
		consensus.BIP34Hash = uint256();
		consensus.fPowAllowMinDifficultyBlocks = false;
		consensus.fPowNoRetargeting = false;
		consensus.fPoSNoRetargeting = false;
		consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
		consensus.nMinerConfirmationWindow = 2016; // nTargetTimespan / nTargetSpacing
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

		/*
		Deployment of BIP68, BIP112, and BIP113.
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 999999999999ULL; // never
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 0; // out of time
		*/

		consensus.nProtocolV1RetargetingFixedTime = 1395631999;
		consensus.nLastPOWBlock = 5000;
		consensus.nStakeTimestampMask = 0xf; // 15
		consensus.nCoinbaseMaturity = 20;
		consensus.nStakeMinAge = 8 * 60 * 60; // 8 hours

		// The best chain should have at least this much work.
		consensus.nMinimumChainWork = uint256S("0x00");
		//consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000002ef7a6c83f19a157178");

		/**
		 * The message start string is designed to be unlikely to occur in normal data.
		 * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
		 * a large 32-bit integer with any alignment.
		 */
		pchMessageStart[0] = 0x6a;
		pchMessageStart[1] = 0xe3;
		pchMessageStart[2] = 0x1b;
		pchMessageStart[3] = 0xc4;
		nDefaultPort = 26681;
		nPruneAfterHeight = 100000;

		genesis = CreateGenesisBlock(1537540522, 1355266, 0x1e0fffff, 1, 0);
		consensus.hashGenesisBlock = genesis.GetHash();
		assert(consensus.hashGenesisBlock == uint256S("0x000006301df0e22e8c3b5eed7aa53e9d3194652b4c7871390f265b0506c9583e"));
		assert(genesis.hashMerkleRoot == uint256S("0x4cb2a8f18bb29e01f10306592dfc03ad28df3cd0260ef65125d5fc1958bc70ef"));

		vSeeds.push_back(CDNSSeedData("134.209.13.43", "134.209.13.43"));
		vSeeds.push_back(CDNSSeedData("142.93.143.35", "142.93.143.35"));
		vSeeds.push_back(CDNSSeedData("68.183.115.49", "68.183.115.49"));
		vSeeds.push_back(CDNSSeedData("167.172.121.86", "167.172.121.86"));
		vSeeds.push_back(CDNSSeedData("157.230.29.96", "157.230.29.96"));
		vSeeds.push_back(CDNSSeedData("157.230.150.227", "157.230.150.227"));
		vSeeds.push_back(CDNSSeedData("159.65.99.102", "159.65.99.102"));

		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 23);
		base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 85);
		base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 151);
		base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
		base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
		cashaddrPrefix = "oduwacoin";

		vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

		fMiningRequiresPeers = true;
		fDefaultConsistencyChecks = false;
		fRequireStandard = true;
		fMineBlocksOnDemand = false;
		fTestnetToBeDeprecatedFieldRPC = false;

		checkpointData = (CCheckpointData) {
			boost::assign::map_list_of
			    (0, uint256S("0x000006301df0e22e8c3b5eed7aa53e9d3194652b4c7871390f265b0506c9583e"))
				(244260, uint256S("0x34cb64db3f25b869794a52a156ab2ef4d9cc2e8635660e9495f14f0fccd0125b"))
				(244261, uint256S("0x3928e448b5499715ed1cb6c48a38728cd0fe72d0385ffd57470aa8d9fefc2841"))
				(244262, uint256S("0x0304aef8843f995125235a4d80ce3a70981a68e7b1048948cf04fbfffb5e69c3"))
				(244300, uint256S("0xc29c11687dce9b2ead8863f9904ccd07d70e77bce0942bd99c039e02cef69c59"))
				(245300, uint256S("0xbc56f6ad6122fcc000dd4b9d9e501dcfcefc736f83c3903ba3adccc90f4be49d"))
				(321586, uint256S("0x60c2741577c676d759655b99840fca515f15ed90b79b108411f8787c2c92e8d8"))
				(393647, uint256S("0x40040135ad4f9de5173aeaeb8a8dec0dcf3733a57a258fc35c5d6c9fb2a86059"))
				(457102, uint256S("0x740f31be3627d7ffdc2bded13ba9ff73ed1ede85eac126cffb6f888043e15876"))
				(515643, uint256S("0x471a72164b5b6c582f4df3a259333a12a749f8add51c24a8d519e739d9caf76e"))
				(587915, uint256S("0x1e3a0efe5894242dbdb19b8c5c6e14cb8cbaa5537738423da268c245f68b092b"))
				(661408, uint256S("0xa612e00610b46cf131ce0161e64dfe16f03cae746b6873bef7e998ae95c7e5d8"))
				(743420, uint256S("0xc2334773223f3d8abc1bca58ac0e37842146954971cfc06334d3218a834fbfb8"))
				(823569, uint256S("0x2400332d2ab7c890443e9d17a9b02373687fad4df4a46fa89e2555845326e366"))
				(956842, uint256S("0x9a69d79fae6c38ce3b979f6a5d1eaf87633a289eac0e8285a61fa04173971695"))
				(1054923, uint256S("0xd1d350718f4c63811375e4c9fcf0dbf1b40948098efde17a9344464cbcb92bff"))
				(1194263, uint256S("0xa622843b76312678b90af5d7297839fad5c658e9811993917bf9ae8d1d085e8a"))
				(1218387, uint256S("0x4567d7d237ec80ddcee1a837faca1c5bb05a4320f2794bdc198439ff74076a4f"))
				(1239909, uint256S("0xa341508b07e1296ad943e8028ac4f86f60f6d065f97913814d23ef89cd806d5f"))
				,
				1622539280, // * UNIX timestamp of last checkpoint block
				2486399,    // * total number of transactions between genesis and last checkpoint
							//   (the tx=... number in the SetBestChain debug.log lines)
				3500.0      // * estimated number of transactions per day after checkpoint
		};
	}
};
static CMainParams mainParams;

/**
 * Testnet
 */
class CTestNetParams : public CChainParams {
public:
	CTestNetParams() {
		strNetworkID = "test";
		consensus.nMaxReorganizationDepth = 500;
		consensus.nMajorityEnforceBlockUpgrade = 750;
		consensus.nMajorityRejectBlockOutdated = 950;
		consensus.nMajorityWindow = 1000;
		consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
		consensus.posLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
		consensus.posLimitV2 = uint256S("000000000000ffffffffffffffffffffffffffffffffffffffffffffffffffff");
		consensus.nTargetTimespan = 16 * 60; // 16 mins
		consensus.nTargetSpacingV1 = 60;
		consensus.nTargetSpacing = 64;
		consensus.BIP34Height = -1;
		consensus.BIP34Hash = uint256();
		consensus.fPowAllowMinDifficultyBlocks = true;
		consensus.fPowNoRetargeting = false;
		consensus.fPoSNoRetargeting = false;
		consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
		consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

		consensus.nProtocolV1RetargetingFixedTime = 1395631999;
		consensus.nLastPOWBlock = 0x7fffffff;
		consensus.nStakeTimestampMask = 0xf;
		consensus.nCoinbaseMaturity = 10;
		consensus.nStakeMinAge = 8 * 60 * 60;

		pchMessageStart[0] = 0xcd;
		pchMessageStart[1] = 0xf2;
		pchMessageStart[2] = 0xc0;
		pchMessageStart[3] = 0xef;
		nDefaultPort = 25714;

		// The best chain should have at least this much work.
		consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000028afadd1049e589cf2");

		/*
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 999999999999ULL; // never
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 0; // out of time
		*/

		nPruneAfterHeight = 1000;

		genesis = CreateGenesisBlock(1393221600, 216178, 0x1f00ffff, 1, 0);
		consensus.hashGenesisBlock = genesis.GetHash();
		//assert(consensus.hashGenesisBlock == uint256S("0x0000724595fb3b9609d441cbfb9577615c292abf07d996d3edabc48de843642d"));
		//assert(genesis.hashMerkleRoot == uint256S("0x12630d16a97f24b287c8c2594dda5fb98c9e6c70fc61d44191931ea2aa08dc90"));

		vFixedSeeds.clear();
		vSeeds.clear();

		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 111);
		base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
		base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
		base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
		base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
		cashaddrPrefix = "blktest";

		vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

		fMiningRequiresPeers = true;
		fDefaultConsistencyChecks = false;
		fRequireStandard = false;
		fMineBlocksOnDemand = false;
		fTestnetToBeDeprecatedFieldRPC = true;

		checkpointData = (CCheckpointData) {
			boost::assign::map_list_of
			(90235, uint256S("0x567898e79184dc2f7dc3a661f794f28566e4b856d70180914f7371b1b3cc82d8")),
				1549558800,
				179080,
				2.0
		};

	}
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
	CRegTestParams() {
		strNetworkID = "regtest";
		consensus.nMaxReorganizationDepth = 50;
		consensus.nMajorityEnforceBlockUpgrade = 51;
		consensus.nMajorityRejectBlockOutdated = 75;
		consensus.nMajorityWindow = 100;
		consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
		consensus.posLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
		consensus.posLimitV2 = uint256S("000000000000ffffffffffffffffffffffffffffffffffffffffffffffffffff");
		consensus.nTargetTimespan = 16 * 60; // 16 mins
		consensus.nTargetSpacingV1 = 64;
		consensus.nTargetSpacing = 64;
		consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
		consensus.BIP34Hash = uint256();
		consensus.fPowAllowMinDifficultyBlocks = true;
		consensus.fPowNoRetargeting = true;
		consensus.fPoSNoRetargeting = true;
		consensus.nRuleChangeActivationThreshold = 108;// 75% for regtest
		consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
		consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

		// The best chain should have at least this much work.
		consensus.nMinimumChainWork = uint256S("0x00");

		/*
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 999999999999ULL; // never
		consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 0; // out of time
		*/

		consensus.nProtocolV1RetargetingFixedTime = 1395631999;
		consensus.nLastPOWBlock = 1000;
		consensus.nStakeTimestampMask = 0xf;
		consensus.nCoinbaseMaturity = 10;
		consensus.nStakeMinAge = 1 * 60 * 60;

		pchMessageStart[0] = 0x70;
		pchMessageStart[1] = 0x35;
		pchMessageStart[2] = 0x22;
		pchMessageStart[3] = 0x06;
		nDefaultPort = 35714;
		nPruneAfterHeight = 100000;

		genesis = CreateGenesisBlock(1393221600, 216178, 0x1f00ffff, 1, 0);
		consensus.hashGenesisBlock = genesis.GetHash();
		//assert(consensus.hashGenesisBlock == uint256S("0x0000724595fb3b9609d441cbfb9577615c292abf07d996d3edabc48de843642d"));
		//assert(genesis.hashMerkleRoot == uint256S("0x12630d16a97f24b287c8c2594dda5fb98c9e6c70fc61d44191931ea2aa08dc90"));

		vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
		vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

		base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 111);
		base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
		base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
		base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
		base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
		cashaddrPrefix = "blkreg";

		fMiningRequiresPeers = false;
		fDefaultConsistencyChecks = true;
		fRequireStandard = false;
		fMineBlocksOnDemand = true;
		fTestnetToBeDeprecatedFieldRPC = false;

	}

	void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
	{
		consensus.vDeployments[d].nStartTime = nStartTime;
		consensus.vDeployments[d].nTimeout = nTimeout;
	}
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
	assert(pCurrentParams);
	return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
	if (chain == CBaseChainParams::MAIN)
		return mainParams;
	else if (chain == CBaseChainParams::TESTNET)
		return testNetParams;
	else if (chain == CBaseChainParams::REGTEST)
		return regTestParams;
	else
		throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
	SelectBaseParams(network);
	pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
	regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
