auto isshoku =
	[](const MENTSU_ANALYSIS* const analysis, bool chin_itsu) -> bool {
		bool flag[TILE_SUIT_HONORS / TILE_SUIT_STEP]; bool yakuFlag = true;
		for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP); i++) flag[i] = true;
		for (int k = 1; k < (chin_itsu ? TILE_NONFLOWER_MAX : TILE_SUIT_HONORS); k++)
			if (analysis->TileCount[k] > 0)
				for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP); i++)
					if ((k / TILE_SUIT_STEP) != i) flag[i] = false;
		for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP); i++)
			yakuFlag &= flag[i];
		return yakuFlag;
	};
