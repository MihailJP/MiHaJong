#include "mouda.h"

#include <cassert>
#ifndef _WIN32
#include <unistd.h>
#endif /*_WIN32*/
#include "../common/strcode.h"
#include "logging.h"
#include "haifu.h"
#include "envtbl.h"
#include "ai/class.h"
#include "remote.h"
#include "../socket/socket.h"
#include "yaku/ykclass.h"
#include "yaku/yaku.h"
#include "../sound/sound.h"
#include "../common/bgmid.h"
#include "sound.h"
#include "fuuro.h"
#include "../graphic/graphic.h"
#include "ruletbl.h"

namespace {
	DiscardTileNum playerdahai(const GameTable* gameStat) { // �v���C���[�̑Ŕv
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		mihajong_graphic::Subscene(mihajong_graphic::tblSubscenePlayerDahai);
#ifdef _WIN32
		DWORD result = mihajong_graphic::ui::WaitUI();
#else /*_WIN32*/
		uint32_t result = mihajong_graphic::ui::WaitUI();
#endif /*_WIN32*/
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneNone);
		DiscardTileNum discardTile(DiscardTileNum::fromSingleInt(result));

		uint8_t dahaiStreamCode = 0x00;
		if (discardTile.type == DiscardTileNum::Agari)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Tsumo;
		else if (discardTile.type == DiscardTileNum::Kyuushu)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Kyuushu;
		else if (discardTile.type == DiscardTileNum::Normal)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_Normal_Offset + discardTile.id;
		else if (discardTile.type == DiscardTileNum::Ankan)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_Ankan_Offset + discardTile.id;
		else if (discardTile.type == DiscardTileNum::Kakan)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_Kakan_Offset + discardTile.id;
		else if (discardTile.type == DiscardTileNum::Riichi)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_Riichi_Offset + discardTile.id;
		else if (discardTile.type == DiscardTileNum::Flower)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_Flower_Offset + discardTile.id;
		else if (discardTile.type == DiscardTileNum::OpenRiichi)
			dahaiStreamCode = mihajong_socket::protocol::Dahai_Type_ORiichi_Offset + discardTile.id;

		if      (EnvTable::Instantiate()->GameMode == EnvTable::Server)
			mihajong_socket::server::send(dahaiStreamCode);
		else if (EnvTable::Instantiate()->GameMode == EnvTable::Client)
			mihajong_socket::client::send(dahaiStreamCode);

		return discardTile;
	}
}

DiscardTileNum getdahai(GameTable* const gameStat) {
	DiscardTileNum DiscardTileIndex;
	/* COM���u�J���j���O�v���Ȃ��悤�ɏ��� */
	GameTable* sandbox = makesandBox(gameStat, gameStat->CurrentPlayer.Active);
	/* �Ŕv���擾���� */
	if (gameStat->CurrentPlayer.Active == gameStat->PlayerID) {
		if (gameStat->statOfActive().AgariHouki) {
			debug(_T("�v���C���[�̃c���Ԃł����c�O�Ȃ���A�K�������ł��B"));
			DiscardTileIndex.type = DiscardTileNum::Normal;
			DiscardTileIndex.id = NumOfTilesInHand - 1;
		} else if (EnvTable::Instantiate()->WatchModeFlag) {
			DiscardTileIndex = aiscript::compdahai(sandbox);
		} else {
			debug(_T("�v���C���[�̃c���Ԃł��B"));
			DiscardTileIndex = playerdahai(gameStat);
		}
	} else if (
		(EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Active].RemotePlayerFlag == -1) ||
		gameStat->statOfActive().AgariHouki) {
			debug(_T("�A�K������������ؒf�����v���C���[�̃c���Ԃł��B"));
			DiscardTileIndex.type = DiscardTileNum::Normal;
			DiscardTileIndex.id = NumOfTilesInHand - 1;
	} else if ((EnvTable::Instantiate()->GameMode == EnvTable::Client) ||
		(EnvTable::Instantiate()->PlayerDat[gameStat->CurrentPlayer.Active].RemotePlayerFlag > 0)) {
			/* �l�b�g�ΐ펞�̏��� */
			debug(_T("�����[�g�v���C���[�̃c���Ԃł��B"));
			DiscardTileIndex = RemoteAction::remotedahai(gameStat);
	} else {
		debug(_T("AI�̃c���Ԃł��B"));
		DiscardTileIndex = aiscript::compdahai(sandbox);
		if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
			assert((DiscardTileIndex.type == DiscardTileNum::Kyuushu) ||
				(DiscardTileIndex.type == DiscardTileNum::Agari) ||
				((DiscardTileIndex.id >= 0) && (DiscardTileIndex.id < NumOfTilesInHand)));
			switch (DiscardTileIndex.type) {
			case DiscardTileNum::Kyuushu:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Kyuushu);
				break;
			case DiscardTileNum::Agari:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Tsumo);
				break;
			case DiscardTileNum::Normal:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Normal_Offset + DiscardTileIndex.id);
				break;
			case DiscardTileNum::Ankan:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Ankan_Offset + DiscardTileIndex.id);
				break;
			case DiscardTileNum::Kakan:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Kakan_Offset + DiscardTileIndex.id);
				break;
			case DiscardTileNum::Riichi:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Riichi_Offset + DiscardTileIndex.id);
				break;
			case DiscardTileNum::OpenRiichi:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_ORiichi_Offset + DiscardTileIndex.id);
				break;
			case DiscardTileNum::Flower:
				mihajong_socket::server::send(mihajong_socket::protocol::Dahai_Type_Flower_Offset + DiscardTileIndex.id);
				break;
			default:
				error(_T("CPU�̑Ŕv�ԍ����Ȃ񂩂�������"));
				break;
			}
		}
	}
	return DiscardTileIndex;
}

