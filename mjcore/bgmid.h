#ifndef BGMID_H
#define BGMID_H

namespace sound {
  namespace IDs {

	const unsigned BgmStart = 0u;
	const unsigned BgmRounds = 28u;
	enum BgmID : unsigned {
		musRichi1 = BgmStart + BgmRounds,
		musRichi2,
		musRichi3,
		musOpenrichi,
		musAgariSelf1,
		musAgariSelf2,
		musAgariSelf3,
		musAgariFurikomi1,
		musAgariFurikomi2,
		musAgariFurikomi3,
		musAgariOther1,
		musAgariOther2,
		musAgariOther3,
		musRyuukyoku,
		musTitle,
		musEnding,
		musEnding2,
		musEnding3,
		musEffect,
		musFinal,
		musShibari,
	};
	const unsigned BgmEnd = musShibari;

	const unsigned SndStart = BgmEnd + 1;
	enum SoundID : unsigned {
		sndDahai1 = SndStart,
		sndDahai2,
		sndTsumo,
		sndSaikoro,
		sndBell,
		sndCountdown,
		sndCuohu,
		sndYakulst1,
		sndYakulst2,
		sndMekuri,
		sndButton,
		sndClick,
		sndCursor,
		sndPage,
		sndType,
		sndPingju,
		sndFlash,
		sndSignal,
		sndClock,
	};
	const unsigned SndEnd = sndClock;

	const unsigned VoxStart = SndEnd + 1;
	enum VSoundID : unsigned {
		voxChi = VoxStart,
		voxPon,
		voxKan,
		voxRichi,
		voxTsumo,
		voxRon,
		voxKyuushu,
		voxFlower,
		voxSikang,
		voxSifeng,
		voxSanjiahu,
		voxSijiarichi,
		voxRonFurikomi,
	};
	const unsigned VoxEnd = voxRonFurikomi;

  }
}

#endif
