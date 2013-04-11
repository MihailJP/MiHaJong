#include "utils.h"

namespace mihajong_graphic {
namespace utils {

playerRelative_type playerRelative = nullptr;
RelativePositionOf_type RelativePositionOf = nullptr;
calcRank_type calcRank = nullptr;
chkFuriten_type chkFuriten = nullptr;
isTenpai_type isTenpai = nullptr;
isRichiReqSatisfied_type isRichiReqSatisfied = nullptr;
calcShanten_type calcShanten = nullptr;
chkdaopaiability_type chkdaopaiability = nullptr;
chkShisanBuDa_type chkShisanBuDa = nullptr;
chkShisiBuDa_type chkShisiBuDa = nullptr;
countTilesInHand_type countTilesInHand = nullptr;
chkAnkanAbility_type chkAnkanAbility = nullptr;
isPao_type isPao = nullptr;
isPaoAgari_type isPaoAgari = nullptr;
bgmplay_type bgmplay = nullptr;
getName_type getName = nullptr;
sendchat_type sendchat = nullptr;

EXPORT void setfunc(
	playerRelative_type ptr_playerRelative,
	RelativePositionOf_type ptr_RelativePositionOf,
	calcRank_type ptr_calcRank,
	chkFuriten_type ptr_chkFuriten,
	isTenpai_type ptr_isTenpai,
	isRichiReqSatisfied_type ptr_isRichiReqSatisfied,
	calcShanten_type ptr_calcShanten,
	chkdaopaiability_type ptr_chkdaopaiability,
	chkShisanBuDa_type ptr_chkShisanBuDa,
	chkShisiBuDa_type ptr_chkShisiBuDa,
	countTilesInHand_type ptr_countTilesInHand,
	chkAnkanAbility_type ptr_chkAnkanAbility,
	isPao_type ptr_isPao,
	isPaoAgari_type ptr_isPaoAgari,
	bgmplay_type ptr_bgmplay,
	getName_type ptr_getName,
	sendchat_type ptr_sendchat)
{
	playerRelative = ptr_playerRelative;
	RelativePositionOf = ptr_RelativePositionOf;
	calcRank = ptr_calcRank;
	chkFuriten = ptr_chkFuriten;
	isTenpai = ptr_isTenpai;
	isRichiReqSatisfied = ptr_isRichiReqSatisfied;
	calcShanten = ptr_calcShanten;
	chkdaopaiability = ptr_chkdaopaiability;
	chkShisanBuDa = ptr_chkShisanBuDa;
	chkShisiBuDa = ptr_chkShisiBuDa;
	countTilesInHand = ptr_countTilesInHand;
	chkAnkanAbility = ptr_chkAnkanAbility;
	isPao = ptr_isPao;
	isPaoAgari = ptr_isPaoAgari;
	bgmplay = ptr_bgmplay;
	getName = ptr_getName;
	sendchat = ptr_sendchat;
}

}
}