namespace { /* �������������p */
	EndType procDahaiSubAgari(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) { /* ���̘a�̏��� */
		EndType RoundEndType = Continuing;
		haifu::haifurectsumo(gameStat); // �v���ɋL�^
		/* ���A������p�̕ϐ� */
		if (RuleData::chkRuleApplied("paarenchan")) {
			if (gameStat->LastAgariPlayer == gameStat->CurrentPlayer.Active) {
				++gameStat->AgariChain;
			} else if (RuleData::chkRuleApplied("breaking_paarenchan") && (gameStat->AgariChain == 7)) {
				gameStat->AgariChain = -1; gameStat->LastAgariPlayer = gameStat->CurrentPlayer.Active;
			} else {
				gameStat->AgariChain = 1; gameStat->LastAgariPlayer = gameStat->CurrentPlayer.Active;
			}
		}
		/* ���̘a�������Ƃ�ϐ��ɐݒ� */
		gameStat->TsumoAgariFlag = true;
		yaku::YAKUSTAT yakuInfo = yaku::yakuCalculator::countyaku(gameStat, gameStat->CurrentPlayer.Active);
		if (((yakuInfo.CoreHan < (gameStat->ShibariFlag ? 2 : 1)) && (yakuInfo.CoreSemiMangan == 0)) ||
			(RuleData::chkRuleApplied("riichi_shibari") && (!gameStat->statOfActive().RichiFlag.RichiFlag)) ||
			((gameStat->PaoFlag[pyMinkan].agariPlayer != -1) && RuleData::chkRule("minkan_pao", "chombo_if_mahjong")))
			RoundEndType = Chonbo; /* ����𖞂����Ă��Ȃ��ꍇ(���������Ȃ�)�c���a�Ƃ��ċǂ��I������ */
		else
			RoundEndType = Agari; /* ����𖞂����Ȃ�a����Ƃ��Đ��� */
		gameStat->TsumoAgariFlag = true;
		gameStat->CurrentPlayer.Agari = gameStat->CurrentPlayer.Active;
		mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active,
			gameStat->TianHuFlag ?
			mihajong_graphic::calltext::RonQualified : //�V�a�̎��̓����ƌ������킵
			mihajong_graphic::calltext::Tsumo
			);
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCall); // �����\������
		gameStat->statOfAgari().HandStat = handExposed;
		if (gameStat->TianHuFlag) sound::Play(sound::IDs::voxRon);
		else sound::Play(sound::IDs::voxTsumo);
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		return RoundEndType;
	}
	EndType procDahaiSubKyuushu(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* ����v���錾���ꂽ�ꍇ */
		DiscardTileIndex.type = DiscardTileNum::Normal;
		DiscardTileIndex.id = NumOfTilesInHand; // ��헬�����ł��Ȃ����̓c���؂�Ƃ݂Ȃ�
		if (RuleData::chkRuleApplied("nine_terminals") &&
			chkdaopaiability(gameStat, gameStat->CurrentPlayer.Active) &&
			gameStat->statOfActive().FirstDrawFlag) {
				mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Kyuushu);
				mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCall); // �����\������
				gameStat->statOfActive().HandStat = handExposed;
				sound::Play(sound::IDs::voxKyuushu);
				mihajong_graphic::GameStatus::updateGameStat(gameStat);
				return KyuushuKyuuhai;
		} else {
			warn(_T("����v�͂ł��܂���B�c���؂�Ƃ݂Ȃ��܂��B"));
			return Continuing;
		}
	}
	EndType procDahaiSubFlower(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* �Ԕv�𔲂����ꍇ�̏��� */
		if ((DiscardTileIndex.type == DiscardTileNum::Ankan) &&
			(gameStat->statOfActive().Hand[DiscardTileIndex.id].tile > TileSuitFlowers)) {
				DiscardTileIndex.type = DiscardTileNum::Flower;
				info(_T("�Ԕv�̏����Ɉڂ�܂��B�Ŕv�R�[�h��␳���܂����B"));
		}
		if (DiscardTileIndex.type == DiscardTileNum::Flower) {
			EndType RoundEndType;
			if (gameStat->chkGameType(SanmaX)) {
				/* �K���O�����[���Ŗk���v�𔲂����Ƃ��̏��� */
				/* ���̃Q�[���ł͂ǂ�Ȏ�ł�(���m���l��łȂ��Ă�����)
					�����k�������ł��郋�[�� */
				if (fuuroproc(gameStat, &RoundEndType, DiscardTileIndex, FuuroNorth))
					return RoundEndType;
				else return DrawRinshan;
			} else {
				/* �Ԕv�𔲂����Ƃ��̏��� */
				/* ���̃Q�[���͎����ꂪ�Ȃ��̂ŉԔv�Ń�������邱�Ƃ͖��� */
				if (fuuroproc(gameStat, &RoundEndType, DiscardTileIndex, FuuroFlower))
					return RoundEndType;
				else return DrawRinshan;
			}
		}
		return Continuing;
	}
	EndType procDahaiSubKan(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* �ÞȁE���Ȃ���Ƃ��̏��� */
		const unsigned kanLim = (RuleData::chkRuleApplied("fifth_kong") ? 5 : 4);
		if ((gameStat->TilePointer < (gameStat->RinshanPointer - (gameStat->DeadTiles - 1))) && // �n�C�e�C�łȂ�
			(gameStat->KangNum < kanLim)) { // ���v���̐������ł���
				if ((DiscardTileIndex.type == DiscardTileNum::Ankan) ||
					(DiscardTileIndex.type == DiscardTileNum::Kakan)) {
						if (RuleData::chkRule("minkan_pao", "yes") || RuleData::chkRule("minkan_pao", "yes_2han")) {
							gameStat->PaoFlag[pyMinkan].paoPlayer =
								gameStat->PaoFlag[pyMinkan].agariPlayer = -1;
						}
						/* �Ȃ�����Ɨ��v�̕����̂�������̂Ŏ��̑Ŕv�� */
						EndType roundEndType;
						if (fuuroproc(gameStat, &roundEndType, DiscardTileIndex,
							(DiscardTileIndex.type == DiscardTileNum::Ankan) ?
							FuuroAnkan : FuuroKakan))
							return roundEndType;
						else return DrawRinshan;
				}
		}
		return Continuing;
	}
	void procDahaiSubRiichi(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) { /* ����������Ƃ��̏��� */
		if (gameStat->tilesLeft() < ACTUAL_PLAYERS) {
			// �c��S�������̎��̓��[�`����
			DiscardTileIndex.type = DiscardTileNum::Normal;
			warn(_T("�R�v�̎c���v���𖞂����Ă��܂���B�ʏ�̑Ŕv�Ƃ݂Ȃ��܂��B"));
		} else if (!isRichiReqSatisfied(gameStat, gameStat->CurrentPlayer.Active)) {
			// �_�_�����𖞂����Ă��Ȃ����̓��[�`����
			DiscardTileIndex.type = DiscardTileNum::Normal;
			warn(_T("�����_�̗v���𖞂����Ă��܂���B�ʏ�̑Ŕv�Ƃ݂Ȃ��܂��B"));
		}
		if ((!RuleData::chkRuleApplied("open_riichi")) && (DiscardTileIndex.type == DiscardTileNum::OpenRiichi)) {
			// �I�[�v�����������̎�
			DiscardTileIndex.type = DiscardTileNum::Riichi;
			warn(_T("�I�[�v�������͂ł��܂���B�ʏ�̗����Ƃ݂Ȃ��܂��B"));
		}
		/* ������錾���� */
		if (DiscardTileIndex.type == DiscardTileNum::OpenRiichi) {
			mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Riichi);
			mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCall); // �����\������
			sound::Play(sound::IDs::voxRichi);
			if (!(gameStat->Player[0].RichiFlag.OpenFlag || gameStat->Player[1].RichiFlag.OpenFlag ||
				gameStat->Player[2].RichiFlag.OpenFlag || gameStat->Player[3].RichiFlag.OpenFlag))
				sound::util::bgmplay(sound::IDs::musOpenrichi);
			mihajong_graphic::GameStatus::updateGameStat(gameStat);
#ifdef _WIN32
			Sleep(1000);
#else /*_WIN32*/
			usleep(1000000);
#endif /*_WIN32*/
			gameStat->statOfActive().HandStat = handOpenRiichi;
			gameStat->statOfActive().RichiFlag.OpenFlag = true;
		}
		if (DiscardTileIndex.type == DiscardTileNum::Riichi) {
			mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Riichi);
			mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCall); // �����\������
			sound::Play(sound::IDs::voxRichi);
			if (!(gameStat->Player[0].RichiFlag.OpenFlag || gameStat->Player[1].RichiFlag.OpenFlag ||
				gameStat->Player[2].RichiFlag.OpenFlag || gameStat->Player[3].RichiFlag.OpenFlag)) {
					if (!(gameStat->Player[0].RichiFlag.RichiFlag || gameStat->Player[1].RichiFlag.RichiFlag ||
						gameStat->Player[2].RichiFlag.RichiFlag || gameStat->Player[3].RichiFlag.RichiFlag)) {
							if (gameStat->CurrentPlayer.Active == gameStat->PlayerID)
								sound::util::bgmplay(sound::IDs::musRichi2);
							else
								sound::util::bgmplay(sound::IDs::musRichi1);
					} else if (!EnvTable::Instantiate()->WatchModeFlag) {
						if (gameStat->CurrentPlayer.Active == gameStat->PlayerID)
							sound::util::bgmplay(sound::IDs::musRichi3);
					}
			}
			mihajong_graphic::GameStatus::updateGameStat(gameStat);
#ifdef _WIN32
			Sleep(1000);
#else /*_WIN32*/
			usleep(1000000);
#endif /*_WIN32*/
		}
	}
	void procDahaiSubPost(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) { /* ���㏈�� */
		/* �Ŕv���L�^���� */
		DiscardTile* const newDiscard = &(gameStat->statOfActive().Discard[++gameStat->statOfActive().DiscardPointer]);
		newDiscard->tcode.tile = gameStat->CurrentDiscard.tile = gameStat->statOfActive().Hand[DiscardTileIndex.id].tile;
		newDiscard->tcode.red  = gameStat->CurrentDiscard.red  = gameStat->statOfActive().Hand[DiscardTileIndex.id].red;
		if (DiscardTileIndex.type == DiscardTileNum::Riichi) /* �����錾�v�̏ꍇ */
			newDiscard->dstat = discardRiichi;
		else if (DiscardTileIndex.type == DiscardTileNum::OpenRiichi) /* �I�[�v�������錾�v�̏ꍇ */
			newDiscard->dstat = discardRiichi;
		else /* ����ȊO�̏ꍇ */
			newDiscard->dstat = discardNormal;
		newDiscard->isDiscardThrough = (DiscardTileIndex.id == NumOfTilesInHand - 1) && (!gameStat->TianHuFlag);
		gameStat->statOfActive().Hand[DiscardTileIndex.id].tile = NoTile;
		gameStat->statOfActive().Hand[DiscardTileIndex.id].red  = Normal;
		/* �ꔭ�̃t���O���~�낷 */
		gameStat->statOfActive().RichiFlag.IppatsuFlag = false;
		/* �����I�ɗ��v���s�Ȃ� */
		lipai(gameStat, gameStat->CurrentPlayer.Active);
		// ���̂Ƃ��v���̂ĂĂ���͂� �\���o�O�h�~�̂���
		for (PlayerID i = 0; i < Players; i++) {
			gameStat->Player[i].Tsumohai().tile = NoTile;
			gameStat->Player[i].Tsumohai().red  = Normal;
		}
		/* ��������������̏ꍇ�A��_���������ꔭ�̃t���O�𗧂Ă� */
		if ((DiscardTileIndex.type == DiscardTileNum::Riichi) || (DiscardTileIndex.type == DiscardTileNum::OpenRiichi)) {
			mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::None);
			mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneNone); // ���������������
			gameStat->statOfActive().RichiFlag.RichiFlag =
				gameStat->statOfActive().RichiFlag.IppatsuFlag = true;
			gameStat->statOfActive().RichiFlag.DoubleFlag =
				gameStat->statOfActive().FirstDrawFlag;
			gameStat->statOfActive().RichiFlag.OpenFlag =
				(DiscardTileIndex.type == DiscardTileNum::OpenRiichi);
			++gameStat->Deposit;
			gameStat->statOfActive().PlayerScore -= 1000;
			if (DiscardTileIndex.type == DiscardTileNum::OpenRiichi)
				chkOpenMachi(gameStat, gameStat->CurrentPlayer.Active);
		}
		/* �V�a��n�a�̃t���O���~�낷 */
		gameStat->statOfActive().FirstDrawFlag =
			gameStat->TianHuFlag = false;
		/* �Ŕv����Ƃ��̉���炷 */
		/* �h�����̂Ă鎞�͋��ł̉��ɂ��� */
		if (gameStat->CurrentDiscard.tile > TileSuitFlowers)
			sound::Play(sound::IDs::sndDahai2);
		else if ((gameStat->CurrentDiscard.red == AkaDora) || (gameStat->DoraFlag.Omote[gameStat->CurrentDiscard.tile] > 0))
			sound::Play(sound::IDs::sndDahai2);
		else if ((gameStat->CurrentDiscard.red == AoDora) && (!RuleData::chkRule("blue_tiles", "-1han")))
			sound::Play(sound::IDs::sndDahai2);
		else
			sound::Play(sound::IDs::sndDahai1);
		/* ���̂Ƃ��v���̂ĂĂ���͂��Ȃ̂ŁA�o�O�h�~�̂��߂̏��� */
		for (PlayerID i = 0; i < Players; i++) {
			gameStat->Player[i].Tsumohai().tile = NoTile;
			gameStat->Player[i].Tsumohai().red  = Normal;
		}
		/* �ĕ`�� */
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
	}
}

EndType procdahai(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex) {
	EndType RoundEndType = Continuing;
	{
		CodeConv::tostringstream o;
		o << _T("�v���C���[ [") << (int)gameStat->CurrentPlayer.Active <<
			_T("] �Ŕv�^�C�v [") << (int)DiscardTileIndex.type <<
			_T("] ��v�ԍ� [") << (int)DiscardTileIndex.id << _T("]");
		info(o.str().c_str());
	}
	/* �������Ă��Ȃ������U���Ȃ炻�̊����̂��ߐU������������ */
	if (!gameStat->statOfActive().RichiFlag.RichiFlag)
		gameStat->statOfActive().DoujunFuriten = false;
	/* ���̘a�̏��� */
	if (DiscardTileIndex.type == DiscardTileNum::Agari) {
		RoundEndType = procDahaiSubAgari(gameStat, DiscardTileIndex);
		if (RoundEndType != Continuing) return RoundEndType;
	}
	/* ����v���錾���ꂽ�ꍇ */
	if ((!gameStat->chkGameType(SanmaS)) && (DiscardTileIndex.type == DiscardTileNum::Kyuushu)) {
		RoundEndType = procDahaiSubKyuushu(gameStat, DiscardTileIndex);
		if (RoundEndType != Continuing) return RoundEndType;
	}
	/* �Ŕv��v���ɋL�^���� */
	if ((DiscardTileIndex.type == DiscardTileNum::Normal) ||
		(DiscardTileIndex.type == DiscardTileNum::Riichi) ||
		(DiscardTileIndex.type == DiscardTileNum::OpenRiichi))
		haifu::haifurecmota(gameStat, DiscardTileIndex);
	/* �Ԕv�𔲂����ꍇ�̏��� */
	if (!gameStat->chkGameType(SanmaS)) {
		RoundEndType = procDahaiSubFlower(gameStat, DiscardTileIndex);
		if (RoundEndType != Continuing) return RoundEndType;
	}
	/* �ÞȁE���Ȃ���Ƃ��̏��� */
	{
		RoundEndType = procDahaiSubKan(gameStat, DiscardTileIndex);
		if (RoundEndType != Continuing) return RoundEndType;
	}
	gameStat->KangFlag.kangFlag = false; // ���J�Ԃ̃t���O���~�낷
	gameStat->PaoFlag[pyMinkan].paoPlayer = gameStat->PaoFlag[pyMinkan].agariPlayer = -1;
	/* ����������Ƃ��̏��� */
	if ((DiscardTileIndex.type == DiscardTileNum::Riichi) ||
		(DiscardTileIndex.type == DiscardTileNum::OpenRiichi))
		procDahaiSubRiichi(gameStat, DiscardTileIndex);
	/* �ߔv�V�a�t���O */
	if ((gameStat->statOfActive().renpaiTenhohStat == 0) &&
		(ShantenAnalyzer::calcShanten(gameStat, gameStat->CurrentPlayer.Active, shantenAll) == -1))
		gameStat->statOfActive().renpaiTenhohStat = 1;
	else if (gameStat->statOfActive().renpaiTenhohStat == 1)
		gameStat->statOfActive().renpaiTenhohStat = -1;
	/* ���㏈�� */
	procDahaiSubPost(gameStat, DiscardTileIndex);
	return Continuing;
}

void tsumoproc(GameTable* const gameStat) {
	/* ���̃v���C���[���v�����̂� */
	gameStat->TianHuFlag = false;
	if (gameStat->chkGameType(SanmaT)) {
		gameStat->CurrentPlayer.Active = (gameStat->CurrentPlayer.Active + 1) % ACTUAL_PLAYERS;
	} else {
		if (gameStat->chkGameType(Sanma4) && (gameStat->playerwind(gameStat->CurrentPlayer.Active) == sWest)) /* �l�l�O���̏ꍇ�͖k�Ƃ��X�L�b�v */
			gameStat->CurrentPlayer.Active = (gameStat->CurrentPlayer.Active + 1) % Players;
		gameStat->CurrentPlayer.Active = (gameStat->CurrentPlayer.Active + 1) % Players;
	}
	/* ���Ƃ̏��Ԃ�����Ă����玟�̏��ڂƂȂ� */
	if (gameStat->playerwind(gameStat->CurrentPlayer.Active) == sEast)
		++gameStat->TurnRound;
	gameStat->statOfActive().Tsumohai().tile =
		gameStat->Deck[gameStat->TilePointer].tile;
	gameStat->statOfActive().Tsumohai().red =
		gameStat->Deck[gameStat->TilePointer].red;
	gameStat->PreviousMeld.Discard = gameStat->PreviousMeld.Stepped = NoTile;
	++gameStat->TilePointer;
	sound::Play(sound::IDs::sndTsumo);
	if (gameStat->tilesLeft() < 10)
		sound::Play(sound::IDs::sndCountdown);
	return;
}
